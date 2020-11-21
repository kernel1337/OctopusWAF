# Octopus WAF 

OctopusWAF is a open source Web application firewall, is made in C language uses libevent to make multiple connections.


![Alt text](https://github.com/CoolerVoid/OctopusWAF/blob/main/doc/octopus_bannerv04.png)

Video demo
---
https://www.youtube.com/watch?v=qbnis-i7EqU
Test detection with libinjection...


First step
---

Install lib-pcre, if you use RPM based distro search the name pcre-devel package, on BSD based search in ports or brew...
you Need libevent-dev to run, on RPM distros libevent-devel, you need to install Openssl-dev and openssl-devel.

To compile and run OctopusWAF, follow this commands:
```
$ git clone https://github.com/CoolerVoid/OctopusWAF
$ cd OctopusWAF; make
# if you need see options try this following
$ bin/OctopusWAF

Example tested on DVWA on simple HTTP channel

$ bin/OctopusWAF -h 127.0.0.1:7008 -r 127.0.0.1:80 --debug --libinjection-sqli --log results_log.txt

Note you can use pcre, horspool and libinjection mode protections in same time...

Open your browser in http://127.0.0.1:7008 and you can test the block when you attack...
```
* Notes: Don't execute with "cd bin; ./OctopusWAF" use full path "bin/OctopusWAF", because binary need load content in config directory.
Use HTTP only for WAF usage, this version 0.1 run TLS but don't have resource to load cert and read TLS requests/responses, if you use TLS the service can lost WAF function and work like reverse proxy.

Tested on Linux but can run in FreeBSD.


Code overview
---
```
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C/C++ Header                    14            133            270           9977
C                               13            591            798           2625
make                             2             14              3             52
Markdown                         1             34              0             52
-------------------------------------------------------------------------------
SUM:                            30            772           1071          12706
-------------------------------------------------------------------------------

```

Future TODO:
---
Resource to load modsec rules https://github.com/SpiderLabs/owasp-modsecurity-crs/

Insert new rules to detect XSS
Insert new rules to detect SQLi
Insert new rules to detect RCE
Insert new rules to detect RFI/LFI
Insert new rules to detect XXE
Insert new rules to detect Anomalys...
Channel for TLS
Cert Load





Reference:
---

https://libevent.org/

https://owasp.org/www-community/Web_Application_Firewall


