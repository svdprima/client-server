#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "general.h"

int isdig (char c)
{
	return (c >= '0' && c <= '9');
}

int get_number (char* inp_str, const unsigned int buff_size, unsigned int* offset)
{
	while (((*offset) < buff_size) && ((inp_str[*offset] == ' ') ||
	        (inp_str[*offset] == '\n') || (inp_str[*offset] == '\0')))
			(*offset)++;
	int sign = 1;
	if (inp_str[*offset] == '-')
	{
		sign = -1;
		(*offset)++;
	}
	int number = 0;
	while (((*offset) <= buff_size) && isdig(inp_str[*offset]))
	{
		number = number * 10 + (inp_str[*offset] - '0');
		(*offset)++;
	}
	return sign * number;
}
