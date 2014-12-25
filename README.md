announce
========

Announce services on the network with Zeroconf/Bonjour with minimal overhead

Description
--
This announces services such as ssh, sftp, and http running on the local machine to the network. Specifically geared toward embedded systems running OpenWrt. It is based on tinysvcmdns.

Building
--
To build, pull this repository into the ```package/``` subdirectory in the OpenWrt SDK, then run ```scripts/feeds install libpthread``` and finally ```make V=s```. This will create ./bin/ar71xx/packages/base/announce_1.0-1_ar71xx.ipk (depending on your architecture)
 
TODO
--

* Find a good way to check which services are running and act accordingly (in a loop); currently ssh and http is (always) announced
* Write a proper OpenWrt Makefile; then write a patch and get it into the official OpenWrt repository
