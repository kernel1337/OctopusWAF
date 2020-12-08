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

	char *lineBuffer = xcalloc(1,1);
	char *line = NULL;
	size_t len = 0, tmp_len = 0;

	while ( getline(&line, &len, arq) != -1  )  
	{
			
		tmp_len += strnlen(line,2048);
		lineBuffer = xreallocarray(lineBuffer, tmp_len, sizeof(char));
		strlcat(lineBuffer,line, tmp_len);
	}

 
	if( fclose(arq) == EOF )
	{
		DEBUG("Error in close() file %s",NameFile);
		exit(0);
	}

	arq = NULL;


	return lineBuffer;
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



