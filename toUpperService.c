#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "encDec.h"

int main(int argc, char* argv[])
{
	//printf("In toUpper\n");
	toUpper(argv[1],argv[2],argv[3]);
}

int toUpper(char* str, char* len,char* fdIn_One)
{
	int length;
	sscanf(len,"%d",&length);
	char upperString[length-1];
	for(int i =0; i<length;i++)
	{
		if((int)str[i]>='a' && (int)str[i]<='z')
		{
			upperString[i]=toupper(str[i]);
		}
		else
		{
			upperString[i]=str[i];
		}
	}
	str=NULL;
	FILE *fp;
	fp = fopen("data.outf","a");
	fputs(upperString,fp);
	fclose(fp);
	length = length-1;
	char lenArg[64];
	sprintf(lenArg,"%d",length);
	int pid;
	pid=fork();
	if(pid==0)
	{
		execl("encoderService","encoderService",upperString,lenArg,fdIn_One,NULL);			//include fd for writing to pipe
	}
	else if(pid>0)
	{
		wait(NULL);
	}
	else
	{
		printf("Fork in loop failed \n");
	}
	return 0;
}
