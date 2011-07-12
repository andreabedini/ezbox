/* ============================================================================
 * Project Name : ezbox Configuration Daemon
 * Module Name  : rc_emc2.c
 *
 * Description  : ezbox run EMC2 Enhanced Machine Controller service
 *
 * Copyright (C) 2008-2011 by ezbox-project
 *
 * History      Rev       Description
 * 2011-07-05   0.1       Write it from scratch
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
#include "rc_func.h"

#if 1
#define DBG(format, args...) do {\
	FILE *fp = fopen("/tmp/emc2.debug", "a"); \
	if (fp) { \
		fprintf(fp, format, ## args); \
		fclose(fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif

int rc_emc2(int flag)
{
	int rc;
	char *p;
	char name[64];
	char buf[64];
	char cmd[128];
	char ini_file[64];
	char ini_dir[64];
	char old_dir[64];
	int num, i;

	switch (flag) {
	case RC_START :
		rc = nvram_match(NVRAM_SERVICE_OPTION(RC, EMC2_ENABLE), "1");
		if (rc < 0) {
			return (EXIT_FAILURE);
		}

		mkdir("/etc/emc2", 0777);
		mkdir("/etc/emc2/configs", 0777);
		mkdir("/var/emc2", 0777);
		mkdir("/var/emc2/nc_files", 0777);

		/* create configs dir */
		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EMC2, INIFILE), ini_file, sizeof(ini_file));
		if (rc <= 0) {
			return (EXIT_FAILURE);
		}

		if (getcwd(old_dir, sizeof(old_dir)) == NULL) {
			snprintf(old_dir, sizeof(old_dir), "%s", ROOT_HOME_PATH);
		}
		strcpy(ini_dir, ini_file);
		p = strrchr(ini_dir, '/');
		if (p != NULL) {
			*p = '\0';
		}
		else {
			strcpy(ini_dir, old_dir);
		}

		snprintf(cmd, sizeof(cmd), "%s -p %s", CMD_MKDIR, ini_dir);
		DBG("huedbg: %s[%d] cmd=[%s]\n", __func__, __LINE__, cmd);
		system(cmd);

		pop_etc_emc2_rtapi_conf(RC_START);

		pop_etc_emc2_configs(RC_START);

		/* export NML_FILE */
		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EMC2, CONF_EMC_NML_FILE), buf, sizeof(buf));
		if (rc <= 0) {
			snprintf(buf, sizeof(buf), "/usr/share/emc/emc.nml");
		}
		setenv("NMLFILE", buf, 1);

		/* start EMC2 Enhanced Machine Controller service */
		/* change dir to ini_dir */
		chdir(ini_dir);

		/* Run emcserver in background, always (it owns/creates the NML buffers) */
		setenv("INI_FILE_NAME", ini_file, 1);
		snprintf(cmd, sizeof(cmd), "start-stop-daemon -S -b -n emcsvr -a /usr/bin/emcsvr -- -ini %s", ini_file);
		DBG("huedbg: %s[%d] cmd=[%s]\n", __func__, __LINE__, cmd);
		system(cmd);

		/* sleep a second */
		sleep(1);

		/* Start REALTIME */
		rc_realtime(RC_START);

		/* export the location of the HAL realtime modules so that
		 * "halcmd loadrt" can find them
		 */
		/* export HAL_RTMOD_DIR=$EMC2_RTLIB_DIR */
		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EMC2, RTLIB_DIR), buf, sizeof(buf));
		if (rc > 0) {
			setenv("HAL_RTMOD_DIR", buf, 1);
		}

		/* Run emcio in background */
		/* $HALCMD loadusr -Wn iocontrol $EMCIO -ini "$INIFILE" */
		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EMC2, CONF_EMCIO_EMCIO), buf, sizeof(buf));
		if (rc > 0) {
			snprintf(cmd, sizeof(cmd), "/usr/bin/halcmd loadusr -Wn iocontrol %s -ini %s", buf, ini_file);
			DBG("huedbg: %s[%d] cmd=[%s]\n", __func__, __LINE__, cmd);
			system(cmd);
		}

		/* Run halui in background, if necessary */
		/* $HALCMD loadusr -Wn halui $HALUI -ini "$INIFILE" */
		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EMC2, CONF_HAL_HALUI), buf, sizeof(buf));
		if (rc > 0) {
			snprintf(cmd, sizeof(cmd), "/usr/bin/halcmd loadusr -Wn halui %s -ini %s", buf, ini_file);
			DBG("huedbg: %s[%d] cmd=[%s]\n", __func__, __LINE__, cmd);
			system(cmd);
		}

		/* execute HALCMD config files (if any)
		 * get first config file name from ini file
		 */
		/* $HALCMD -i "$INIFILE" -f $CFGFILE */
		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EMC2, CONF_HAL_HALFILE_NUM), buf, sizeof(buf));
		if (rc > 0) {
			num = atoi(buf);
			for (i = 0; i < num; i++) {
				snprintf(name, sizeof(name), "%s_%d", NVRAM_SERVICE_OPTION(EMC2, CONF_HAL_HALFILE), i);
				rc = ezcfg_api_nvram_get(name, buf, sizeof(buf));
				if (rc > 0) {
					snprintf(cmd, sizeof(cmd), "/usr/bin/halcmd -i %s -f %s", ini_file, buf);
					DBG("huedbg: %s[%d] cmd=[%s]\n", __func__, __LINE__, cmd);
					system(cmd);
					sleep(1);
				}
			}
		}

		/* execute discrete HAL commands from ini file (if any)
		 * get first command from ini file
		 */
		/* $HALCMD $HALCOMMAND */
		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EMC2, CONF_HAL_HALCMD_NUM), buf, sizeof(buf));
		if (rc > 0) {
			num = atoi(buf);
			for (i = 0; i < num; i++) {
				snprintf(name, sizeof(name), "%s_%d", NVRAM_SERVICE_OPTION(EMC2, CONF_HAL_HALCMD), i);
				rc = ezcfg_api_nvram_get(name, buf, sizeof(buf));
				if (rc > 0) {
					snprintf(cmd, sizeof(cmd), "/usr/bin/halcmd %s", buf);
					DBG("huedbg: %s[%d] cmd=[%s]\n", __func__, __LINE__, cmd);
					system(cmd);
				}
			}
		}

		/* start the realtime stuff ticking */
		/* $HALCMD start */
		snprintf(cmd, sizeof(cmd), "/usr/bin/halcmd %s", "start");
		DBG("huedbg: %s[%d] cmd=[%s]\n", __func__, __LINE__, cmd);
		system(cmd);

		/* Run emctask in background */
		/* $EMCTASK -ini "$INIFILE" & */
		rc = ezcfg_api_nvram_get(NVRAM_SERVICE_OPTION(EMC2, CONF_TASK_TASK), buf, sizeof(buf));
		if (rc > 0) {
			snprintf(cmd, sizeof(cmd), "start-stop-daemon -S -b -n %s -a /usr/bin/%s -- -ini %s", buf, buf, ini_file);
			DBG("huedbg: %s[%d] cmd=[%s]\n", __func__, __LINE__, cmd);
			system(cmd);
		}

		/* Run display in foreground */
		/* keystick -ini "$INIFILE" */

		/* restore to original dir */
		chdir(old_dir);

		break;

	case RC_STOP :
		system("start-stop-daemon -K -s KILL -n emcsvr");
		break;

	case RC_RESTART :
		rc = rc_emc2(RC_STOP);
		sleep(1);
		rc = rc_emc2(RC_START);
		break;
	}
	return (EXIT_SUCCESS);
}
