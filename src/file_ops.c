#include "file_ops.h"
#include "opt_extract.h"

//read lines of file
char 
*read_lines (char * NameFile)
{
	char *buffer = NULL;
	int fd = open(NameFile, O_RDONLY | O_EXCL );
	FILE *arq=NULL;

	if (-1 != fd) 
	{
		arq = fdopen(fd, "rx");

		if( arq == NULL )
		{
			DEBUG("error in filename %s  to open() file:  %s",NameFile,strerror(errno));		
			exit(0);
		}
	
		fseek(arq, 0, SEEK_END);
    		long length = ftell(arq);
    		fseek(arq, 0, SEEK_SET);
    		buffer = xmalloc(sizeof(char)*(length + 1));
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
	} else {
		if (close(fd) == -1) 
		{
			
			DEBUG("error in filename %s  to open() file:  %s",NameFile,strerror(errno));		
			exit(0);
		}
	}

	return buffer;
}

void 
log_make (char *mode, char *addr_ip, char *data, size_t len_data)
{
	size_t len_log=len_data+strlen(addr_ip)+(25*sizeof(char)+strlen(mode));
	char *log_in=xmalloc(len_log);

	burn_mem(log_in,0, len_log-1); // preserve last byte of canary here, need or can cause crashes, warning have a hardening in make file ;-) 
	snprintf(log_in,len_log,"Mode: %s\nIP addr: %s\nData: %s\n", mode, addr_ip, data);
	write_log(log_in);
	free(log_in);
}


// write line in file
void 
write_log (char *str)
{
	int fd = open(param.logfile, O_CREAT | O_WRONLY | O_APPEND, 0760 ); 
	FILE *arq=NULL;

	
	if (-1 != fd) 
	{

		arq = fdopen(fd, "ax");

		if (arq == NULL) 
		{
			DEBUG("error in filename %s  to open() file:  %s",param.logfile,strerror(errno));		
		        exit(0);
		}

		fprintf(arq,"%s\n",str); 

		if (fclose(arq) == EOF)
		{
			DEBUG("error in Write() file %s",param.logfile);
			exit(0);
		}
	} else {

		if (close(fd) == -1) 
		{
			
			DEBUG("error in filename %s  to open() file:  %s",param.logfile,strerror(errno));		
			exit(0);
		}
	}

	arq=NULL; 

}



