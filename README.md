announce
========

Announce services to the network via Zeroconf/Bonjour

Description
--
announce is a minimalistic way to announce ssh and http services that are running on an OpenWrt machine by default to the network with Zeroconf/Bonjour. It is specifically targeted toward embedded systems, where a full avahi-daemon is too much overhead.

Building
--
To build, pull this repository into the ```package/``` subdirectory in the OpenWrt SDK, then run ```scripts/feeds install libpthread``` and finally ```make V=s```. This will create ./bin/ar71xx/packages/base/announce_1.0-1_ar71xx.ipk (depending on your architecture).
 
TODO
--

* Find a good way to check which services are running and act accordingly (in a loop); currently ssh and http is (always) announced
* Write a proper OpenWrt Makefile; then write a patch and get it into the official OpenWrt repository
