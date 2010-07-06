/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : main.c
 *
 * Description  : EZCD main program
 *
 * Copyright (C) 2010 by ezbox-project
 *
 * History      Rev       Description
 * 2010-06-13   0.1       Write it from scratch
 * ============================================================================
 */

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

#include "ezcd.h"

static int ezcd_exit;

static void signal_handler(int sig_num)
{
	if (sig_num == SIGCHLD) {
		do {
			/* nothing */
		} while (waitpid(-1, &sig_num, WNOHANG) > 0);
	} else {
		ezcd_exit = sig_num;
	}
}

static void ezcd_show_usage(void)
{
	printf("Usage: ezcd [-d] [-c max_worker_threads]\n");
	printf("\n");
	printf("  -d\tdaemonize\n");
	printf("  -c\tmax worker threads\n");
	printf("  -h\thelp\n");
	printf("\n");
}

static void perror_msg(char *text)
{
	fprintf(stderr, "%s\n", text);
}

static int mem_size_mb(void)
{
	FILE *fp;
	char buf[4096];
	long int memsize = -1;

	fp = fopen("/proc/meminfo", "r");
	if (fp == NULL)
		return -1;

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		long int value;

		if (sscanf(buf, "MemTotal: %ld kB", &value) == 1) {
			memsize = value / 1024;
			break;
		}
	}

	fclose(fp);
	return memsize;
}

static int ezcd_main(int argc, char **argv)
{
	int daemonize = 0;
	int c = 0;
	int threads_max = 0;
	struct ezcd_context *ctx;

	for (;;) {
		c = getopt( argc, argv, "c:dh");
		if (c == EOF) break;
		switch (c) {
			case 'c':
				threads_max = atoi(optarg);
				break;
			case 'd':
				daemonize = 1;
				break;
			case 'h':
			default:
				ezcd_show_usage();
				exit(EXIT_FAILURE);
		}
        }

	ezcd_exit = 0;
	signal(SIGCHLD, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);

	ctx = ezcd_start();
	if (ctx == NULL) {
		printf("%s\n", "Cannot initialize ezcd context");
		exit(EXIT_FAILURE);
	}

	if (threads_max <= 0) {
		int memsize = mem_size_mb();

		/* set value depending on the amount of RAM */
		if (memsize > 0)
			threads_max = 2 + (memsize / 8);
		else
			threads_max = 2;
	}
	ezcd_set_threads_max(ctx, threads_max);

	if (daemonize)
	{
		printf("daemonizing\n");
		if (daemon(0, 1) < 0) {
			perror_msg("daemon");
			ezcd_stop(ctx);
			exit(EXIT_FAILURE);
		}
	}

	fflush(stdout);
	while (ezcd_exit == 0)
		sleep(1);

	printf("Exiting on signal %d, "
		"waiting for all threads to finish...", ezcd_exit);
	fflush(stdout);
	ezcd_stop(ctx);
	printf("%s", " done.\n");

	return (EXIT_SUCCESS);
}

static int sock_read (int fd, void* buff, int count)
{
	void* pts = buff;
	int status = 0, n;

	if (count <= 0) return SOCKERR_OK;

	while (status != count) {
		n = read (fd, pts + status, count - status);

		if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			else
				return SOCKERR_IO;
		}

		if (n == 0)
			return SOCKERR_CLOSED;

		status += n;
	}

	return status;
}

static int sock_write (int fd, const void* buff, int count)
{
	const void* pts = buff;
	int status = 0, n;

	if (count < 0) return SOCKERR_OK;

	while (status != count) {
		n = write (fd, pts + status, count - status);
		if (n < 0) {
			if (errno == EPIPE)
				return SOCKERR_CLOSED;
			else if (errno == EINTR) {
				continue;
			}
			else
				return SOCKERR_IO;
		}
		status += n;
	}

	return status;
}

static int ezci_main(int argc, char **argv)
{
	int c = 0;
	int conn_fd = -1;
	int n = 0;
	char buf[32];
	char data[32];

	for (;;) {
		c = getopt( argc, argv, "h");
		if (c == EOF) break;
		switch (c) {
			case 'h':
			default:
				ezcd_show_usage();
				exit(EXIT_FAILURE);
		}
        }

	conn_fd = ezcd_client_connection(EZCD_SOCKET_PATH, 'a');
	if (conn_fd < 0) {
		printf("error : %s\n", "ezcd_client_connection");
		exit(EXIT_FAILURE);
	}

	memset(buf, 0, sizeof(buf));
	strcpy(buf, "mytest!");
        n = sock_write (conn_fd, buf, sizeof (buf));

        if (n == SOCKERR_IO) {
            printf ("write error on fd %d\n", conn_fd);
        }
        else if (n == SOCKERR_CLOSED) {
            printf ("fd %d has been closed.\n", conn_fd);
        }
        else
            printf ("Wrote %s to server. \n", buf);

	memset(data, 0, sizeof(data));
        n = sock_read (conn_fd, data, sizeof (data));
        if (n == SOCKERR_IO) {
            printf ("read error on fd %d\n", conn_fd);
        }
        else if (n == SOCKERR_CLOSED) {
            printf ("fd %d has been closed.\n", conn_fd);
        }
        else
            printf ("Got that! data is %s\n", data);

	return (EXIT_SUCCESS);
}

static int init_main(int argc, char **argv)
{
        FILE *file = NULL;
	char *init_argv[] = { "/sbin/init", NULL };
	int ret = 0;
	char kver[32] = "";

	/* unset umask */
	umask(0);

	/* /proc */
	mkdir("/proc", 0555);
	mount("proc", "/proc", "proc", MS_MGC_VAL, NULL);

	/* sysfs -> /sys */
	mkdir("/sys", 0755);
	mount("sysfs", "/sys", "sysfs", MS_MGC_VAL, NULL);

	/* get kernel version */
	file = fopen("/proc/version", "r");
	if (file != NULL)
	{
		if (fgets(kver, 15, file) != NULL)
		{
			if (!strcmp(kver, "Linux version "))
			{
				if (fgets(kver, sizeof(kver), file) != NULL)
				{
					char *p = strchr(kver, ' ');
					if (p)
					{
						*p = '\0';
					}
				}
				else
				{
					kver[0] = '\0';
				}
			}
			else
			{
				kver[0] = '\0';
			}
		}
		fclose(file);
	}


	/* /dev */
	mkdir("/dev", 0755);
	// mount("tmpfs", "/dev", "tmpfs", MS_MGC_VAL, NULL);

	/* /etc */
	mkdir("/etc", 0755);

	/* /tmp */
	mkdir("/tmp", 0777);

	/* /var */
	mkdir("/var", 0777);

	/* hotplug2 */
	mknod("/dev/console", S_IRWXU|S_IFCHR, makedev(5, 1));
	ret = system("/sbin/hotplug2 --set-worker /lib/hotplug2/worker_fork.so --set-rules-file /etc/hotplug2-init.rules --no-persistent --set-coldplug-cmd /sbin/udevtrigger");
	ret = system("/sbin/hotplug2 --set-worker /lib/hotplug2/worker_fork.so --set-rules-file /etc/hotplug2-init.rules --persistent &");

	/* init shms */
	mkdir("/dev/shm", 0777);

	/* Mount /dev/pts */
	mkdir("/dev/pts", 0777);
	mount("devpts", "/dev/pts", "devpts", MS_MGC_VAL, NULL);

	/* init hotplug */
	file = fopen("/proc/sys/kernel/hotplug", "w");
	if (file != NULL)
	{
		fprintf(file, "%s", "");
		fclose(file);
	}

	/* load preinit kernel modules */
	file = fopen("/etc/modules", "r");
	if (file != NULL)
	{
		char cmd[64];
		char buf[32];
		while(fgets(buf, sizeof(buf), file) != NULL)
		{
			if(buf[0] != '#')
			{
				int len = strlen(buf);
				while((len > 0) && 
				      (buf[len] == '\0' || 
				       buf[len] == '\r' || 
				       buf[len] == '\n'))
				{
					buf[len] = '\0';
					len --;
				}
				if(len > 0)
				{
					snprintf(cmd, sizeof(cmd), "insmod /lib/modules/%s/%s.ko", kver, buf);
					ret = system(cmd);
				}
			}
		}
		fclose(file);
	}

	/* switch to /sbin/init */
	execv(init_argv[0], init_argv);

	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	char *name = strrchr(argv[0], '/');
	name = name ? name+1 : argv[0];

	if (!strcmp(name, "init")) {
		return init_main(argc, argv);
	}
	else if (!strcmp(name, "ezcd")) {
		return ezcd_main(argc, argv);
	}
	else if (!strcmp(name, "ezci")) {
		return ezci_main(argc, argv);
	}
	else {
		printf("Unkown name [%s]!\n", name);
		return (EXIT_FAILURE);
	}
}
