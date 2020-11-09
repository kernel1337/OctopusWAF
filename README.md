# Octopus WAF 

OctopusWAF is a open source Web application firewall, is made in C language uses libevent to make multiple connections.

![Alt text](https://github.com/CoolerVoid/OctopusWAF/blob/main/doc/octopuswaf2.png)

First step
---

Instal lib-pcre or in RPM based distros search pcre-devel, in BSD search in ports...

To run OctopusWAF follow this commands:
```
$ git clone https://github.com/CoolerVoid/OctopusWAF
$ cd OctopusWAF; make
$ bin/OctopusWAF
```
Don't execute with "cd bin; ./OctopusWAF" use full path "bin/OctopusWAF", because binary need load content in config indirectory.



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





Reference:
---

https://libevent.org/

https://owasp.org/www-community/Web_Application_Firewall


