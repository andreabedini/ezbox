/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : upnp_monitor.c
 *
 * Description  : ezbox upnp monitor daemon program
 *
 * Copyright (C) 2008-2011 by ezbox-project
 *
 * History      Rev       Description
 * 2011-12-19   0.1       Write it from scratch
 * ============================================================================
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#include "ezcd.h"

#if 1
#define DBG(format, args...) do {\
	FILE *fp = fopen("/dev/kmsg", "a"); \
	if (fp) { \
		fprintf(fp, format, ## args); \
		fclose(fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif

typedef struct task_node_s {
	char *cmd;
	long time;
	int interval;
	struct task_node_s *next;
} task_node_t;

static bool running = true;
static bool alarmed = false;
static task_node_t *task_queue = NULL;

static void terminate_handler(int sig)
{
	DBG("<6>upnp_monitor: terminated\n");
	running = false;
	/* alarm process a second later */
	alarm(1);
}

static void alarm_handler(int sig)
{
	DBG("<6>upnp_monitor: alarmed\n");
	if (running == false) {
		/* alarm to stop process a second later */
		alarm(1);
	}
	alarmed = true;
}

static void delete_task(task_node_t *task)
{
	if (task != NULL) {
		if (task->cmd != NULL)
			free(task->cmd);
		free(task);
	}
}

static task_node_t *insert_task(task_node_t *head, task_node_t *task)
{
	task_node_t *tp, *pre;

	if (task == NULL) {
		return head;
	}
	if (head == NULL) {
		task->next = NULL;
		return task;
	}

	pre = NULL;
	tp = head;
	while((tp != NULL) && (task->time >= tp->time)) {
		pre = tp;
		tp = tp->next;
	}

	if (pre == NULL) {
		/* task should be the first node in queue */
		task->next = head;
		return task;
	}
	else {
		pre->next = task;
		task->next = tp;
		return head;
	}
}

static void read_task()
{
	FILE *fp;
	char buf[256];
	int fargc;
	char *fargv[RC_MAX_ARGS];
	task_node_t *pre_task, *task;
	int interval;

	/* read new task from task file */
	fp = ezcfg_api_upnp_open_task_file("r");
	if (fp != NULL) {
		/* read task file */
		while (utils_file_get_line(fp, buf, sizeof(buf), "#", LINE_TAIL_STRING) == true) {
			fargc = utils_parse_upnp_task(buf, strlen(buf) + 1, fargv);
			if (fargc < 4) {
				continue;
			}

			if (strcmp(fargv[0], "add") == 0) {
				task = malloc(sizeof(task_node_t));
				if (task != NULL) {
					memset(task, 0, sizeof(task_node_t));
					task->cmd = strdup(fargv[1]); /* command */
					if (task->cmd != NULL) {
						task->time = atol(fargv[2]); /* execute time */
						task->interval = atoi(fargv[3]); /* repeat interval */
						task->next = NULL;
						task_queue = insert_task(task_queue, task);
					}
					else {
						/* strdup command string error! */
						DBG("<6>upnp_monitor: strdup\n");
						free(task);
					}
				}
			}
			else if (strcmp(fargv[0], "del") == 0) {
				interval = atoi(fargv[3]);
				pre_task = NULL;
				task = task_queue;
				while (task != NULL) {
					if ((strcmp(task->cmd, fargv[1]) == 0) && (interval == task->interval)){
						if (pre_task == NULL) {
							/* first task node match */
							task_queue = task->next;
							delete_task(task);
							task = task_queue;
						}
						else {
							pre_task->next = task->next;
							delete_task(task);
							task = pre_task->next;
						}
					}
					else {
						pre_task = task;
						task = pre_task->next;
					}
				}
			}
		}
		ezcfg_api_upnp_close_task_file(fp);
	}
}

int upnp_monitor_main(int argc, char **argv)
{
	pid_t pid;
	proc_stat_t *pidList;
	task_node_t *task;
	int i;

	/* first check if upnp_monitor has run */
	pid = getpid();
	pidList = utils_find_pid_by_name("upnp_monitor");
	if (pidList == NULL) {
		printf("find upnp_monitor pid error!\n");
		return (EXIT_FAILURE);
	}

	for (i=0; pidList[i].pid > 0; i++) {
		if (pidList[i].pid != pid) {
			free(pidList);
			printf("upnp_monitor is running!\n");
			return (EXIT_FAILURE);
		}
	}

	free(pidList);

	/* then fork to run as a daemon */
	pid = fork();
	if (pid < 0) {
		DBG("<6>upnp_monitor: can't fork");
		return (EXIT_FAILURE);
	}
	if (pid > 0) {
		return (EXIT_SUCCESS); /* parent */
	}

	/* child */
	/* unset umask */
	umask(0);

	signal(SIGTERM, terminate_handler);
	signal(SIGALRM, alarm_handler);

	running = true;
	alarmed = false;
	/* main loop */
	while (running == true) {
		struct sysinfo si;

		/* clean alarmed flag */
		alarmed = false;

		/* read new task from task file */
		read_task();

		/* execute task in queue */
		if (sysinfo(&si) < 0) {
			DBG("<6>upnp_monitor: sysinfo\n");
			continue;
		}
		task = task_queue;
		while((task != NULL) && (task->time <= si.uptime)) {
			utils_system(task->cmd);
			task_queue = task->next;
			task->next = NULL;
			if (task->interval > 0) {
				task->time = si.uptime + task->interval;
				task_queue = insert_task(task_queue, task);
			}
			else {
				delete_task(task);
			}
			task = task_queue;
		}

		/* setup sleep time */
		if (task == NULL) {
			/* wait for signal */
			if (alarmed == false)
				sleep(10);
		}
		else {
			if (sysinfo(&si) < 0) {
				DBG("<6>upnp_monitor: sysinfo\n");
				continue;
			}
			if (task->time > si.uptime) {
				if (alarmed == false)
					sleep(task->time - si.uptime);
			}
		}
	}

	/* read task from task file */
	read_task();

	/* clean up task queue */
	task = task_queue;
	while (task != NULL) {
		if (task->time == 0) {
			utils_system(task->cmd);
		}
		task_queue = task->next;
		delete_task(task);
		task = task_queue;
	}
		
	return (EXIT_SUCCESS);
}
