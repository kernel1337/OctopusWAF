# Octopus WAF 

OctopusWAF is a open source Web application firewall, is made in C language uses libevent to make multiple connections.

![Alt text](https://github.com/CoolerVoid/OctopusWAF/blob/main/doc/octopuswaf2.png)

First step
---

Instal lib-pcre, if you use RPM based distros search pcre-devel package, in BSD based search in ports or brew...
Need libevent-dev, on RPM distros libevent-devel, Openssl-dev and openssl-devel.

To compile and run OctopusWAF follow this commands:
```
$ git clone https://github.com/CoolerVoid/OctopusWAF
$ cd OctopusWAF; make
$ bin/OctopusWAF
Example tested on DVWA
$ bin/OctopusWAF -h 127.0.0.1:2006 -r 127.0.0.1:80 -m horspool --debug
Open your browser in http://127.0.0.1:2006

```
Don't execute with "cd bin; ./OctopusWAF" use full path "bin/OctopusWAF", because binary need load content in config indirectory.

Tested on Linux but can run in FreeBSD.


Code overview
---
```
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                               12            324            138            997
C/C++ Header                    11             63             70            212
make                             1              1              0             30
Markdown                         1              6              0              3
-------------------------------------------------------------------------------
SUM:                            25            394            208           1242
-------------------------------------------------------------------------------

```

TODO:
---
Resource to load modsec rules https://github.com/SpiderLabs/owasp-modsecurity-crs/

Insert rules to detect XSS

Insert rules to detect SQLi

Insert rules to detect RCE

Insert rules to detect RFI/LFI

Insert rules to detect XXE

Insert rules to detect Anomalys...

Channel for TLS

Cert Load





Reference:
---

https://libevent.org/

https://owasp.org/www-community/Web_Application_Firewall


