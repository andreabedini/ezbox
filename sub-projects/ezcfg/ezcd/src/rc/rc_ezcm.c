/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : rc_ezcm.c
 *
 * Description  : ezbox populate ezcm config file
 *
 * Copyright (C) 2008-2011 by ezbox-project
 *
 * History      Rev       Description
 * 2011-05-24   0.1       Write it from scratch
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
#include "utils.h"
#include "pop_func.h"

#if 0
#define DBG printf
#else
#define DBG(format, arg...)
#endif

int rc_ezcm(int flag)
{
	switch (flag) {
	case RC_BOOT :
		/* generate ezcm config file */
		pop_etc_ezcm_conf(flag);
		break;

	case RC_RELOAD :
		/* re-generate ezcm config file */
		pop_etc_ezcm_conf(flag);
		break;
	}
	return (EXIT_SUCCESS);
}
