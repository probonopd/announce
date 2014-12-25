announce
========

Announce services to the network via Zeroconf/Bonjour

Description
--
announce is a minimalistic way to announce ssh and http services that are running on an OpenWrt machine by default to the network with Zeroconf/Bonjour. It is specifically targeted toward embedded systems, where a full avahi-daemon is too much overhead.

Installation
--
I have precompiled announce for ar71xx architecture. You can find the ipk package inside the feed in this repository.

Simply download it and run
opkg install announce_1_ar71xx.ipk on the target machine to install it.
 
TODO
--

* Find a good way to check which services are running and act accordingly (in a loop); currently ssh and http is (always) announced
* Write a proper OpenWrt Makefile; then write a patch and get it into the official OpenWrt repository
