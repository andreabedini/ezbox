#!/bin/sh -e

autoreconf --verbose --install --symlink --force

EZCFG_CFLAGS="-g -Wall \
-Wmissing-declarations -Wmissing-prototypes \
-Wnested-externs -Wpointer-arith \
-Wpointer-arith -Wsign-compare -Wchar-subscripts \
-Wstrict-prototypes -Wshadow \
-Wformat=2 -Wtype-limits"

case "$CFLAGS" in
	*-O[0-9]*)
		;;
	*)
		EZCFG_CFLAGS="$EZCFG_CFLAGS -O2"
		;;
esac

libdir() {
	echo $(cd $1/$(gcc -print-multi-os-directory); pwd)
}

#args="--prefix=/usr/local \
#--sysconfdir=/etc \
#--sbindir=/sbin \
#--libdir=$(libdir /lib) \
#--libexecdir=/lib/ezcfg \
#--includedir=/usr/include \
#--enable-ezcfg_test \
#--enable-ezcfg_nls"

args="--prefix=/usr/local \
--sysconfdir=/etc \
--with-ezbox-distro=fuxi \
--enable-service-ezcfg_httpd \
--enable-service-ezcfg_httpd_cgi_index \
--enable-service-ezcfg_httpd_cgi_admin \
--enable-service-ezcfg_httpd_cgi_nvram \
--enable-service-ezcfg_httpd_ssi \
--enable-service-ezcfg_upnpd \
--enable-service-ezcfg_upnpd_igd1 \
--enable-service-openssl \
--enable-service-iptables \
--enable-service-ezctp \
"

export CFLAGS="$CFLAGS $EZCFG_CFLAGS"
./configure $args $@
