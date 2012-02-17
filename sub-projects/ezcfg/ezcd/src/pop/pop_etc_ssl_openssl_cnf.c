/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : pop_etc_ssl_openssl_cnf.c
 *
 * Description  : ezbox /etc/ssl/openssl.cnf file generating program
 *
 * Copyright (C) 2008-2012 by ezbox-project
 *
 * History      Rev       Description
 * 2012-02-17   0.1       Write it from scratch
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
#include "pop_func.h"

int pop_etc_ssl_openssl_cnf(int flag)
{
        FILE *file = NULL;
	char buf[128];
	//char *p, *token, *savep;
	char name[32];
	int i, rc, ret;

	switch(flag) {
	case RC_ACT_BOOT :
	case RC_ACT_START :
	case RC_ACT_RESTART :
		/* setup directories */
		mkdir(EZCFG_SSL_CONF_ROOT_PATH, 0755);
		mkdir(EZCFG_SSL_CERT_ROOT_PATH, 0755);
		mkdir(EZCFG_SSL_PRIV_ROOT_PATH, 0755);

		/* generate /etc/ssl/openssl.cnf */
		file = fopen(EZCFG_SSL_CONF_ROOT_PATH "/openssl.cnf", "w");
		if (file == NULL) {
			return (EXIT_FAILURE);
		}

		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(WAN, DOMAIN), buf, sizeof(buf));
		if ((rc == 0) && (*buf != '\0')) {
			fprintf(file, "domain %s\n", buf);
		}

		for (i = 1; i <= 3; i++) {
			snprintf(name, sizeof(name), "%s%d",
				NVRAM_SERVICE_OPTION(WAN, DNS), i);
			rc = ezcfg_api_nvram_get(name, buf, sizeof(buf));
			if (rc > 0) {
				fprintf(file, "nameserver %s\n", buf);
			}
		}
		fclose(file);
		ret = EXIT_SUCCESS;
		break;

	case RC_ACT_STOP :
		/* remove /etc/ssl/openssl.cnf */
		unlink(EZCFG_SSL_CONF_ROOT_PATH "/openssl.cnf");
		ret = EXIT_SUCCESS;
		break;

	default :
		ret = EXIT_FAILURE;
		break;
	}

	return ret;
}