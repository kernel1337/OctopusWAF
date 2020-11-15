#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>
#include <sys/resource.h>

#include <signal.h>
#include "mem_ops.h"
#include "validate.h"
#include "opt_extract.h"
#include "server.h"

// todo use this in future for TLS
const char *ssl_certchain = "cert/certkey.pem";
const char *ssl_privatekey = "cert/privateKey.key";


int 
main(int argc, char **argv)
{
	no_write_coredump(); 
	load_signal_alarm();

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) 
	{
		perror("signal()");
		return 1;
	}


	parser_opts(argc,argv);

	init_banner_octopus();		
	start_octopus_server();

	exit(0);
}
