export MALLOC_MMAP_THRESHOLD_=1
export MALLOC_CHECK_=1
export MALLOC_PERTURB_=1
CC=gcc
# -mmitigate-rop is obsolete.
HARDENING=-fstack-protector-all -pie -fPIE -ftrapv
CFLAGS=-W -Wall -O2 -Wno-unused-variable -Wno-unused-parameter $(HARDENING)
LIBS=-levent_openssl -lssl -lcrypto -levent -lpcre 
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
	$(CC) $(CFLAGS) -c $(DIR)*.c -I$(DIR_HEADERS) 
	$(CC) -o $(DIROUT)OctopusWAF *.o $(LDFLAGS) $(LIBS)  
	@echo "  "
	@echo " Execute \"bin/OctopusWAF\" to start...  "   

.PHONY: clean

clean:
	rm -f *.o $(DIROUT)OctopusWAF
