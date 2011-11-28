/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : thread/worker_ssdp.c
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2008-2011 by ezbox-project
 *
 * History      Rev       Description
 * 2011-11-13   0.1       Split it from worker.c
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stddef.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/un.h>
#include <pthread.h>

#include "ezcfg.h"
#include "ezcfg-private.h"
#include "ezcfg-soap_http.h"

#if 1
#define DBG(format, args...) do { \
	char path[256]; \
	FILE *fp; \
	snprintf(path, 256, "/tmp/%d-debug.txt", getpid()); \
	fp = fopen(path, "a"); \
	if (fp) { \
		fprintf(fp, format, ## args); \
		fclose(fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif

void ezcfg_worker_process_ssdp_new_connection(struct ezcfg_worker *worker)
{
	char *buf;
	int buf_len;
	struct ezcfg *ezcfg;
	struct ezcfg_ssdp *ssdp;

	ASSERT(worker != NULL);

	ssdp = (struct ezcfg_ssdp *)(ezcfg_worker_get_proto_data(worker));
	ASSERT(ssdp != NULL);

	ezcfg = ezcfg_worker_get_ezcfg(worker);
	buf = ezcfg_socket_get_buffer(ezcfg_worker_get_client(worker));
	if (buf == NULL) {
		err(ezcfg, "not enough memory for processing ssdp new connection\n");
		return;
	}
	buf_len = ezcfg_socket_get_buffer_len(ezcfg_worker_get_client(worker));

	info(ezcfg, "ssdp=[%s], len=%d\n", buf, buf_len);
	return;
}
