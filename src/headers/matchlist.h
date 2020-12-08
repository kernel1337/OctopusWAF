#ifndef MATCHLIST_H__
#define MATCHLIST_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "file_ops.h"
#include "mem_ops.h"
#include "opt_extract.h"
#include "utils.h"
#include "strsec.h"
#include "match_algorithms.h"

void load_all_rules();
char *matchlist (char * input,int input_len,short option_algorithm);

#endif
