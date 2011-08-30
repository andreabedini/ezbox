#
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/Minimal
	NAME:=Minimal Profile (no drivers)
	PACKAGES:=uboot-ar71xx-nbg460n_550n_550nh
endef

define Profile/Minimal/Description
	Minimal package set compatible with most boards.
endef
$(eval $(call Profile,Minimal))
