#include "stdafx.h"
#include<stdio.h>

int search(int *matrix, int rows, int coloumns, int key)
{
	int i;
	i = coloumns - 1;
	while (i <= (rows * coloumns) - 1)
	{
		if (key == matrix[i])
			return 1;
		else if (key < matrix[i])
		{
			i--;
			if ((i + 1) % coloumns == 0)
				break;
		}
		else
			i += coloumns;
	}
	return 0;
}

int main()
{
	int a[20] = { 5, 11, 15, 19, 25, 7, 15, 20, 23, 32, 11, 17, 22, 34, 33, 25, 32, 33, 55, 99 };
	printf("Result is %d\n", search(a, 4, 5, 11));
	getchar();
	return 0;
}