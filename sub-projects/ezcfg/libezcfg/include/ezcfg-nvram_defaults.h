/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : ezcfg-nvram_defaults.h
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2010 by ezbox-project
 *
 * History      Rev       Description
 * 2010-11-16   0.1       Write it from scratch
 * ============================================================================
 */

#ifndef _EZCFG_NVRAM_DEFAULTS_H_
#define _EZCFG_NVRAM_DEFAULTS_H_

#include "ezcfg-ezcfg.h"
#include "ezcfg-lan.h"
#include "ezcfg-wan.h"
#include "ezcfg-rc.h"
#include "ezcfg-dnsmasq.h"

#define GLUE2(a, b)      	a ## b
#define GLUE3(a, b, c)   	a ## b ## c
#define GLUE4(a, b, c, d)	a ## b ## c ## d

#define NVRAM_PREFIX(service)      	GLUE3(EZCFG_, service, _NVRAM_PREFIX)
#define SERVICE_OPTION(service, name)	GLUE4(EZCFG_, service, _, name)

#define NVRAM_SERVICE_OPTION(service, name) \
	NVRAM_PREFIX(service) SERVICE_OPTION(service, name)

#endif