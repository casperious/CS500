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
	int numChars = sizeof(blocks) -2 -2;						// shell and session manager are the last 2 elements of argv. Remove argv[0] and null so 4 elements removed
	printf("Num chars is %d\n",numChars);
	char* fdIn_One = blocks[1];
	printf("fdIn[1] in decoder is %s\n",fdIn_One);
	char data[numChars+1];
	printf("Size of data is %ld\n",sizeof(data));
	
	int k =0;
	char c;
	for(int i =2;i<=numChars+1;i++)
	{
		c= (int)strtol(blocks[i],NULL,2);
		data[k] = c;
		printf("%c ",data[k]);
		k++;
	}
	printf("\n");
	data[numChars] = '\0';
	for(int i =0;i<=numChars;i++)
	{
		printf("%c ",data[i]);
	}
	printf("\n");
	char charCount[64];
	sprintf(charCount,"%d",numChars+1);
	printf("charCount is %s\n",charCount);																			//pass -1 as fdIn_One when calling from producer. if(fd==-1) then write to data.done, else fork
	int pid;
	pid = fork();
	if(pid==0)
	{	
		printf("Calling to upper with %s\n",fdIn_One);
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
	return 0;
	// convert bit string to characters, then send to toUpperService
}
