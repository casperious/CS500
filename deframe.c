#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char* argv[])
{
	deframe(argv[1]);
	return 0;
}

int deframe(char* inData)
{
	//printf("Recieved %s in deframe\n",inData);
	
	char* characters[70]={"checkRemoveParityService"};
	int j =0;
	int k =1;
	char block[9]="00000000";
	for(int i =0;i<strlen(inData)+1;i++)
	{
		
		if(j<=7)
		{
			block[j]=inData[i];
			j++;
		}
		else
		{
			characters[k]=strdup(block);
			j = 0;
			block[j]=inData[i];
			j++;
			k++;
		}
	}
	if(strcmp(characters[1],"00010110")!=0 || strcmp(characters[2],"00010110")!=0)
	{
		printf("Incorrect syn chars characters[1] = %s chars[2] = %s\n",characters[1],characters[2]);
		return -1;
	}
	int pid;
	pid = fork();
	if(pid==0)
	{
		//printf("Sending blocks to check parity\n");
		execv("checkRemoveParityService",characters);
	}
	else if(pid>0)
	{
		wait(NULL);
	}
	else
	{
		printf("fork in deframe failed\n");
	}
	
	return 0;

}
