/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : http/admin/http_html_admin_foot.c
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2011 by ezbox-project
 *
 * History      Rev       Description
 * 2011-04-18   0.1       Write it from scratch
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <pthread.h>

#include <locale.h>
#include <libintl.h>

#include "ezcfg.h"
#include "ezcfg-private.h"
#include "ezcfg-http.h"
#include "ezcfg-html.h"

/**
 * Private functions
 **/

/**
 * Public functions
 **/

int ezcfg_http_html_admin_set_html_foot(
	struct ezcfg_http *http,
	struct ezcfg_nvram *nvram,
	struct ezcfg_html *html,
	int pi, int si)
{
	struct ezcfg *ezcfg;
	struct ezcfg_locale *locale = NULL;
	int foot_index, child_index;
	int ret = -1;

	ASSERT(http != NULL);
	ASSERT(nvram != NULL);
	ASSERT(html != NULL);
	ASSERT(pi > 1);

	ezcfg = html->ezcfg;

        /* set locale info */
	locale = ezcfg_locale_new(ezcfg);
	if (locale != NULL) {
		ezcfg_locale_set_domain(locale, EZCFG_HTTP_HTML_ADMIN_FOOT_DOMAIN);
		ezcfg_locale_set_dir(locale, EZCFG_HTTP_HTML_LANG_DIR);
	}

	/* <div id="foot"> */
	foot_index = ezcfg_html_add_body_child(html, pi, si, EZCFG_HTML_DIV_ELEMENT_NAME, NULL);
	if (foot_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, foot_index, EZCFG_HTML_ID_ATTRIBUTE_NAME, EZCFG_HTTP_HTML_ADMIN_DIV_ID_FOOT, EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	/* <p>2010-2011 &#169; ezbox</p> */
	child_index = ezcfg_html_add_body_child(html, foot_index, -1, EZCFG_HTML_P_ELEMENT_NAME, ezcfg_locale_text(locale, "2010-2011 &#169; ezbox"));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child error.\n");
		goto func_exit;
	}

	/* must return menu index */
	ret = foot_index;
func_exit:
	if (locale != NULL)
		ezcfg_locale_delete(locale);

	return ret;
}
