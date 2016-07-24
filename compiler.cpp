// compiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct symbolTable{
	int address, size;
	char name[4];
	struct symbolTable *next;
};
struct symbolTable *s;
struct intermediateCode{
	int no, opcode, parameters[4];
};

struct registers{
	int no;
	char name[2];
};

struct memoryStack{
	int stack[32];
};

struct blockAddress{
	char name[4];
	int address;
};

struct registers r[8];

int power(int value, int p)
{
	int i=1;
	while (p>0)
	{
		i = i * 10;
		p--;
	}
	return i;
}

int isRegister(char *str)
{
	int i = 0, key = -1;
	while (i<8)
	{
		if (strcmp(str, r[i].name) == 0)
		{
			key = i;
			break;
		}
		i++;
	}
	return key;
}

int isSymbol(char *str, struct symbolTable *s)
{
	struct symbolTable *temp;
	temp = s;
	char *p;
	int i=0,j=0;
	p = strstr(str, "[");
	if (p != NULL)
	{
		p = (char *)malloc(3 * sizeof(char));
		i = 0;
		j = -1;
		while (str[i] != '[')
		{
			p[++j] = str[i];
			i++;
		}
		p[++j] = '\0';
		i++;
		i = str[i] - 48;
		str=strcpy(str, p);
	}
	while (temp!=NULL)
	{
		if (strcmp(temp->name, str) == 0)
			return temp->address + i;
		temp = temp->next;
	}
	return -1;
}

int getOppCode(char *buffer)
{
	char *p;
	p = strstr(buffer, "MOV");
	if (p != NULL)
	{
		char *str;
		str = (char *)malloc(5 * sizeof(char));
		int i = strlen(buffer) - 1;
		int j = -1;
		while (buffer[i]!=44)
		{
			str[++j] = buffer[i];
			i--;
		}
		str[++j] = '\0';
		str = strrev(str);
		int isr = isRegister(str);
		if (isr>=0 && isr<15)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	p = strstr(buffer, "ADD");
	if (p != NULL)
		return 3;
	p = strstr(buffer, "SUB");
	if (p != NULL)
		return 4;
	p = strstr(buffer, "MUL");
	if (p != NULL)
		return 5;
	p = strstr(buffer, "JUMP");
	if (p != NULL)
		return 6;
	p = strstr(buffer, "IF");
	if (p != NULL)
		return 7;
	p = strstr(buffer, "EQ");
	if (p != NULL)
		return 8;
	p = strstr(buffer, "LT");
	if (p != NULL)
		return 9;
	p = strstr(buffer, "GT");
	if (p != NULL)
		return 10;
	p = strstr(buffer, "LTEQ");
	if (p != NULL)
		return 11;
	p = strstr(buffer, "GTEQ");
	if (p != NULL)
		return 12;
	p = strstr(buffer, "PRINT");
	if (p != NULL)
		return 13;
	p = strstr(buffer, "READ");
	if (p != NULL)
		return 14;
	return 0;
}

void convertASMcode(FILE *f, struct symbolTable *s)
{
	char buffer[512];
	int constIndex = 0;
	struct symbolTable *head, *temp;
	struct memoryStack m;
	for (int i = 0; i < 32;i++)
	{
		m.stack[i] = -1;
	}
	while (fscanf(f, "%[^\n]\n", buffer) != EOF)
	{
		char *poss;
		if (strcmp(buffer, "START:") == 0)
			break;
		poss=strstr(buffer, "DATA");
		head = (struct symbolTable *)malloc(sizeof(symbolTable));
		if (poss != NULL)
		{
			//printf("%s::", buffer);
			//printf("Not constant ");
			poss = strstr(buffer, "[");
			if (poss == NULL){
				head->size = 1;
				int i=strlen(buffer)-1;
				int j = -1;
				while (buffer[i]!=32)
				{
					head->name[++j] = buffer[i];
					i--;
				}
				head->name[++j] = '\0';
				//head->name = strrev(head->name);
			}
			else
			{
				head->size = 0;
				int i = strlen(buffer)-2, ten=0;
				while (buffer[i]!=91)
				{
					head->size += power(10, ten)*(buffer[i] - 48);
					i--;
				}
				i--;
				int j = -1;
				while (buffer[i] != 32)
				{
					head->name[++j] = buffer[i];
					i--;
				}
				head->name[++j] = '\0';
			}
		}
		else
		{
			//printf("%s::", buffer);
			//printf("Constant ");
			head->size = 0;
			int i = strlen(buffer) - 1;
			int value = 0;
			int ten = 0;
			while (buffer[i]>=48 && buffer[i]<=57)
			{
				value += (power(10, ten))*(buffer[i] - 48);
				i--;
			}
			//printf("constant value is %d\n", value);
			m.stack[constIndex] = value;
			while (buffer[i]!=61)
			{
				i--;
			}
			i--;
			int j = -1;
			while (buffer[i] != 32)
			{
				head->name[++j] = buffer[i];
				i--;
			}
			head->name[++j] = '\0';
		}
		//printf("%d %s\n", head->size, head->name);
		memset(buffer, '\0', 512);
		if (s != 0)
		{
			if (temp->size != 0)
				head->address = temp->size + temp->address;
			else
				head->address = 1 + temp->address;
			temp->next = head;
			temp = head;
			constIndex = temp->address + temp->size;
		}
		else
		{
			head->address = 0;
			s = temp = head;
		}
	}
	temp->next = 0;
	printf("Symbol Table is\nname\taddress\tsize\n");
	temp = s;
	while (temp != NULL)
	{
		printf("%s\t%d\t%d\n", temp->name, temp->address, temp->size);
		temp = temp->next;
	}
	printf("Memory stack\nindex\tvalue\n");
	int i = 0;
	while (i<32)
	{
		if (m.stack[i] != -1)
			printf("%d\t%d\n", i, m.stack[i]);
		i++;
	}
	printf("End of initializations\n");
	struct intermediateCode ic[25];
	int k = 0, count = 1;

	while (fscanf(f, "%[^\n]\n", buffer) != EOF)
	{
		printf("%s\n", buffer);
		int opcode = getOppCode(buffer);
		if (opcode > 0 && opcode < 15)
		{
			int i, isr, ism, x, isr1, isr2;
			ic[k].no = count;
			ic[k].opcode = opcode;
//			int param;
			char *reg, *mem, *r1, *r2;
			switch (opcode)
			{
			case 1:
				//char *reg, *mem;
				x = -1;
				reg = (char *)malloc(3 * sizeof(char));
				mem = (char *)malloc(3 * sizeof(char));
				i = strlen(buffer) - 1;
				while (buffer[i]!=44)
				{
					reg[++x] = buffer[i];
					i--;
				}
				reg[++x] = '\0';
				i--;
				x = -1;
				while (buffer[i]!=32)
				{
					mem[++x] = buffer[i];
					i--;
				}
				mem[++x] = '\0';
				reg = strrev(reg);
				mem = strrev(mem);
				isr = isRegister(reg);
				ism = isSymbol(mem,s);
				ic[k].no = count;
				count++;
				ic[k].opcode = opcode;
				ic[k].parameters[0] = ism;
				ic[k].parameters[1] = isr;
				printf("\n%d %d %d %d\n", ic[k].no, ic[k].opcode, ic[k].parameters[0], ic[k].parameters[1]);
				break;
			case 2:
				//char *reg, *mem;
				x = -1;
				reg = (char *)malloc(3 * sizeof(char));
				mem = (char *)malloc(3 * sizeof(char));
				i = strlen(buffer) - 1;
				while (buffer[i] != 44)
				{
					mem[++x] = buffer[i];
					i--;
				}
				mem[++x] = '\0';
				i--;
				x = -1;
				while (buffer[i] != 32)
				{
					reg[++x] = buffer[i];
					i--;
				}
				reg[++x] = '\0';
				reg = strrev(reg);
				mem = strrev(mem);
				isr = isRegister(reg);
				ism = isSymbol(mem,s);
				ic[k].no = count;
				count++;
				ic[k].opcode = opcode;
				ic[k].parameters[0] = isr;
				ic[k].parameters[1] = ism;
				printf("\n%d %d %d %d\n", ic[k].no, ic[k].opcode, ic[k].parameters[0], ic[k].parameters[1]);
				break;
			case 3:
			case 4:
			case 5:
				reg = (char *)malloc(3 * sizeof(char));
				r1 = (char *)malloc(3 * sizeof(char));
				r2 = (char *)malloc(3 * sizeof(char));
				i = 0;
				x = -1;
				while (buffer[i]!=32)
				{
					i++;
				}
				i++;
				while (buffer[i]!=44)
				{
					reg[++x] = buffer[i];
					i++;
				}
				reg[++x] = '\0';
				x = -1;
				i++;
				while (buffer[i] != 44)
				{
					r1[++x] = buffer[i];
					i++;
				}
				r1[++x] = '\0';
				x = -1;
				i++;
				while (buffer[i] != '\0')
				{
					r2[++x] = buffer[i];
					i++;
				}
				r2[++x] = '\0';
				//printf("%s#%s#%s\n", reg, r1, r2);
				isr = isRegister(reg);
				if (isr==-1)
				{
					isr = isSymbol(reg, s);
				}
				isr1 = isRegister(r1);
				if(isr1==-1)
				{
					isr1 = isSymbol(r1, s);
				}
				isr2 = isRegister(r2);
				if (isr2==-1)
				{
					isr2 = isSymbol(r2, s);
				}
				ic[k].opcode = opcode;
				ic[k].no = count;
				ic[k].parameters[0] = isr;
				ic[k].parameters[1] = isr1;
				ic[k].parameters[2] = isr2;
				printf("\n%d %d %d %d %d\n", ic[k].no, ic[k].opcode, ic[k].parameters[0], ic[k].parameters[1], ic[k].parameters[2]);
				count++;
				break;
			case 7:
				i = 0;
				r1 = (char *)malloc(3 * sizeof(char));
				r2 = (char *)malloc(3 * sizeof(char));
				while (buffer[i] != 32){
					i++;
				}
				i++;
				x = -1;
				while (buffer[i]!=32)
				{
					r1[++x] = buffer[i];
					i++;
				}
				r1[++x] = '\0';
				i++;
				x = -1;
				while (buffer[i] != 32)
				{
					reg[++x] = buffer[i];
					i++;
				}
				reg[++x] = '\0';
				i++;
				x = -1;
				while (buffer[i] != 32)
				{
					r2[++x] = buffer[i];
					i++;
				}
				r2[++x] = '\0';
				isr1 = isRegister(r1);
				if (isr1 == -1)
				{
					isr1 = isSymbol(r1, s);
				}
				isr = getOppCode(reg);
				isr2 = isRegister(r2);
				if (isr2 == -1)
				{
					isr2 = isSymbol(r2, s);
				}
				ic[k].no = count++;
				ic[k].opcode = opcode;
				ic[k].parameters[0] = isr1;
				ic[k].parameters[1] = isr;
				ic[k].parameters[2] = isr2;
				printf("\n%d %d %d %d %d\n", ic[k].no, ic[k].opcode, ic[k].parameters[0], ic[k].parameters[1], ic[k].parameters[2]);
				//printf()
				break;
			case 13:
				i = strlen(buffer) - 1;
				reg = (char *)malloc(3 * sizeof(char));
				x = -1;
				while (buffer[i]!=32)
				{
					reg[++x] = buffer[i];
					i--;
				}
				reg[++x] = '\0';
				reg = strrev(reg);
				isr = isRegister(reg);
				if (isr == -1)
				{
					isr = isSymbol(reg, s);
				}
				ic[k].no = count;
				count++;
				ic[k].opcode = opcode;
				ic[k].parameters[0] = isr;
				break;
			case 14:
//				char *reg;
				reg = (char *)malloc(3 * sizeof(char));
				i = strlen(buffer) - 1;
				reg[0] = buffer[i-1];
				reg[1] = buffer[i];
				reg[2] = '\0';
				//reg = strrev(reg);
				//reg[2] = '\0';
				printf("reg is %s\n", reg);
				isr = isRegister(reg);
				ic[k].no = count;
				count++;
				ic[k].opcode = opcode;
				ic[k].parameters[0] = isr;
				printf("\n%d %d %d\n", ic[k].no, ic[k].opcode, ic[k].parameters[0]);
				break;
			default:
				break;
			}
		}
		printf("oppcode::%d\n", opcode);
		memset(buffer, '\0', 512);
		k++;
	}
}

int main(){
	FILE *f;
	char buffer[512];
	f = fopen("asmcode.txt", "r");
	if (!f)
	{
		perror("error opening file");
	}
	
	strcpy(r[0].name, "AX");
	r[0].no = 0;
	strcpy(r[1].name, "BX");
	r[1].no = 1;
	strcpy(r[2].name, "CX");
	r[2].no = 2;
	strcpy(r[3].name, "DX");
	r[3].no = 3;
	strcpy(r[4].name, "EX");
	r[4].no = 4;
	strcpy(r[5].name, "FX");
	r[5].no = 5;
	strcpy(r[6].name, "GX");
	r[6].no = 6;
	strcpy(r[7].name, "HX");
	r[7].no = 7;
	int i;
	printf("Register codes\nname\tnumber\n");
	for (i = 0; i < 8; i++)
	{
		printf("%s\t%d\n", r[i].name, r[i].no);
	}
	//struct symbolTable *s;
	s = 0;
	convertASMcode(f,s);
	
	fclose(f);
	getchar();
	return 0;
}