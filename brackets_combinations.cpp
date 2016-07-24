#include "stdafx.h"
#include<stdio.h>
#define MAX_SIZE 100

void print(int pos, int n, int open, int close)
{
	static char str[MAX_SIZE];
	if (close == n)
	{
		printf("%s \n", str);
		return;
	}
	else
	{
		if (open > close) {
			str[pos] = '}';
			print(pos + 1, n, open, close + 1);
		}
		if (open < n) {
			str[pos] = '{';
			print(pos + 1, n, open + 1, close);
		}
	}
}

void printParenthesis(int n)
{
	if (n > 0)
		print(0, n, 0, 0);
	return;
}

int main()
{
	int n = 3;
	printParenthesis(n);
	getchar();
	return 0;
}