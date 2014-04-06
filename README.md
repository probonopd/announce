announce
========

Announce services to the network via Zeroconf/Bonjour

Description
--

announce is a minimalistic way to announce services that are running on a machine to the network with Zeroconf/Bonjour. It is specifically targeted toward embedded systems, where a full avahi-daemon is too much overhead.

Its targets are:
 
* Be just a few KB large so that it can be installed on OpenWrt routers without much need for space
* Announce services based on what is actually running (automatically stop announcing if a servie goes down)
* Announce services on all available network ports (e.g., start announcing once WLAN goes up)
* Be fully configurable using shell and init scripts
 
This is achieved by:

* Running a shell script every few seconds that checks which IP addresses are available and which services are running locally

Installation
--
I have precompiled announce for ar71xx architecture. You can find the ipk package inside the feed in this repository.

Simply download it and run
opkg install announce_1_ar71xx.ipk on the target machine to install it.

Building from source
--
I did NOT use a full OpenWrt Buildroot to compile this, since I did not like the overhead involved. Instead I just used the toolchain and a simple script to package the ipk. This saves lots of download bandwidth, learning time, and hassle.

Instructions to be written
 
TODO
--

* Make everything more CPU/RAM efficient, e.g., by moving parts of the bash code to C (pull requests welcome)
