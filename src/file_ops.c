#include "file_ops.h"


//read lines of file
char 
*read_lines (char * NameFile)
{
	FILE * arq;

	arq = fopen(NameFile, "r");

	if( arq == NULL )
	{
		DEBUG("error in filename %s  to open() file:  %s",NameFile,strerror(errno));		
		exit(0);
	}

	fseek(arq, 0, SEEK_END);
    	long length = ftell(arq);
    	fseek(arq, 0, SEEK_SET);
    	char *buffer = xmalloc(sizeof(char)*(length + 1));
    	buffer[length] = '\0';
	
    	if (fread(buffer, sizeof(char), length, arq)<=0)
	{
		DEBUG("Config error Empty rule file %s",NameFile);
		exit(0);
	}
 
	if (fclose(arq) == EOF)
	{
		DEBUG("Error in close() file %s",NameFile);
		exit(0);
	}

	arq = NULL;

	return buffer;
}

void 
log_make (char *mode, char *addr_ip, char *data, size_t len_data)
{
	size_t len_log=len_data+strlen(addr_ip)+(25*sizeof(char)+strlen(mode));
	char *log_in=xmalloc(len_log);

	burn_mem(log_in,0, len_log-1); // preserve last byte of canary here, need or can cause crashes
	snprintf(log_in,len_log,"Mode: %s\nIP addr: %s\nData: %s\n", mode, addr_ip, data);
	write_log(log_in);
	free(log_in);
}


// write line in file
void 
write_log (char *str)
{
	FILE *arq;
 
	arq = fopen(param.logfile,"a"); 

	if (arq == NULL) 
	{
//		fclose(arq);
		DEBUG("error in WriteFile() %s",param.logfile);
	        exit(0);
	}

	fprintf(arq,"%s\n",str); 

	if (fclose(arq) == EOF)
	{
		DEBUG("error in Write() file %s",param.logfile);
		exit(0);
	}

	arq=NULL;
 

}



