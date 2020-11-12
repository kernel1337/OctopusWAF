#include "utils.h"
#include "validate.h"

void isnull_argv()
{

  if ( strnlen ( param.hostarg, 256 ) == 0 )
    die ( "Need host argument\n" );

  if ( strnlen ( param.redirectarg, 256 ) == 0 )
    die ( "Need redirect argument" );

  // FIX: I think this is clearer
  if ( param.option_algorithm < 1 || param.option_algorithm > 4 )
    die ( "Need log argument" );
}



bool is_request(char *ptr)
{
	if(ptr == NULL)
		return false;

	if(ptr[0]==' ')
		return false;

	if(ptr[1]==' ')
		return false;

	if(strnlen(ptr,12) < 10)
		return false;

// is GET ?
 	if(ptr[0]=='G' && ptr[1]=='E' && ptr[2]=='T')
		return true;

// is POST ?
 	if(ptr[0]=='P' && ptr[1]=='O' && ptr[2]=='S' && ptr[3]=='T')
		return true;

// is PUT ?
 	if(ptr[0]=='P' && ptr[1]=='U' && ptr[2]=='T')
		return true;
// is DELETE ?
 	if(ptr[0]=='D' && ptr[1]=='E' && ptr[2]=='L' && ptr[3]=='E' && ptr[4]=='T' && ptr[5]=='E')
		return true;

 return false;
}
