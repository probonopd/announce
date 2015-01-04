OpenWrt repository for announce
========

Binaries built from this repository can be downloaded from http://probonopd.github.io/announce/.

To install the announce package, run
```
echo "src/gz announce http://probonopd.github.io/announce" >> /etc/opkg.conf
opkg update
opkg install announce
```
