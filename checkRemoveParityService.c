#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char* argv[])
{
	checkRemoveParity(argv);
	return 0;
}

int checkRemoveParity(char** characters)
{
	//printf("args[3] in removeParity is %s\n",characters[3]);
	int numCharsNumOnes = 0;
	if(characters[3][0]=='0')
	{
		for(int i =1;i<8;i++)
		{
			if(characters[3][i]=='1')
			{
				numCharsNumOnes++;
			}
		}
		if(numCharsNumOnes%2==0)
		{
			printf("Num chars has wrong parity\n");
			exit(-1);
		}	
	}
	else
	{
		for(int i =1;i<8;i++)
		{
			if(characters[3][i]=='1')
			{
				numCharsNumOnes++;
			}
		}
		if(numCharsNumOnes%2==1)
		{
			printf("Num chars has wrong parity\n");
			exit(-1);
		}	
	}
	int numChars = (int)strtol(characters[3],NULL,2);
	char* removedBitsChars[numChars+2];	
	removedBitsChars[0]="decoder";
	int k =1;
	//char* removedBitsChars[66]={""};
	for(int i = 4;i<=numChars+3;i++)
	{
		//printf("Looking at %s\n",characters[i]);
		char block[8];
		if(characters[i][0]=='0')
		{
			//printf("In 0\n");
			int numOnes = 0;
			for(int j=1;j<strlen(characters[i]);j++)
			{	
				if(characters[i][j]=='1')
				{
					numOnes++;
				}	
				//block[j-1]=characters[i][j];
			}
			//printf("Num ones is %d\t",numOnes);
			if(numOnes%2==0)
			{
				printf("Incorrect parity\n");
				exit(-10);
			}
			strncpy(block,characters[i]+1,7);
			//printf("Block is %s and num 1s is %d\n",block,numOnes);
		}
		else
		{
			//printf("In 1\n");
			int numOnes = 0;
			for(int j=1;j<8;j++)
			{	
				//printf("Checking %c\t",characters[i][j]);
				if(characters[i][j]=='1')
				{
					//printf("char is 1\n");
					numOnes+=1;
				}
				/*else{
					printf("\n");
				}*/
			}
			//printf("Num ones is %d\t",numOnes);
			if(numOnes%2==1)
			{
				printf("Incorrect parity\n");
				exit(-10);
			}
			strncpy(block,characters[i]+1,7);
			//printf("Block is %s and numOnes is %d\n",block,numOnes);
		}
		removedBitsChars[k]=strdup(block);
		//printf("block is %s\n",removedBitsChars[k]);
		k++;
	}
	removedBitsChars[numChars+1]=NULL;
	for(int i =0;i<=numChars+1;i++)
	{
		printf("removedBitsChars[%d] is %s\n",i,removedBitsChars[i]);
	}
	printf("All parity bits verified \n");
	int pid;
	pid = fork();
	if(pid==0)
	{
		execv("decoder",removedBitsChars);
	}
	else if (pid>0)
	{
		wait(NULL);
	}
	else
	{
		printf("Fork in removeParity failed\n");
		return -1;
	}
	return 0;
	
	return 0;

}
