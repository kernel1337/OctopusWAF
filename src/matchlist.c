#include "matchlist.h"

/* read lines of file matchlist.txt and test if match strings

If match context with request return array of void pointer with:
if true returns matched string

*/

void 
load_all_rules()
{
	param.regex_rules = read_lines("config/regex_rules.txt");
	param.match_rules = read_lines("config/match_list_request.txt");
	param.block_addr_list = read_lines("config/blocklist_ip.txt");

}


char *
matchlist (char *input,int input_len, short option_algorithm)
{
	bool at_list = false;
	int line_len = 0;
	char *line = NULL, *delim = "\n";

	if (option_algorithm == 4)
		line = strtok(param.regex_rules, delim);
	else
		line = strtok(param.match_rules, delim);


	while(line != NULL)
	{
		chomp(line);
		line_len = strnlen(line,2048);			

// remove \n\0 etc... sub -2 at line_len
		if (line_len>4)		
			switch (option_algorithm)
			{
				case 1:
				at_list = DFA_Search(line,line_len-2, input, input_len);
				break;

				case 2:
				at_list = horspool_search(input,input_len, line, line_len-2);
				break;
			
				case 3:
				at_list = Rabin_Karp_search(input, input_len, line, line_len-2);
				break;
 	
				case 4:
				at_list = pcre_regex_search(input, input_len, line);
				break; 
			}

		line = strtok(NULL, delim);
	}


	if (at_list==true) 
	{
		char *tmp = xstrndup(line, line_len);
		return tmp;
	} else
		return NULL;
}


