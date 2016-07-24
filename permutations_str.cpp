//#include "stdafx.h"
#include <stdio.h>
#include <string.h>

void swap(char *x, char *y)
{
	char temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void permutationOfString(char *str, int start, int end)
{
	int i;
	if (start == end)
		printf("%s\n", str);
	else
	{
		for (i = start; i <= end; i++)
		{
			swap((str + start), (str + i));
			permutationOfString(str, start + 1, end);
			swap((str + start), (str + i));
		}
	}
}

int main()
{
	char str[] = "ABCD";
	int n = strlen(str);
	permutationOfString(str, 0, n - 1);
	getchar();
	return 0;
}
