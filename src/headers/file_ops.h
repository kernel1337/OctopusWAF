#ifndef FILE_OPS_H__
#define FILE_OPS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "opt_extract.h"

void log_make (char *mode, char *addr_ip, char *data, size_t len_data);
void write_log (char *str);

#endif
