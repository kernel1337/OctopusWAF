#include "blocklist.h"
#include "mem_ops.h"
#include "utils.h"
#include "strsec.h"

//read lines of file
bool 
blocklist_ip (char * addr)
{
	bool at_list = false;
	char *line = NULL, *delim = "\n";

	line = strtok(param.block_addr_list, delim);

	while (line != NULL && at_list==false)
	{   
	        chomp(line);	
		if (strnstr(line,addr,128))
			at_list = true;

		line = strtok(NULL, delim);
	}


	return at_list;
}


