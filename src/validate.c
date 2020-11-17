#include "utils.h"
#include "validate.h"

void 
isnull_argv (void)
{

	if (strnlen( param.hostarg, 256 ) == 0)
		die("Need host argument\n");

	if (strnlen( param.redirectarg, 256 ) == 0)
		die("Need redirect argument");

	if (param.option_algorithm < 1 || param.option_algorithm > 4)
		die("Need log argument");
}



bool 
is_request (char *ptr)
{
	if (ptr == NULL)
		return false;

	if (ptr[0] == ' ')
		return false;

	if (ptr[1] == ' ')
		return false;

	if (strnlen(ptr,12) < 10)
		return false;

	assert(ptr);
// is GET ?
 	if (memcmp(ptr, "GET",3) == 0)
		return true;

// is POST ?
 	if (memcmp(ptr, "POST",4) == 0)
		return true;

// is PUT ?
 	if (memcmp(ptr, "PUT",3) == 0)
		return true;
// is DELETE ?
 	if (memcmp(ptr, "DELETE",6) == 0)
		return true;

	return false;
}
