# cppcomb

This is a POC for FD.io/sweetcomb to prove that sysrepo can be used with VOM instead of developping an independant layer to interact with VAPI.

Advantages of using VOM over scvpp
==================================

* VOM offers a backward compatible API
* VOM is used in many other project (less bugs/more contributions)
* It would help putting VOM as the default configuration backward-compatible agen/middleware which should be used to interact with VPP
* There is an existing mechanism to recommit configurations in case of failure of VPP

Inconvenients of using VOM over scvpp
=====================================

* Mix C++ and C languages

# Install

1. Install VPP
2. Install sysrepo
3. Install ietf-interfaces yang model
```
$ cd src/yang
# sysrepoctl --install --yang ietf-interfaces@2018-02-20.yang
# sysrepoctl --install --yang iana-if-type@2017-01-19.yang 
```
4. Install cppcomb plugin
```
$ make
# make install
```

# Run 

1. `sysrepod -d -l3`
2. `sysrepo-plugind -d -l3`

# Notes

VOM depends on libboost-all-dev
