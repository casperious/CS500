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

/*
characters[0] is service name
chars[1] is fdIn[1]
chars[2] and [3] are syn
chars[4] is numChars
*/
int checkRemoveParity(char** characters)		
{
	int numCharsNumOnes = 0;			
	if(characters[4][0]=='0')
	{
		for(int i =1;i<8;i++)
		{
			if(characters[4][i]=='1')
			{
				numCharsNumOnes++;
			}
		}
		if(numCharsNumOnes%2==0)
		{
			printf("Num chars has wrong parity. Corrupted Data\n");
		}	
	}
	else
	{
		for(int i =1;i<8;i++)
		{
			if(characters[4][i]=='1')
			{
				numCharsNumOnes++;
			}
		}
		if(numCharsNumOnes%2==1)
		{
			printf("Num chars has wrong parity. Corrupted Data\n");
		}	
	}
	//remove first bit in characters[4]
	char lenBin[7]="";
	memcpy(lenBin,&characters[4][1],7);
	int numChars = (int)strtol(lenBin,NULL,2);
	char* removedBitsChars[numChars+3];										//+1 for decoder, +1 for fdIn[1], +1 for null
	int len = sizeof(removedBitsChars)/sizeof(*removedBitsChars);
	removedBitsChars[0]="decoder";
	removedBitsChars[1]=characters[1];
	int k =2;
	for(int i = 5;i<=numChars+4;i++)
	{
		char block[8];
		if(characters[i][0]=='0')
		{
			int numOnes = 0;
			for(int j=1;j<strlen(characters[i]);j++)
			{	
				if(characters[i][j]=='1')
				{
					numOnes++;
				}	
			}
			if(numOnes%2==0)
			{
				printf("Incorrect parity. Corrupted Data\n");
			}
			strncpy(block,characters[i]+1,7);
		}
		else
		{
			int numOnes = 0;
			for(int j=1;j<8;j++)
			{	
				if(characters[i][j]=='1')
				{
					numOnes+=1;
				}
			}
			if(numOnes%2==1)
			{
				printf("Incorrect parity. Corrupted Data\n");
			}
			strncpy(block,characters[i]+1,7);
		}
		removedBitsChars[k]=strdup(block);
		k++;
	}
	removedBitsChars[len-1]=NULL;
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
}
