#ifndef OPT_EXTRACT_H__
#define OPT_EXTRACT_H__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>
#include "mem_ops.h"
#include "validate.h"
#include "strsec.h"
#include "utils.h"

struct choice  {
	char *hostarg;
	char *redirectarg;
	char *logfile;
	char *block_addr_list;
	char *regex_rules;
	char *match_rules;
	bool debug;
	bool libinjection_sqli;
	bool libinjection_xss;
	bool pcre;
	short option_algorithm;
};

struct choice param;

void parser_opts (int argc, char **argv);
void init_banner_octopus (void);
void option_banner_octopus (void);


#endif
