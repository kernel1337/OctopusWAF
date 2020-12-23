#include "matchlist.h"
#include "opt_extract.h"

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


bool
matchlist (char *input,int input_len, short option_algorithm)
{
	bool at_list = false;
	int line_len = 0;
	char *delim = "\n";
	char *regex_list=strdup(param.regex_rules);
	char *match_list=strdup(param.match_rules);
	char *config = (option_algorithm==4)?regex_list:match_list;
	char *line = strtok(config, delim);

	while(line != NULL && at_list == false)
	{
		line_len = strnlen(line,2048);		

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

	free(match_list);
	free(regex_list);
	return at_list;
}


