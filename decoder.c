#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char* argv[])
{
	/*printf("In decoder. size of blocks is %ld\n",sizeof(argv[1]));
	for(int i =0;i<sizeof(argv[1]);i++)
	{
		printf("argv[%d] is %s\n",i,argv[i]);
	}*/
	//printf("Argv[1] is %s",argv[1]);
	decode(argv);
}

int decode(char** blocks)
{
	//printf("First block is %s\n",blocks[1]);
	//printf("Size of blocks is %ld\n",sizeof(blocks));
	int len = 0;
	while(blocks[len+2]!=NULL)
	{
		len++;
	}
	int numChars = len;
	printf("Num chars is %d\n",numChars);
	char* fdIn_One = blocks[1];
	//printf("fdIn[1] in decoder is %s\n",fdIn_One);
	char data[numChars+1];
	for(int i =0;i<=numChars;i++)
	{
		data[i]='\0';
	}
	printf("Size of data is %ld\n",sizeof(data));
	
	int k =0;
	char c;
	printf("Decoded string is \n");
	for(int i =2;i<=numChars+1;i++)
	{
		c= (int)strtol(blocks[i],NULL,2);
		data[k] = c;
	//	printf("%c",data[k]);
		k++;
	}
	data[numChars] = '\0';
	printf("checking what data is %s\n",data);
	char charCount[64];
	sprintf(charCount,"%d",numChars+1);
	printf("charCount of decoded string is %s\n",charCount);																	
	//pass -1 as fdIn_One when calling from producer. if(fd==-1) then write to data.done, else fork
	int fd;
	sscanf(fdIn_One,"%d",&fd);
	char dataArg[numChars+1];
	for(int i =0;i<=numChars;i++)
	{
		char x = data[i];
		printf("%c",x);
		dataArg[i]=x;
		//dataArg[i]=data[i];
		//memcpy(dataArg[i],data[i],1);
		//strncat(dataArg,(char)data[i],1);
		//printf("%c",data[i]);	
	}
	//memcpy(dataArg,data,strlen(data));
	//strncat(dataArg,'\0\0',1);
	dataArg[numChars]='\0';
	printf("What is dataArg saved %s\n", dataArg);
	if(fd==-1)
	{
		FILE* ptr;
		ptr = fopen("data.done","a");
		printf("Writing \n ------------------\n %s \n-----------------\n", data);
		fputs(data,ptr);
		fclose(ptr);
		return 1;
	}
	else
	{
		int pid;
		pid = fork();
		if(pid==0)
		{	
			printf("\n==================\n");
			
			printf("\n=================\n");
			printf("Calling to upper with %s\n",data);
			execl("toUpperService","toUpperService",data,charCount,fdIn_One,NULL);
		}
		else if (pid>0)
		{
			wait(NULL);
			//printf("In parent\n");
		}
		else
		{
			printf("Fork fail in decoder\n");
		}
	}
	return 0;
	// convert bit string to characters, then send to toUpperService
}
