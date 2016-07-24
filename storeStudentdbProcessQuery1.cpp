// sqlQuery.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>

struct student
{
	int no;
	char name[25],college[25];
};

int main(){
	FILE *f,*fp;
	struct student s;
	int count = 0;
	f = fopen("data.txt", "r");
	fp = fopen("studentData.bin", "w");
	if (!f || !fp)
	{
		perror("unable to open\n");
	}
	printf("opened successfull\n");
	fseek(f, 0, SEEK_SET);
	while (fscanf(f,"%d,%[^,],%[^\n]\n",&s.no,s.name,s.college)!=EOF)
	{
		printf("%d %s %s\n", s.no, s.name, s.college);
		fwrite(&s, sizeof(student), 1, fp);		
		count++;
		if (count % 20 == 0)
			getchar();
	}
	printf("count is %d", count);
	getchar();
	return 0;
}

