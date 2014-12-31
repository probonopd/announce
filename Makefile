#
# To build, pull the announce repository into the "package/" subdirectory in the OpenWrt SDK,
# then run "scripts/feeds install libpthread" and finally "make V=s".
# This will create ./bin/ar71xx/packages/base/announce_1.0_ar71xx.ipk (depending on your architecture)
#

include $(TOPDIR)/rules.mk

PKG_NAME:=announce
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DEPENDS:= +libpthread

define Package/announce
  SUBMENU:=Utilities
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Announce services on the network with Zeroconf/Bonjour 
  URL:=https://github.com/probonopd/announce
  DEPENDS:= +libpthread 
endef

define Package/announce/description
  Announce services on the network with Zeroconf/Bonjour.
  This announces services such as ssh, sftp, and http running on the local machine
  to the network.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Package/announce/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/announce $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/announce.initscript $(1)/etc/init.d/announce
endef

$(eval $(call BuildPackage,announce))
