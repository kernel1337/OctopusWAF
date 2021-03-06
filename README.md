# Octopus WAF 

![Alt text](https://github.com/CoolerVoid/OctopusWAF/blob/main/doc/octopus_bannerv04.png)

OctopusWAF is an open-source Web application firewall entirely created in C language that uses libevent to make multiple connections.
<img align="right" width="240" height="220" src="https://raw.githubusercontent.com/CoolerVoid/OctopusWAF/main/doc/octopus.png">
The event-driven architecture is optimized for many parallel connections (keep-alive), vital for high-performance AJAX applications.
This tool is very light. You can deploy in any, please. This resource turns perfect for protecting specific endpoints that need custom protection.

Video demo
---
https://www.youtube.com/watch?v=qbnis-i7EqU
Test detection with libinjection.


First step
---

Install libpcre or libpcre-dev with apt. If you use RPM-based distro, search the name pcre-devel package, on BSD systems search in ports or brew(MacOS)...
You Need libevent-dev to run; on RPM distros libevent-devel, you need to install OpenSSL-dev and OpenSSL-devel.

Example in debian based:
```
$ sudo apt install libssl-dev libevent-dev libpcre3-dev make gcc
```


To compile and run OctopusWAF, follow these commands:
```
$ git clone https://github.com/CoolerVoid/OctopusWAF
$ cd OctopusWAF; make
# if you need to see options try the following
$ bin/OctopusWAF

The example tested on DVWA on a simple HTTP channel.

$ bin/OctopusWAF -h 127.0.0.1:7008 -r 127.0.0.1:80 --debug --libinjection-sqli --log results_log.txt

Note you can use pcre, horspool, and libinjection mode protections simultaneously.

Open your browser in http://127.0.0.1:7008, and you can test the block when you attack.
```
* Notes: Don't execute with "cd bin; ./OctopusWAF" use full path "bin/OctopusWAF" because binary needs load content in the config directory.
Use HTTP only for WAF usage. This version 0.1 runs TLS but doesn't have a resource to load cert and read TLS requests/responses. If you use TLS, the service can lose the WAF function and work a reverse proxy.

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
* Resource to load modsec rules https://github.com/SpiderLabs/owasp-modsecurity-crs/
* Resource to use NLP and try to classify payload to block using machine learning (KNN or naive Bayes)
* Insert new regulations to detect XSS
* Insert new rules to detect SQLi
* Insert new rules to see RCE
* Insert new rules to detect RFI/LFI
* Insert new rules to detect XXE
* Insert new rules to detect Anomalies.
* Channel for TLS
* Cert Load
* Issues.





Reference:
---

https://libevent.org/

https://owasp.org/www-community/Web_Application_Firewall


