#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char *argv[])
{
	//printf("In parityAdder \n");
	addParity(argv[1],argv[2]);
	return 0;
}

/*
go through all characters and break them into chunks of 7. check for odd parity, and add a 0 or 1 to the MSB
*/
void addParity(char *inData, char* fdOut_One)
{
	char outData[1025] = "";
	int j =1;
	char buffer[9] = "0";
	for(int i =0;i<strlen(inData)+1;i++)
	{
		if(j<=7)
		{
			buffer[j] = inData[i];
			j++;
			
		}
		else
		{
			j = 1;
			i--;
			int numOnes = 0;
			char parityBit='\0';
			for(int k = 1;k<strlen(buffer);k++)
			{
				if(buffer[k]=='1')
				{
					numOnes++;
				}
			}
			if(numOnes%2==0)
			{
				parityBit='1';
			}
			else
			{
				parityBit='0';
			}
			buffer[0] = parityBit;
			strncat(outData,buffer,8);
		}
	}
	int pid;
	pid = fork();
	if(pid==0)
	{
		//printf("In parity child pushing %s data frame to buildFrameService\n", outData);
		execl("buildFrameService","buildFrameService",outData,fdOut_One,NULL);
	}
	else if(pid>0)
	{
		printf("In parity parent\n");
		wait(NULL);
	}

}
