#ifndef FILE_OPS_H__
#define FILE_OPS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utils.h"
#include "opt_extract.h"
#include "mem_ops.h"

char *read_lines (char * NameFile); 
void log_make (char *mode, char *addr_ip, char *data, size_t len_data);
void write_log (char *str);

#endif
