#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char* argv[])
{
	decode(argv);
}

int decode(char** blocks)
{
	int len = 0;
	while(blocks[len+2]!=NULL)
	{
		len++;
	}
	int numChars = len;
	char* fdIn_One = blocks[1];
	char* data = malloc(numChars+1);													//allocation might be wrong. Use malloc
	for(int i =0;i<=numChars;i++)
	{
		data[i]='\0';
	}
	int k =0;
	char c;
	for(int i =2;i<=numChars+1;i++)
	{
		c= (int)strtol(blocks[i],NULL,2);
		data[k] = c;
		k++;
	}
	data[numChars] = '\0';
	char charCount[64];
	sprintf(charCount,"%d",numChars+1);
	int fd;
	sscanf(fdIn_One,"%d",&fd);
	if(fd==-1)
	{
		FILE* ptr;
		ptr = fopen("data.done","a");
		fputs(data,ptr);
		fclose(ptr);
		free(data);
		data=NULL;
		return 1;
	}
	else
	{
		int pid;
		pid = fork();
		if(pid==0)
		{	
			execl("toUpperService","toUpperService",data,charCount,fdIn_One,NULL);
		}
		else if (pid>0)
		{
			wait(NULL);
		}
		else
		{
			printf("Fork fail in decoder\n");
		}
	}
	return 0;
}
