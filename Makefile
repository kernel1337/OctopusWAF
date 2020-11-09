export MALLOC_MMAP_THRESHOLD_=1
export MALLOC_CHECK_=1
export MALLOC_PERTURB_=1
CC=gcc
CFLAGS=-W -Wall -O2 -Wno-unused-variable -Wno-unused-parameter
HARDENING= -mmitigate-rop -fstack-protector-all -pie -fPIE -ftrapv
DFLAGS= -levent_openssl -levent_pthreads -lpthread -lssl -lcrypto -levent -lpcre 
DIR=src/
DIR_HEADERS=src/headers/
DIROUT=bin/
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS=-Wl
else
	LDFLAGS=-Wl,-z,relro,-z,now 
endif

OctopusWAF: $(DIR)OctopusWAF.c 
	@echo "           boing         boing         boing           "   
	@echo " e-e           . - .         . - .         . - .        "  
	@echo "(\_/)\       '       \`.   ,'       \`.   ,'       .      "  
	@echo " \`-'\ \`--.___,         . .           . .          .     "  
	@echo "    '\( ,_.-'                                           "  
	@echo "       \\               \"             \"          Compile !"    
	@echo "       ^\' "
	$(CC) $(CFLAGS) $(DFLAGS) -c $(DIR)*.c -I$(DIR_HEADERS) -Ilib
	$(CC) -o $(DIROUT)OctopusWAF *.o $(LDFLAGS) $(HARDENING) $(DFLAGS)  
	@echo "  "
	@echo " Execute \"bin/OctopusWAF\" to start...  "   
clean:
	rm -f *.o OctopusWAF
