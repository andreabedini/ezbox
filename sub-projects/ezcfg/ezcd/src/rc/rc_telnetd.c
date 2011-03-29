/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : rc_telnetd.c
 *
 * Description  : ezbox run telnet service
 *
 * Copyright (C) 2010 by ezbox-project
 *
 * History      Rev       Description
 * 2010-11-17   0.1       Write it from scratch
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

#include "ezcd.h"

int rc_telnetd(int flag)
{
	int rc;
	rc = nvram_match(NVRAM_SERVICE_OPTION(RC, TELNETD_ENABLE), "1");
	if (rc < 0) {
		return (EXIT_FAILURE);
	}
	switch (flag) {
	case RC_START :
		system("start-stop-daemon -S -b -n telnetd -a /usr/sbin/telnetd -- -l /bin/sh");
		break;

	case RC_STOP :
		system("start-stop-daemon -K -n telnetd");
		break;

	case RC_RESTART :
		rc = rc_telnetd(RC_STOP);
		rc = rc_telnetd(RC_START);
		break;
	}
	return (EXIT_SUCCESS);
}