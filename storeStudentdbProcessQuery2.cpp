#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct student
{
	int no;
	char name[25], college[25];
};

void findnext(char *P, int *next, int p, int k)
{
	int i, j;
	j = -1;
	i = k;
	next[0] = -1;
	while (i<p)
	{
		while (((P[i] == P[j]) && (i<(p - 1))) || (j == -1))
		{
			i++;
			j++;
			next[i] = j;
		}
		j = next[j];
	}
}

int kmpAlgorithm(char *T, char *P, int k)
{
	int t, p, i, j, next[25];
	t = strlen(T);
	p = strlen(P);
	//printf("%d %d",t,p);
	findnext(P, next, p, k);
	i = k; j = 0;
	while (i <= (t - p))
	{
		while (((j<p) && (T[i] == P[j])) || (j == -1))
		{
			i++;
			j++;
		}
		if (j == p)
		{
			return i - p;
		}
		j = next[j];
	}
	return -1;
}

void getQueryAndEntiity(char *buffer, char *entity, char *value)
{
	int i = strlen(buffer)-2;
	if (buffer[i] != 34)
		return;
	i--;
	int j = -1;
	printf("%s%s%s", value, entity, buffer);
	while (buffer[i]!=34)
	{
		value[++j] = buffer[i];
		i--;
	}
	value[++j] = '\0';
	value = strrev(value);
	i--;
	i--;
	j = -1;
	while (buffer[i]!=32)
	{
		entity[++j] = buffer[i];
		i--;
	}
	entity[++j] = '\0';
	entity = strrev(entity);
}

int stringToNumber(char *string)
{
	int num = 0, i = 0;
	//printf("string is %s\n", string);
	while (string[i] != '\0')
	{
		num = (num * 10) + (string[i] - 48);
		i++;
	}
	return num;
}

int parseQuery(char *buffer)
{
	int i,returnValue;
	i = 0;
	while (buffer[i]!=32)
	{
		i++;
	}
	i++;
	if (buffer[i] == '*')
	{
		if (buffer[i + 1] == ';')
			returnValue = 0;
		else
		{
			returnValue = 1;
		}
	}
	else
	{
		int x = 0, y = 0, z = 0;
		int r = kmpAlgorithm(buffer, "no", 0);
		char c = buffer[r + 2];
		if ((c == 32 || c == 44 || c==59) && (r != -1))
			x = 1;
		r = kmpAlgorithm(buffer, "name", 0);
		c = buffer[r + 4];
		if ((c == 32 || c == 44 || c == 59) && (r != -1))
			y = 1;
		r = kmpAlgorithm(buffer, "college", 0);
		c = buffer[r + 7];
		if ((c == 32 || c == 44 || c == 59) && (r != -1))
			z = 1;
		//printf("x::%d y::%d z::%d\n", x, y, z);
		//if (x && y && z)
			//returnValue=1;
		if (x && y)
			returnValue = 5;
		else if (x && z)
			returnValue = 6;
		else if (y && z)
			returnValue = 7;
		else if (x)
			returnValue = 2;
		else if (y)
			returnValue = 3;
		else
			returnValue = 4;
	}
	//i++;
	return returnValue;
}
// only queries select *;(u don't have to give table name since there is only one table I considered)
//or select * where <condition>;
//or select <entities> where <condition>;
//entity names are written only from the structure (eg. no='23' or name='abc' or college='jntuv')
void executeQuery(char *query, FILE *f)
{
	int n = parseQuery(query);
	struct student s;
	long int size = 0;
	char *string, *entity;
	string = (char *)malloc(20 * sizeof(char));
	entity = (char *)malloc(10 * sizeof(char));
	memset(string, '\0', 20);
	memset(entity, '\0', 10);
//	string = NULL;
//	entity = NULL;

	if (n == 0)
	{
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		while (ftell(f) != size)
		{
			fread(&s, sizeof(student), 1, f);
			printf("%d %s %s %d\n", s.no, s.name, s.college, n);
		}
	}
	else if (n == 1)
	{
		int i, j=-1;	
		i = strlen(query)-1;
		while (query[i]!=34)
		{
			i--;
		}
		i--;
		while (query[i]!=34)
		{
			
			string[++j] = query[i];
			//printf("%c%c ", query[i],string[j]);
			i--;
		}
		
		string[++j] = '\0';
		//printf("\nstring is %s\n", string);
		string = strrev(string);
		while (query[i]!=61)
		{
			i--;
		}
		i--;
		j = -1;
		while (query[i]!=32)
		{
			entity[++j] = query[i];
			i--;
		}
		entity[++j] = '\0';
		entity = strrev(entity);
		printf("entity is %s\nvalue is %s\n", entity, string);
		if (strcmp(entity, "no") == 0)
		{
			int id = stringToNumber(string);
			printf("condition is id::%d\n",id);
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tNAME\t\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (s.no == id)
				{
					printf("%d\t%s\t\t%s\n", s.no, s.name, s.college);
					break;
				}
			}
			//printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "name") == 0)
		{
			int count = 0;
			printf("condition is name\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tNAME\t\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.name,string)==0)
				{
					printf("%d\t%s\t\t%s\n", s.no, s.name, s.college);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tNAME\t\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.college,string)==0)
				{
					printf("%d\t%s\t\t%s\n", s.no, s.name, s.college);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
	}
	else if (n == 2)
	{
		printf("query to print id\n");
		getQueryAndEntiity(query, entity, string);
		if (strcmp(entity, "no") == 0)
		{
			int id = stringToNumber(string);
			printf("condition is id::%d\n", id);
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (s.no == id)
				{
					printf("%d\n", s.no);
					break;
				}
			}
			//printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "name") == 0)
		{
			int count = 0;
			printf("condition is name\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.name, string) == 0)
				{
					printf("%d\n", s.no);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "college") == 0)
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.college, string) == 0)
				{
					printf("%d\n", s.no);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				printf("%d\n", s.no);
				count++;
			}
			printf("\n %d entries\n", count);
		}
	}
	else if (n == 3)
	{
		printf("query to print name\n");
		getQueryAndEntiity(query, entity, string);
		if (strcmp(entity, "no") == 0)
		{
			int id = stringToNumber(string);
			printf("condition is id::%d\n", id);
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("NAME\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (s.no == id)
				{
					printf("%s\n", s.name);
					break;
				}
			}
			//printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "name") == 0)
		{
			int count = 0;
			printf("condition is name\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("NAME\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.name, string) == 0)
				{
					printf("%s\n", s.name);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "college") == 0)
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("NAME\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.college, string) == 0)
				{
					printf("%s\n", s.name);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("NAME\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				printf("%s\n", s.name);
				count++;
			}
			printf("\n%d entries\n", count);
		}
	}
	else if (n == 4)
	{
		printf("query to print college\n");
		getQueryAndEntiity(query, entity, string);
		if (strcmp(entity, "no") == 0)
		{
			int id = stringToNumber(string);
			printf("condition is id::%d\n", id);
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("COLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (s.no == id)
				{
					printf("%s\n", s.college);
					break;
				}
			}
			//printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "name") == 0)
		{
			int count = 0;
			printf("condition is name\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("COLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.name, string) == 0)
				{
					printf("%s\n", s.college);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "college") == 0)
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("COLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.college, string) == 0)
				{
					printf("%s\n", s.college);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("COLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				printf("%s\n", s.college);
				count++;
			}
			printf("\n %d entries\n", count);
		}
	}
	else if (n == 5)
	{
		printf("query to print id,name\n");
		getQueryAndEntiity(query, entity, string);
		if (strcmp(entity, "no") == 0)
		{
			int id = stringToNumber(string);
			printf("condition is id::%d\n", id);
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tNAME\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (s.no == id)
				{
					printf("%d\t%s\n", s.no, s.name);
					break;
				}
			}
			//printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "name") == 0)
		{
			int count = 0;
			printf("condition is name\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tNAME\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.name, string) == 0)
				{
					printf("%d\t%s\n", s.no, s.name);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "college") == 0)
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tNAME\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.college, string) == 0)
				{
					printf("%d\t%s\n", s.no, s.name);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tNAME\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				printf("%d\t%s\n", s.no, s.name);
				count++;
			}
			printf("\n %d entries\n", count);
		}
	}
	else if (n == 6)
	{
		printf("query to print id,college\n");
		getQueryAndEntiity(query, entity, string);
		if (strcmp(entity, "no") == 0)
		{
			int id = stringToNumber(string);
			printf("condition is id::%d\n", id);
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (s.no == id)
				{
					printf("%d\t%s\n", s.no,s.college);
					break;
				}
			}
			//printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "name") == 0)
		{
			int count = 0;
			printf("condition is name\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.name, string) == 0)
				{
					printf("%d\t%s\n", s.no, s.college);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "college") == 0)
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.college, string) == 0)
				{
					printf("%d\t%s\n", s.no, s.college);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("ID\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				printf("%d\t%s\n", s.no, s.college);
				count++;
			}
			printf("\n %d entries\n", count);
		}
	}
	else if (n == 7)
	{
		printf("query to print name,college\n");
		getQueryAndEntiity(query, entity, string);
		if (strcmp(entity, "no") == 0)
		{
			int id = stringToNumber(string);
			printf("condition is id::%d\n", id);
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("NAME\t\t\tCOLLEGe\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (s.no == id)
				{
					printf("%s\t\t\t%s\n", s.name, s.college);
					break;
				}
			}
			//printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "name") == 0)
		{
			int count = 0;
			printf("condition is name\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("NAME\t\t\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.name, string) == 0)
				{
					printf("%s\t\t\t%s\n", s.name, s.college);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else if (strcmp(entity, "college") == 0)
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("NAME\t\t\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				if (strcmp(s.college, string) == 0)
				{
					printf("%s\t\t\t%s\n", s.name, s.college);
					count++;
				}
			}
			printf("\n %d entries\n", count);
		}
		else
		{
			int count = 0;
			printf("condition is college\n");
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			printf("NAME\t\t\tCOLLEGE\n");
			while (ftell(f) != size)
			{
				fread(&s, sizeof(student), 1, f);
				printf("%s\t\t\t%s\n", s.name, s.college);
				count++;
			}
			printf("\n %d entries\n", count);
		}
	}
}

int main(){
	FILE *fp;
	struct student s;
	int n = 0;
	fp = fopen("studentData.bin", "r");
	if (!fp)
	{
		perror("unable to open");
	}
	char query[50];
	printf("Enter the query::\n");
	gets(query);
	executeQuery(query, fp);
	getchar();
	return 0;
}