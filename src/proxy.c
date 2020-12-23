#include "proxy.h"
#include "opt_extract.h"



struct event_base *base;
struct sockaddr_storage listen_on_addr;
struct sockaddr_storage connect_to_addr;
int connect_to_addrlen;
//static SSL_CTX *ssl_ctx = NULL;

char *
addr_2_str (struct sockaddr *res)
{
	char *s = NULL;

		switch(res->sa_family) 
		{
    			case AF_INET: 
			{
        			struct sockaddr_in *addr_in = (struct sockaddr_in *)res;
        			s = xmalloc(INET_ADDRSTRLEN);
        			inet_ntop(AF_INET, &(addr_in->sin_addr), s, INET_ADDRSTRLEN);
        			break;
			}
    			
    			case AF_INET6:
			{	
        			struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)res;
        			s = xmalloc(INET6_ADDRSTRLEN);
        			inet_ntop(AF_INET6, &(addr_in6->sin6_addr), s, INET6_ADDRSTRLEN);
        			break;
			}
    			
    			default:
        			break;
		}

	return s;
}

// block msg
void 
block_msg (struct bufferevent *bev)
{
// Here you can hide msg error to gain performance
      
	char *block_msg=
			"HTTP/1.1 404 Not Found\r\n"
			"Content-type: text/html\r\n"
			"\r\n"
			"<html>\r\n"
			" <body>\r\n"
			"  <h1>Blocked</h1>\r\n"
			"  <p>The requested URL was not found on this server.</p>\r\n"
			" </body>\r\n"
			"</html>\r\n";
	bufferevent_write(bev, block_msg, strlen(block_msg));
	bufferevent_setwatermark(bev, EV_WRITE, MAX_OUTPUT/2, MAX_OUTPUT);
	bufferevent_disable(bev, EV_READ);
}


// jack the ripper function 
// func to split request in chunks, util because a lot functions cannot match request buffer in full mode
bool 
split_and_check (char * input,  bool (*lambda)(char *argvs))
{
	bool test = false;
	char *delim = "\n";
	char *ptr = strtok(input, delim);

		while (ptr != NULL && test != true)
		{
			test = lambda(ptr);
			ptr = strtok(NULL, delim);
		}

	return test;

}

char *
bufferevent_get_addr(struct bufferevent *bev)
{
	struct sockaddr addr;
        int sock_in = bufferevent_getfd(bev);
    	socklen_t addr_size = sizeof(struct sockaddr);
    	int res = getpeername(sock_in, &addr, &addr_size);
	char *addr_ip = addr_2_str(&addr);

	return addr_ip;
}

// if return true, blocks...
bool 
filter_check (struct bufferevent *bev)
{
	struct evbuffer *input = bufferevent_get_input(bev); 
	size_t len = evbuffer_get_length(input);
	char *data = xmalloc(len);
	char *addr_ip = bufferevent_get_addr(bev);
	bool test = false;

	evbuffer_copyout(input, data, len);

	char *tmpbuf = urldecode(data,strnlen(data,6128));
	int len_tmp = strnlen(tmpbuf,6128);

		if (is_request(tmpbuf))
		{
			if (blocklist_ip(addr_ip) == true)
			{
				test = true;

				if (param.debug == true)
					printf("Block per address \nIP addr: %s\n input: %s\n", addr_ip, data);
					
				if (param.logfile)
					log_make("Block per address", addr_ip, " ", 1);
			}

			if (param.option_algorithm != 0)
			{
				if (matchlist(tmpbuf,len_tmp, param.option_algorithm) == true)
				{
					test = true;

					if (param.debug == true)
						printf("Algorithm match!\n IP addr: %s\n input: %s\n", addr_ip, data);
					
					if (param.logfile)
						log_make("Algorithm match", addr_ip, data, len);

				}
			}

			if (param.libinjection_sqli == true)
				if (split_and_check(tmpbuf,libinjection_test_sqli)==true)
				{
					test = true;

					if (param.debug == true)
						printf("%s Libinjection match\n%s IP addr: %s\n input: %s\n",GREEN,LAST, addr_ip, data);
	
					if (param.logfile)
						log_make("Libinejction SQLi match", addr_ip, data, len);
				}


			if (param.pcre == true)
			{	
				if (matchlist(tmpbuf,len_tmp, 4)==true)
				{
					test = true;

					if (param.debug == true)
						printf("%s PCRE match\n%s IP addr: %s\ninput: %s\n", YELLOW,LAST, addr_ip, data );
	
					if (param.logfile)
						log_make("Lib PCRE match per regex", addr_ip, data, len);
				}
			}
		}

	free(data);
	data = NULL;
	free(addr_ip);
	addr_ip = NULL;
//	free(tmpbuf);

	return test;
}


void 
readcb (struct bufferevent *bev, void *ctx)
{
	struct bufferevent *partner = ctx;
	struct evbuffer *src, *dst;
	size_t len;

	src = bufferevent_get_input(bev);
	len = evbuffer_get_length(src);

		if (!partner) 
		{
			evbuffer_drain(src, len);
			return;
		}

		if (filter_check(bev)==true)
		{
			block_msg(bev);

			if (param.debug==true)
			{
				printf("\n%s---===DETECT ATTACK HERE!===---\n%s",RED,LAST);
				printf("%s███████████████████████████████████\n%s",RED,LAST);
			}
		} else {

			dst = bufferevent_get_output(partner);
			evbuffer_add_buffer(dst, src);

			if (evbuffer_get_length(dst) >= MAX_OUTPUT) 
			{
				bufferevent_setcb(partner, readcb, drained_writecb, eventcb, bev);
				// pass to otherside
				bufferevent_setwatermark(partner, EV_WRITE, MAX_OUTPUT/2, MAX_OUTPUT);
				bufferevent_disable(bev, EV_READ);
			}
		}
}

// func from libevent examples
void 
drained_writecb (struct bufferevent *bev, void *ctx)
{
	struct bufferevent *partner = ctx;

	/* We were choking the other side until we drained our outbuf a bit.
	 * Now it seems drained. */
	bufferevent_setcb(bev, readcb, NULL, eventcb, partner);
	bufferevent_setwatermark(bev, EV_WRITE, 0, 0);

		if (partner)
			bufferevent_enable(partner, EV_READ);
}

void 
close_on_finished_writecb(struct bufferevent *bev, void *ctx)
{
	struct evbuffer *b = bufferevent_get_output(bev);

		if (evbuffer_get_length(b) == 0) 
			bufferevent_free(bev);
	
}

//func from libevent examples
void 
eventcb (struct bufferevent *bev, short what, void *ctx)
{
	struct bufferevent *partner = ctx;

	if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) 
	{
		if (what & BEV_EVENT_ERROR) 
		{
			unsigned long err;
			while ((err = (bufferevent_get_openssl_error(bev)))) 
			{
				const char *msg = (const char*)
				    ERR_reason_error_string(err);
				const char *lib = (const char*)
				    ERR_lib_error_string(err);
	 			const char *func = (const char*)
				    ERR_func_error_string(err);
				fprintf(stderr,
				    "%s in %s %s\n", msg, lib, func);
			}

			if (errno)
				perror("connection error");
		}

		if (partner) 
		{
			/* Flush all pending data */
			readcb(bev, ctx);

			if (evbuffer_get_length(bufferevent_get_output(partner)) ) 
			{
				/* We still have to flush data from the other
				 * side, but when that's done, close the other
				 * side. */
				bufferevent_setcb(partner,
				    NULL, close_on_finished_writecb,
				    eventcb, NULL);
				bufferevent_disable(partner, EV_READ);
			} else {
				/* We have nothing left to say to the other
				 * side; close it. */
				bufferevent_free(partner);
			}
		}
		bufferevent_free(bev);
	}
}

// following example of libevent
void 
accept_cb (struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *a, int slen, void *p)
{
	int use_wrapper = 1;
	struct bufferevent *b_out, *b_in;
	bool allow_addr = false;

	b_in = bufferevent_socket_new(base, fd,BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);

		if (!ssl_ctx || use_wrapper)
			b_out = bufferevent_socket_new(base, -1,BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
		else {
			SSL *ssl = SSL_new(ssl_ctx);
			b_out = bufferevent_openssl_socket_new(base, -1, ssl,BUFFEREVENT_SSL_CONNECTING,BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
		}

	assert(b_in && b_out && a);

		if (bufferevent_socket_connect(b_out,(struct sockaddr*)&connect_to_addr, connect_to_addrlen)<0) 
		{
			perror("bufferevent_socket_connect");
			bufferevent_free(b_out);
			bufferevent_free(b_in);
			return;
		}

		if (ssl_ctx && use_wrapper) 
		{
			struct bufferevent *b_ssl;
			SSL *ssl = SSL_new(ssl_ctx);
			b_ssl = bufferevent_openssl_filter_new(base,b_out, ssl, BUFFEREVENT_SSL_CONNECTING,BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
	
			if (!b_ssl) 
			{
				perror("Bufferevent_openssl_new");
				bufferevent_free(b_out);
				bufferevent_free(b_in);
				return;
			}
	
			b_out = b_ssl;
		}

	bufferevent_setcb(b_in, readcb, NULL, eventcb, b_out);
	bufferevent_setcb(b_out, readcb, NULL, eventcb, b_in);

	bufferevent_enable(b_in, EV_READ|EV_WRITE);
	bufferevent_enable(b_out, EV_READ|EV_WRITE);

}

