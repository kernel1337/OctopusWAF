#include "utils.h"
#include "validate.h"

void isnull_argv()
{
  // FIXME: Why 128 chars? Hostnames could be way bigger, don't they?

  if ( strnlen ( param.hostarg, 128 ) == 0 )
    die ( "Need host argument\n" );

  if ( strnlen ( param.redirectarg, 128 ) == 0 )
    die ( "Need redirect argument" );

  // FIX: I think this is clearer
  if ( param.option_algorithm < 1 || param.option_algorithm > 4 )
    die ( "Need log argument" );
}

bool is_request ( char *ptr )
{
  if ( ptr == NULL )
    return false;

  // FIXME: Isn't this unecessary?
  if ( ptr[0] == ' ' || ptr[1] == ' ' )
    return false;

  // FIXME: Why 12? Why 10?
  if ( strnlen ( ptr, 12 ) < 10 )
    return false;

  const char **p;
  static const char *http_methods[] = 
  { "GET ", "POST ", "PUT ", "DELETE ", NULL };
  // FIX: Must check the space after to avoid false matches.
  // FIXME: How about HEAD, CONNECT, OPTIONS, TRACE (from RFC 7231) and PATCH (from RFC 5789)?

  p = http_methods;
  while ( *p )
  {
    if ( memcmp( ptr, *p, strlen( *p ) ) == 0 )
      return true;
    p++;
  }

  return false;
}
