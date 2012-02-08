/* ============================================================================
 * Project Name : ezbox configuration utilities
 * File Name    : ezcfg-private.h
 *
 * Description  : interface to configurate ezbox information
 *
 * Copyright (C) 2008-2012 by ezbox-project
 *
 * History      Rev       Description
 * 2010-07-12   0.1       Write it from scratch
 * ============================================================================
 */

#ifndef _EZCFG_PRIVATE_H_
#define _EZCFG_PRIVATE_H_

#include <syslog.h>
#include <signal.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/syscall.h>

#include "ezcfg.h"
#include "ezcfg-types.h"

#define gettid() syscall(__NR_gettid)

typedef void * (*ezcfg_thread_func_t)(void *);
/* name-value pair struct */
typedef struct ezcfg_nv_pair_s {
	char *name;
	char *value;
} ezcfg_nv_pair_t;

/* nvram validator struct */
typedef struct ezcfg_nv_validator_s {
	char *name;
	bool (*handler)(const char *value);
} ezcfg_nv_validator_t;


/* rc service invocation struct */
typedef struct ezcfg_rc_triple_s {
	char *service;
	char *action;
	int wait;
} ezcfg_rc_triple_t;


/* common/ezcfg.c */
#include "ezcfg-priv_common.h"


/* thread/thread.c */
int ezcfg_thread_start(struct ezcfg *ezcfg, int stacksize,
	pthread_t *thread_id, ezcfg_thread_func_t func, void *param);


/* nvram/nvram-defaults.c */
extern ezcfg_nv_pair_t default_nvram_settings[];
extern char *default_nvram_savings[];
int ezcfg_nvram_get_num_default_nvram_settings(void);
int ezcfg_nvram_get_num_default_nvram_savings(void);


/* nvram/nvram-validators.c */
extern ezcfg_nv_validator_t default_nvram_validators[];
bool ezcfg_nvram_validate_value(struct ezcfg *ezcfg, char *name, char *value);
int ezcfg_nvram_get_num_default_nvram_validators(void);


/* nvram/nvram.c */
struct ezcfg_nvram;
bool ezcfg_nvram_delete(struct ezcfg_nvram *nvram);
struct ezcfg_nvram *ezcfg_nvram_new(struct ezcfg *ezcfg);
bool ezcfg_nvram_set_backend_type(struct ezcfg_nvram *nvram, const int index, const int type);
bool ezcfg_nvram_set_coding_type(struct ezcfg_nvram *nvram, const int index, const int type);
bool ezcfg_nvram_set_storage_path(struct ezcfg_nvram *nvram, const int index, const char *path);
bool ezcfg_nvram_set_total_space(struct ezcfg_nvram *nvram, const int total_space);
int ezcfg_nvram_get_total_space(struct ezcfg_nvram *nvram);
int ezcfg_nvram_get_free_space(struct ezcfg_nvram *nvram);
int ezcfg_nvram_get_used_space(struct ezcfg_nvram *nvram);
bool ezcfg_nvram_get_version_string(struct ezcfg_nvram *nvram, char *buf, size_t len);
bool ezcfg_nvram_get_storage_backend_string(struct ezcfg_nvram *nvram, const int index, char *buf, size_t len);
bool ezcfg_nvram_get_storage_coding_string(struct ezcfg_nvram *nvram, const int index, char *buf, size_t len);
bool ezcfg_nvram_get_storage_path_string(struct ezcfg_nvram *nvram, const int index, char *buf, size_t len);
bool ezcfg_nvram_set_default_settings(struct ezcfg_nvram *nvram, ezcfg_nv_pair_t *settings, int num_settings);
bool ezcfg_nvram_set_entry(struct ezcfg_nvram *nvram, const char *name, const char *value);
bool ezcfg_nvram_get_entry_value(struct ezcfg_nvram *nvram, const char *name, char **value);
bool ezcfg_nvram_unset_entry(struct ezcfg_nvram *nvram, const char *name);
bool ezcfg_nvram_get_all_entries_list(struct ezcfg_nvram *nvram, struct ezcfg_list_node *list);
bool ezcfg_nvram_commit(struct ezcfg_nvram *nvram);
bool ezcfg_nvram_fill_storage_info(struct ezcfg_nvram *nvram, const char *conf_path);
bool ezcfg_nvram_initialize(struct ezcfg_nvram *nvram);
bool ezcfg_nvram_load(struct ezcfg_nvram *nvram, int flag);
bool ezcfg_nvram_reload(struct ezcfg_nvram *nvram);
bool ezcfg_nvram_match_entry(struct ezcfg_nvram *nvram, char *name1, char *name2);
bool ezcfg_nvram_match_entry_value(struct ezcfg_nvram *nvram, char *name, char *value);
bool ezcfg_nvram_is_valid_entry_value(struct ezcfg_nvram *nvram, char *name, char *value);
bool ezcfg_nvram_insert_socket(struct ezcfg_nvram *nvram, struct ezcfg_link_list *list);
bool ezcfg_nvram_remove_socket(struct ezcfg_nvram *nvram, struct ezcfg_link_list *list);


/* auth/auth.c */
#include "ezcfg-priv_auth.h"
#if 0
struct ezcfg_auth * ezcfg_auth_new(struct ezcfg *ezcfg);
bool ezcfg_auth_delete(struct ezcfg_auth *auth);
bool ezcfg_auth_set_type(struct ezcfg_auth *auth, char *type);
bool ezcfg_auth_set_user(struct ezcfg_auth *auth, char *user);
bool ezcfg_auth_set_realm(struct ezcfg_auth *auth, char *realm);
bool ezcfg_auth_set_domain(struct ezcfg_auth *auth, char *domain);
bool ezcfg_auth_set_secret(struct ezcfg_auth *auth, char *secret);
bool ezcfg_auth_is_valid(struct ezcfg_auth *auth);
bool ezcfg_auth_list_in(struct ezcfg_auth **list, struct ezcfg_auth *auth);
bool ezcfg_auth_list_insert(struct ezcfg_auth **list, struct ezcfg_auth *auth);
void ezcfg_auth_list_delete(struct ezcfg_auth **list);
bool ezcfg_auth_check_authorized(struct ezcfg_auth **list, struct ezcfg_auth *auth);
#endif


/* locale/locale.c */
struct ezcfg_locale * ezcfg_locale_new(struct ezcfg *ezcfg);
bool ezcfg_locale_delete(struct ezcfg_locale *locale);
bool ezcfg_locale_set_domain(struct ezcfg_locale *locale, char *domain);
bool ezcfg_locale_set_dir(struct ezcfg_locale *locale, char *dir);
char * ezcfg_locale_text(struct ezcfg_locale *locale, char *msgid);


/* uuid/uuid.c */
struct ezcfg_uuid;
struct ezcfg_uuid *ezcfg_uuid_new(struct ezcfg *ezcfg, int version);
bool ezcfg_uuid_delete(struct ezcfg_uuid *uuid);
bool ezcfg_uuid_generate(struct ezcfg_uuid *uuid);
bool ezcfg_uuid_export_str(struct ezcfg_uuid *uuid, char *buf, int len);
int ezcfg_uuid_get_version(struct ezcfg_uuid *uuid);
bool ezcfg_uuid_set_store_name(struct ezcfg_uuid *uuid, const char *store_name);
char *ezcfg_uuid_get_store_name(struct ezcfg_uuid *uuid);
bool ezcfg_uuid_v1_set_mac(struct ezcfg_uuid *uuid, unsigned char *mac, int len);
bool ezcfg_uuid_v1_enforce_multicast_mac(struct ezcfg_uuid *uuid);


/* socket/socket.c */
#include "ezcfg-priv_socket.h"

/* http/http.c */
#include "ezcfg-priv_http.h"

/* html/html.c */
struct ezcfg_html;
void ezcfg_html_delete(struct ezcfg_html *html);
struct ezcfg_html *ezcfg_html_new(struct ezcfg *ezcfg);
void ezcfg_html_reset_attributes(struct ezcfg_html *html);
struct ezcfg *ezcfg_html_get_ezcfg(struct ezcfg_html *html);
unsigned short ezcfg_html_get_version_major(struct ezcfg_html *html);
unsigned short ezcfg_html_get_version_minor(struct ezcfg_html *html);
bool ezcfg_html_set_version_major(struct ezcfg_html *html, unsigned short major);
bool ezcfg_html_set_version_minor(struct ezcfg_html *html, unsigned short minor);
int ezcfg_html_get_max_nodes(struct ezcfg_html *html);
bool ezcfg_html_set_max_nodes(struct ezcfg_html *html, const int max_nodes);
int ezcfg_html_set_root(struct ezcfg_html *html, const char *name);
bool ezcfg_html_add_root_attribute(struct ezcfg_html *html, const char *name, const char *value, int pos);
int ezcfg_html_set_head(struct ezcfg_html *html, const char *name);
int ezcfg_html_get_head_index(struct ezcfg_html *html);
int ezcfg_html_add_head_child(struct ezcfg_html *html, int pi, int si, const char *name, const char *content);
bool ezcfg_html_add_head_child_attribute(struct ezcfg_html *html, int ei, const char *name, const char *value, int pos);
int ezcfg_html_set_body(struct ezcfg_html *html, const char *name);
int ezcfg_html_get_body_index(struct ezcfg_html *html);
int ezcfg_html_add_body_child(struct ezcfg_html *html, int pi, int si, const char *name, const char *content);
bool ezcfg_html_add_body_child_attribute(struct ezcfg_html *html, int ei, const char *name, const char *value, int pos);
int ezcfg_html_get_element_index(struct ezcfg_html *html, const int pi, const int si, char *name);
char *ezcfg_html_get_element_content_by_index(struct ezcfg_html *html, const int index);
bool ezcfg_html_parse(struct ezcfg_html *html, char *buf, int len);
int ezcfg_html_get_message_length(struct ezcfg_html *html);
int ezcfg_html_write_message(struct ezcfg_html *html, char *buf, int len);


/* css/css.c */
struct ezcfg_css;
void ezcfg_css_delete(struct ezcfg_css *css);
struct ezcfg_css *ezcfg_css_new(struct ezcfg *ezcfg);
unsigned short ezcfg_css_get_version_major(struct ezcfg_css *css);
unsigned short ezcfg_css_get_version_minor(struct ezcfg_css *css);
bool ezcfg_css_set_version_major(struct ezcfg_css *css, unsigned short major);
bool ezcfg_css_set_version_minor(struct ezcfg_css *css, unsigned short minor);
int ezcfg_css_add_rule_set(struct ezcfg_css *css, char *selector, char *name, char *value);
int ezcfg_css_get_message_length(struct ezcfg_css *css);
int ezcfg_css_write_message(struct ezcfg_css *css, char *buf, int len);


/* ssl/ssl.c */
#include "ezcfg-priv_ssl.h"


/* ssi/ssi.c */
struct ezcfg_ssi;
struct ezcfg_ssi *ezcfg_ssi_new(struct ezcfg *ezcfg, struct ezcfg_nvram *nvram);
void ezcfg_ssi_delete(struct ezcfg_ssi *ssi);
bool ezcfg_ssi_set_document_root(struct ezcfg_ssi *ssi, const char *root);
bool ezcfg_ssi_set_path(struct ezcfg_ssi *ssi, const char *path);
FILE *ezcfg_ssi_open_file(struct ezcfg_ssi *ssi, const char *mode);
int ezcfg_ssi_file_get_line(struct ezcfg_ssi *ssi, char *buf, size_t size);


/* http/http_ssi.c */
struct ezcfg_http_ssi;
void ezcfg_http_ssi_delete(struct ezcfg_http_ssi *hs);
struct ezcfg_http_ssi *ezcfg_http_ssi_new(
	struct ezcfg *ezcfg,
	struct ezcfg_http *http,
	struct ezcfg_nvram *nvram,
	struct ezcfg_ssi *ssi);
int ezcfg_http_handle_ssi_request(
	struct ezcfg_http *http,
	struct ezcfg_nvram *nvram,
	struct ezcfg_ssi *ssi);


/* http/http_nvram.c */
#include "ezcfg-priv_http_nvram.h"


/* http/http_html_index.c */
int ezcfg_http_handle_index_request(struct ezcfg_http *http, struct ezcfg_nvram *nvram);


/* http/http_html_admin.c */
int ezcfg_http_handle_admin_request(struct ezcfg_http *http, struct ezcfg_nvram *nvram);

struct ezcfg_http_html_admin;
struct ezcfg_http_html_admin *ezcfg_http_html_admin_new(
	struct ezcfg *ezcfg,
	struct ezcfg_http *http,
	struct ezcfg_nvram *nvram);
void ezcfg_http_html_admin_delete(struct ezcfg_http_html_admin *admin);
int ezcfg_http_html_admin_get_action(struct ezcfg_http_html_admin *admin);
bool ezcfg_http_html_admin_handle_post_data(struct ezcfg_http_html_admin *admin);
bool ezcfg_http_html_admin_save_settings(struct ezcfg_http_html_admin *admin);
struct ezcfg *ezcfg_http_html_admin_get_ezcfg(struct ezcfg_http_html_admin *admin);
struct ezcfg_http *ezcfg_http_html_admin_get_http(struct ezcfg_http_html_admin *admin);
struct ezcfg_html *ezcfg_http_html_admin_get_html(struct ezcfg_http_html_admin *admin);
bool ezcfg_http_html_admin_set_html(struct ezcfg_http_html_admin *admin, struct ezcfg_html *html);
struct ezcfg_nvram *ezcfg_http_html_admin_get_nvram(struct ezcfg_http_html_admin *admin);
struct ezcfg_link_list *ezcfg_http_html_admin_get_post_list(struct ezcfg_http_html_admin *admin);


/* http/admin/http_html_admin_common_head.c */
int ezcfg_http_html_admin_set_html_common_head(struct ezcfg_http_html_admin *admin, int pi, int si);
bool ezcfg_http_html_admin_set_http_html_common_header(struct ezcfg_http_html_admin *admin);
bool ezcfg_http_html_admin_set_http_css_common_header(struct ezcfg_http_html_admin *admin);

/* http/admin/http_html_admin_head.c */
int ezcfg_http_html_admin_set_html_head(
	struct ezcfg_http_html_admin *admin,
	int pi, int si);

/* http/admin/http_html_admin_foot.c */
int ezcfg_http_html_admin_set_html_foot(
	struct ezcfg_http_html_admin *admin,
	int pi, int si);

/* http/admin/http_html_admin_button.c */
int ezcfg_http_html_admin_set_html_button(
	struct ezcfg_http_html_admin *admin,
	int pi, int si);

/* http/admin/http_html_admin_menu.c */
int ezcfg_http_html_admin_set_html_menu(
	struct ezcfg_http_html_admin *admin,
	int pi, int si);

/* http/admin/http_html_admin_menu_status.c */
int ezcfg_http_html_admin_html_menu_status(
	struct ezcfg_http_html_admin *admin,
	int pi, int si);

/* http/admin/http_html_admin_menu_setup.c */
int ezcfg_http_html_admin_html_menu_setup(
	struct ezcfg_http_html_admin *admin,
	int pi, int si);

/* http/admin/http_html_admin_menu_management.c */
int ezcfg_http_html_admin_html_menu_management(
	struct ezcfg_http_html_admin *admin,
	int pi, int si);

/* http/admin/http_html_admin_menu_cnc.c */
#if (HAVE_EZBOX_SERVICE_EMC2 == 1)
int ezcfg_http_html_admin_html_menu_cnc(
	struct ezcfg_http_html_admin *admin,
	int pi, int si);
#endif

/* http/admin/http_html_admin_layout_css.c */
int ezcfg_http_html_admin_layout_css_handler(struct ezcfg_http_html_admin *admin);

/* http/admin/http_html_admin_status_system.c */
int ezcfg_http_html_admin_status_system_handler(struct ezcfg_http_html_admin *admin);
/* http/admin/http_html_admin_status_lan.c */
#if (HAVE_EZBOX_LAN_NIC == 1)
int ezcfg_http_html_admin_status_lan_handler(struct ezcfg_http_html_admin *admin);
#endif
/* http/admin/http_html_admin_view_dhcp_client_table.c */
int ezcfg_http_html_admin_view_dhcp_client_table_handler(struct ezcfg_http_html_admin *admin);
/* http/admin/http_html_admin_status_wan.c */
#if (HAVE_EZBOX_WAN_NIC == 1)
int ezcfg_http_html_admin_status_wan_handler(struct ezcfg_http_html_admin *admin);
#endif


/* http/admin/http_html_admin_setup_system.c */
int ezcfg_http_html_admin_setup_system_handler(struct ezcfg_http_html_admin *admin);
/* http/admin/http_html_admin_setup_lan.c */
#if (HAVE_EZBOX_LAN_NIC == 1)
int ezcfg_http_html_admin_setup_lan_handler(struct ezcfg_http_html_admin *admin);
#endif
/* http/admin/http_html_admin_setup_wan.c */
#if (HAVE_EZBOX_WAN_NIC == 1)
int ezcfg_http_html_admin_setup_wan_handler(struct ezcfg_http_html_admin *admin);
#endif


/* http/admin/http_html_admin_management_authz.c */
int ezcfg_http_html_admin_management_authz_handler(struct ezcfg_http_html_admin *admin);
/* http/admin/http_html_admin_setup_lan.c */
int ezcfg_http_html_admin_management_default_handler(struct ezcfg_http_html_admin *admin);
/* http/admin/http_html_admin_setup_wan.c */
int ezcfg_http_html_admin_management_upgrade_handler(struct ezcfg_http_html_admin *admin);

#if (HAVE_EZBOX_SERVICE_EMC2 == 1)
int ezcfg_http_html_admin_cnc_setup_handler(struct ezcfg_http_html_admin *admin);
int ezcfg_http_html_admin_cnc_default_handler(struct ezcfg_http_html_admin *admin);
int ezcfg_http_html_admin_cnc_latency_handler(struct ezcfg_http_html_admin *admin);
#endif

/* xml/xml.c */
#include "ezcfg-priv_xml.h"

/* soap/soap.c */
struct ezcfg_soap;
void ezcfg_soap_delete(struct ezcfg_soap *soap);
struct ezcfg_soap *ezcfg_soap_new(struct ezcfg *ezcfg);
void ezcfg_soap_reset_attributes(struct ezcfg_soap *soap);
unsigned short ezcfg_soap_get_version_major(struct ezcfg_soap *soap);
unsigned short ezcfg_soap_get_version_minor(struct ezcfg_soap *soap);
bool ezcfg_soap_set_version_major(struct ezcfg_soap *soap, unsigned short major);
bool ezcfg_soap_set_version_minor(struct ezcfg_soap *soap, unsigned short minor);
int ezcfg_soap_get_max_nodes(struct ezcfg_soap *soap);
bool ezcfg_soap_set_max_nodes(struct ezcfg_soap *soap, const int max_nodes);
int ezcfg_soap_set_envelope(struct ezcfg_soap *soap, const char *name);
bool ezcfg_soap_add_envelope_attribute(struct ezcfg_soap *soap, const char *name, const char *value, int pos);
int ezcfg_soap_set_body(struct ezcfg_soap *soap, const char *name);
int ezcfg_soap_get_body_index(struct ezcfg_soap *soap);
int ezcfg_soap_add_body_child(struct ezcfg_soap *soap, int pi, int si, const char *name, const char *content);
bool ezcfg_soap_add_body_child_attribute(struct ezcfg_soap *soap, int ei, const char *name, const char *value, int pos);
int ezcfg_soap_get_element_index(struct ezcfg_soap *soap, const int pi, const int si, char *name);
char *ezcfg_soap_get_element_content_by_index(struct ezcfg_soap *soap, const int index);
bool ezcfg_soap_parse(struct ezcfg_soap *soap, char *buf, int len);
int ezcfg_soap_get_message_length(struct ezcfg_soap *soap);
int ezcfg_soap_write_message(struct ezcfg_soap *soap, char *buf, int len);

/* soap/soap_http.c */
struct ezcfg_soap_http;
void ezcfg_soap_http_delete(struct ezcfg_soap_http *sh);
struct ezcfg_soap_http *ezcfg_soap_http_new(struct ezcfg *ezcfg);
struct ezcfg_soap *ezcfg_soap_http_get_soap(struct ezcfg_soap_http *sh);
struct ezcfg_http *ezcfg_soap_http_get_http(struct ezcfg_soap_http *sh);
unsigned short ezcfg_soap_http_get_soap_version_major(struct ezcfg_soap_http *sh);
unsigned short ezcfg_soap_http_get_soap_version_minor(struct ezcfg_soap_http *sh);
bool ezcfg_soap_http_set_soap_version_major(struct ezcfg_soap_http *sh, unsigned short major);
bool ezcfg_soap_http_set_soap_version_minor(struct ezcfg_soap_http *sh, unsigned short minor);
unsigned short ezcfg_soap_http_get_http_version_major(struct ezcfg_soap_http *sh);
unsigned short ezcfg_soap_http_get_http_version_minor(struct ezcfg_soap_http *sh);
bool ezcfg_soap_http_set_http_version_major(struct ezcfg_soap_http *sh, unsigned short major);
bool ezcfg_soap_http_set_http_version_minor(struct ezcfg_soap_http *sh, unsigned short minor);
char *ezcfg_soap_http_get_http_header_value(struct ezcfg_soap_http *sh, char *name);
void ezcfg_soap_http_reset_attributes(struct ezcfg_soap_http *sh);
bool ezcfg_soap_http_parse_header(struct ezcfg_soap_http *sh, char *buf, int len);
bool ezcfg_soap_http_parse_message_body(struct ezcfg_soap_http *sh);
char *ezcfg_soap_http_set_message_body(struct ezcfg_soap_http *sh, const char *body, int len);
void ezcfg_soap_http_dump(struct ezcfg_soap_http *sh);
int ezcfg_soap_http_get_message_length(struct ezcfg_soap_http *sh);
int ezcfg_soap_http_write_message(struct ezcfg_soap_http *sh, char *buf, int len);

/* igrs/igrs.c */
#include "ezcfg-priv_igrs.h"

/* igrs/igrs_isdp.c */
struct ezcfg_igrs_isdp;
void ezcfg_igrs_isdp_delete(struct ezcfg_igrs_isdp *isdp);
struct ezcfg_igrs_isdp *ezcfg_igrs_isdp_new(struct ezcfg *ezcfg);
void ezcfg_igrs_isdp_dump(struct ezcfg_igrs_isdp *isdp);

/* upnp/upnp_gena.c */
struct ezcfg_upnp_gena;
void ezcfg_upnp_gena_delete(struct ezcfg_upnp_gena *upnp_gena);
struct ezcfg_upnp_gena *ezcfg_upnp_gena_new(struct ezcfg *ezcfg);
void ezcfg_upnp_gena_reset_attributes(struct ezcfg_upnp_gena *upnp_gena);

/* thread/master.c */
#include "ezcfg-priv_master.h"


/* thread/worker.c */
#include "ezcfg-priv_worker.h"

/* ctrl/ctrl.c - daemon runtime setup */
#include "ezcfg-priv_ctrl.h"

/* util/util.c */
#include "ezcfg-priv_util.h"

/* uevent/uevent.c */
struct ezcfg_uevent;
void ezcfg_uevent_delete(struct ezcfg_uevent *uevent);
struct ezcfg_uevent *ezcfg_uevent_new(struct ezcfg *ezcfg);

/* upnp/upnp.c */
#include "ezcfg-priv_upnp.h"

#endif /* _EZCFG_PRIVATE_H_ */
