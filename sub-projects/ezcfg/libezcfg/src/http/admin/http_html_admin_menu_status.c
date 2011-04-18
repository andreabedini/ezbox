/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : http/admin/http_html_admin_menu_status.c
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2011 by ezbox-project
 *
 * History      Rev       Description
 * 2011-04-15   0.1       Write it from scratch
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
static int submenu_status_system(struct ezcfg_html *html, int pi, int si, struct ezcfg_locale *locale)
{
	struct ezcfg *ezcfg;
	int li2_index, a2_index;
	int child_index;
	int ret = -1;

	ASSERT(html != NULL);
	ASSERT(pi > 1);

	ezcfg = html->ezcfg;

	/* status_system */
	/* submenu <ul> <li> */
	li2_index = ezcfg_html_add_body_child(html, pi, si, EZCFG_HTML_LI_ELEMENT_NAME, NULL);
	if (li2_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* submenu <ul> <li> <a> */
	a2_index = ezcfg_html_add_body_child(html, li2_index, -1, EZCFG_HTML_A_ELEMENT_NAME, ezcfg_locale_text(locale, "System"));
	if (a2_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, a2_index, EZCFG_HTML_HREF_ATTRIBUTE_NAME, "/admin/status_system", EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	/* submenu <ul> <li> <a> <span> */
	child_index = ezcfg_html_add_body_child(html, a2_index, -1, EZCFG_HTML_SPAN_ELEMENT_NAME, ezcfg_locale_text(locale, "The status of the system -- You can check system information and so on."));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* must return menu index */
	ret = li2_index;
func_exit:

	return ret;
}

static int submenu_status_lan(struct ezcfg_html *html, int pi, int si, struct ezcfg_locale *locale)
{
	struct ezcfg *ezcfg;
	int li2_index, a2_index;
	int child_index;
	int ret = -1;

	ASSERT(html != NULL);
	ASSERT(pi > 1);

	ezcfg = html->ezcfg;

	/* status_lan */
	/* submenu <ul> <li> */
	li2_index = ezcfg_html_add_body_child(html, pi, si, EZCFG_HTML_LI_ELEMENT_NAME, NULL);
	if (li2_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* submenu <ul> <li> <a> */
	a2_index = ezcfg_html_add_body_child(html, li2_index, -1, EZCFG_HTML_A_ELEMENT_NAME, ezcfg_locale_text(locale, "LAN"));
	if (a2_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, a2_index, EZCFG_HTML_HREF_ATTRIBUTE_NAME, "/admin/status_lan", EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	/* submenu <ul> <li> <a> <span> */
	child_index = ezcfg_html_add_body_child(html, a2_index, -1, EZCFG_HTML_SPAN_ELEMENT_NAME, ezcfg_locale_text(locale, "The status of LAN -- You can check local area network information and so on."));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* must return menu index */
	ret = li2_index;
func_exit:

	return ret;
}

static int submenu_status_wan(struct ezcfg_html *html, int pi, int si, struct ezcfg_locale *locale)
{
	struct ezcfg *ezcfg;
	int li2_index, a2_index;
	int child_index;
	int ret = -1;

	ASSERT(html != NULL);
	ASSERT(pi > 1);

	ezcfg = html->ezcfg;

	/* status_lan */
	/* submenu <ul> <li> */
	li2_index = ezcfg_html_add_body_child(html, pi, si, EZCFG_HTML_LI_ELEMENT_NAME, NULL);
	if (li2_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* submenu <ul> <li> <a> */
	a2_index = ezcfg_html_add_body_child(html, li2_index, -1, EZCFG_HTML_A_ELEMENT_NAME, ezcfg_locale_text(locale, "WAN"));
	if (a2_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, a2_index, EZCFG_HTML_HREF_ATTRIBUTE_NAME, "/admin/status_wan", EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	/* submenu <ul> <li> <a> <span> */
	child_index = ezcfg_html_add_body_child(html, a2_index, -1, EZCFG_HTML_SPAN_ELEMENT_NAME, ezcfg_locale_text(locale, "The status of WAN -- You can check wide area network information and so on."));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* must return menu index */
	ret = li2_index;
func_exit:

	return ret;
}


/**
 * Public functions
 **/
int ezcfg_http_html_admin_html_menu_status(struct ezcfg_html *html, int pi, int si)
{
	struct ezcfg *ezcfg;
	struct ezcfg_locale *locale = NULL;
	int li_index, a_index;
	int ul2_index, li2_index;
	int child_index;
	int ret = -1;

	ASSERT(html != NULL);
	ASSERT(pi > 1);

	ezcfg = html->ezcfg;

        /* set locale info */
	locale = ezcfg_locale_new(ezcfg);
	if (locale != NULL) {
		ezcfg_locale_set_domain(locale, EZCFG_HTTP_HTML_ADMIN_MENU_DOMAIN);
		ezcfg_locale_set_dir(locale, EZCFG_HTTP_HTML_LANG_DIR);
	}

	/* menu <ul> <li> */
	li_index = ezcfg_html_add_body_child(html, pi, si, EZCFG_HTML_LI_ELEMENT_NAME, NULL);
	if (li_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* menu <ul> <li> <a> */
	a_index = ezcfg_html_add_body_child(html, li_index, -1, EZCFG_HTML_A_ELEMENT_NAME, ezcfg_locale_text(locale, "Status"));
	if (a_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}
	ezcfg_html_add_body_child_attribute(html, a_index, EZCFG_HTML_HREF_ATTRIBUTE_NAME, "/admin/status_system", EZCFG_XML_ELEMENT_ATTRIBUTE_TAIL);

	/* menu <ul> <li> <a> <span> */
	child_index = ezcfg_html_add_body_child(html, a_index, -1, EZCFG_HTML_SPAN_ELEMENT_NAME, ezcfg_locale_text(locale, "The status of the device -- You can check running info and so on."));
	if (child_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* submenu <ul> */
	ul2_index = ezcfg_html_add_body_child(html, li_index, a_index, EZCFG_HTML_UL_ELEMENT_NAME, NULL);
	if (ul2_index < 0) {
		err(ezcfg, "ezcfg_html_add_body_child err.\n");
		goto func_exit;
	}

	/* status_system */
	/* submenu <ul> <li> */
	li2_index = submenu_status_system(html, ul2_index, -1, locale);
	if (li2_index < 0) {
		err(ezcfg, "submenu_status_system err.\n");
		goto func_exit;
	}

	/* status_wan */
	/* submenu <ul> <li> */
	li2_index = submenu_status_lan(html, ul2_index, li2_index, locale);
	if (li2_index < 0) {
		err(ezcfg, "submenu_status_lan err.\n");
		goto func_exit;
	}

	/* status_wan */
	/* submenu <ul> <li> */
	li2_index = submenu_status_wan(html, ul2_index, li2_index, locale);
	if (li2_index < 0) {
		err(ezcfg, "submenu_status_wan err.\n");
		goto func_exit;
	}

	/* must return menu index */
	ret = li_index;
func_exit:
	if (locale != NULL)
		ezcfg_locale_delete(locale);

	return ret;
}
