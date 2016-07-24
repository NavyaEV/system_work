// jsonDeserialiser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct data{
	char entity[25];
	char value[30];
	struct data *array;
	struct data *next;
};

void parseJSON(char *buffer, struct data *d)
{
	int i = 0, j = -1;
	while (buffer[i]!=58)
	{
		if (buffer[i] == 34 || buffer[i] == 32 || buffer[i] == 123 || buffer[i] == 125 || buffer[i] == 91 || buffer[i] == 93){
			i++;
			continue;
		}
		d->entity[++j] = buffer[i];
		i++;
	}
	d->entity[++j] = '\0';
	i++;
	j = -1;
	while (buffer[i]!='\0')
	{
		if (buffer[i] == 34 || buffer[i] == 32 || buffer[i] == 123 || buffer[i] == 125 || buffer[i] == 91 || buffer[i] == 93){
			i++;
			continue;
		}
		d->value[++j] = buffer[i];
		i++;
	}
	d->value[++j] = '\0';
}

void printData(struct data *head)
{
	struct data *temp;
	temp = head;
	while (temp!=NULL)
	{
		printf("entity is %s\ndata is %s\n\n", temp->entity, temp->value);
		if (temp->array != 0)
			printData(temp->array);
		temp = temp->next;
	}
}

struct data * convertJSON(FILE *fp)
{
	int openParanthesis = 0;
	char c, buffer[1024];
	struct data d;
	struct data *start, *head, *temp = 0;
	//start = head = temp = 0;
	start = 0;
	int j = -1, key = 0;
	c = fgetc(fp);
	while (c != EOF)
	{
		if (c == 34)
		{
			head = (struct data *)malloc(sizeof(data));
			//c = fgetc(fp);
				while (1)
				{
					if (c == 44)
						break;
					else if (c == 123)
					{
						key = 1;
						head->array=convertJSON(fp);
					}
					if (key != 1){
						head->array = 0;
						key = 0;
					}
					buffer[++j] = c;
					if((c = fgetc(fp))==EOF)
						break;
				}
				buffer[++j] = '\0';
				//printf("buffer is %s\n", buffer);
				parseJSON(buffer, &d);
				//printf("entity is %s\ndata is %s\n\n", d.entity, d.value);
				
				strcpy(head->entity, d.entity);
				strcpy(head->value, d.value);
				//printf("entity is %s\ndata is %s\n\n", head->entity, head->value);
				if (start != 0)
				{
					temp->next = head;
					temp = head;
				}
				else
				{
					start = temp = head;
				}
				fflush(stdin);
				memset(buffer, '\0', 1024);
				j = -1;
		}
		c = fgetc(fp);
	}
	temp->next = 0;
	//printData(start);
	return start;
}

int main()
{
	FILE *fp;
	struct data *head;
	fp = fopen("jsonData.txt", "r");
	if (!fp)
	{
		perror("Unable to open file");
	}
	head=convertJSON(fp);
	printData(head);
	getchar();
	return 0;
}
