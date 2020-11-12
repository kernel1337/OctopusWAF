#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#include "utils.h"
#include "mem_ops.h"

void die ( char *msg )
{
  puts ( msg );
  exit ( 0 ); // FIXME: Should be EXIT_FAILURE, isn't it?
}

void No_Pause_Waf()
{
  DEBUG ( "\n"
          " When start Waf\n"
          " You Cannot be terminated using Ctrl+C or Ctrl+Z...\n"
          " Wait task ends... press <enter> to continue... \n" );

  getchar();

  // FIXME: Unecessary to flush stdout. Final '\n' will do.
  fflush ( stdout );
}

void no_write_coredump ( void )
{
  struct rlimit rlim;

  rlim.rlim_cur = 0;
  rlim.rlim_max = 0;
  setrlimit ( RLIMIT_CORE, &rlim );

}

void load_signal_alarm ( void )
{
  static const int signals[] = { SIGINT, SIGTSTP, SIGQUIT, 0 };
  const int *p;

  struct sigaction sigIntHandler =
  { .sa_handler = ( void (*)(int) )No_Pause_Waf };

  if ( sigemptyset ( &sigIntHandler.sa_mask ) )
  {
    DEBUG ( "Error at signal" );
    exit ( 1 );   // FIXME: Should be EXIT_FAILURE, isn't it?
  }

  p = signals;
  while ( *p )
  {
    if ( sigaction ( *p, &sigIntHandler, NULL ) )
    {
      DEBUG ( "Error at signal" );
      exit ( 1 );   // FIXME: Should be EXIT_FAILURE, isn't it?
    }

    p++;
  }
}

char from_hex ( char ch )
{
  return isdigit ( ch ) ? ch - '0' : tolower ( ch ) - 'a' + 10;
}

char *urldecode ( char *str, int size )
{
  char *pstr = str, *buf = xmalloc ( size + 1 ), *pbuf = buf;

  while ( *pstr )
  {
    if ( *pstr == '%' )
    {
      if ( pstr[1] && pstr[2] )
      {
        *pbuf++ = from_hex ( pstr[1] ) << 4 | from_hex ( pstr[2] );
        pstr += 2;
      }
    }
    else if ( *pstr == '+' )
      *pbuf++ = ' ';

    else
      *pbuf++ = *pstr;

    pstr++;
  }

  *pbuf = '\0';

  return buf;
}


// remove all blank space ' ' from string
char *deadspace ( char *str )
{
  char *out = str, *put = str;

  for ( ; *str ; ++str )
    if ( *str != ' ' )
      *put++ = *str;

  *put = 0;

  return out;
}


// DFA to test if letter between a-z or A-Z
int test_letter ( char p )
{
  switch ( p )
  {
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
      return 0;
  }

  return 1;
}

// random case return string, input= tomato output=ToMatO or tOmATo...
// FIXME: It doen't do what it's said... :(
char *all2lowcase ( char *str )
{
  assert( str );

  // FIX: Easier if substitute this with strdup().
  char *str_new = xstrdup ( str );
  char *p;

  p = str_new;
  while ( *p )
  {
    if ( ! test_letter ( *p ) )
      *p ^= 0x20;    // FIX: This will work as intended

    p++;
  }

  return str_new;
}
