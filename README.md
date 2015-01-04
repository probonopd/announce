OpenWrt repository for announce
========

Binaries built from this repository on 2015-01-04 at 18:32:23 can be downloaded from http://probonopd.github.io/announce/.

To install the announce package, run
```
echo "src/gz announce http://probonopd.github.io/announce" >> /etc/opkg.conf
opkg update
opkg install announce
```
