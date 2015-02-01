announce [![Build Status](https://travis-ci.org/probonopd/announce.svg)](https://travis-ci.org/probonopd/announce)
========

Announce services on the network with Zeroconf/Bonjour with minimal overhead

Description
--
This announces services such as ssh, sftp, and http running on the local machine to the network. Specifically geared toward embedded systems running OpenWrt. It is based on tinysvcmdns. 

Note that this could become obsolete once OpenWrt completes **mdns** and integrates it fully into the system; however, so far it is not well documented and I could not figure out how to use it yet. It also appears not to be finished yet. http://git.openwrt.org/?p=project/mdnsd.git;a=tree; In the meantime, announce does its job for me even though it is very basic.

Downloading
--
For trunk, announce is in the official OpenWrt packages feed and can be installed and enabled with `opkg install announce ; /etc/init.d/announce enable ; /etc/init.d/announce start`.

For barrier breaker, run
```
echo "src/gz announce http://probonopd.github.io/announce" >> /etc/opkg.conf
opkg update
opkg install announce
# Enable and start the service with:
/etc/init.d/announce enable
/etc/init.d/announce start
```
Check the ```.travis.yml``` file to see how this is compiled on http://travis-ci.org automatically. Please file an issue if you need addional architectures and/or OpenWrt versions.

Building
--
To build, pull this repository into the ```package/``` subdirectory in the OpenWrt SDK with ```git clone https://github.com/probonopd/announce.git```, then run ```scripts/feeds install libpthread``` and finally ```make V=s```. This will create ./bin/ar71xx/packages/base/announce_1.0-1_ar71xx.ipk (depending on your architecture).

Here is a detailed log of how this can be successfully compiled on a 64-bit host on which I do not have root rights:

```
wget https://downloads.openwrt.org/barrier_breaker/14.07/ar71xx/generic/OpenWrt-SDK-ar71xx-for-linux-x86_64-gcc-4.8-linaro_uClibc-0.9.33.2.tar.bz2
tar xfj OpenWrt-SDK-*.tar.bz2
cd OpenWrt-SDK-*/

cd package/
git clone https://github.com/probonopd/announce.git
cd -

scripts/feeds update packages

# This is ONLY needed for trunk but NOT for barrier_breaker
ls ./staging_dir/toolchain-*/lib/libpthread.so.0 || scripts/feeds install libpthread

# The following is ONLY needed for barrier_breaker but NOT for trunk
mkdir -p local ; cd local
wget "http://us.archive.ubuntu.com/ubuntu/pool/main/c/ccache/ccache_3.1.6-1_amd64.deb"
ar x ccache_3.1.6-1_amd64.deb 
tar xfz data.tar.gz
cd -
export PATH=/home/irmagic/projects/openwrt_barrier_breaker/OpenWrt-SDK-*/local/usr/bin/:$PATH

make V=s
```
Check the ```.travis.yml``` file to see how this is compiled on http://travis-ci.org automatically.

TODO
--
* Find a good way to check which services are running and act accordingly (in a loop)
* See https://github.com/probonopd/announce/issues
Pull requests are welcome.
