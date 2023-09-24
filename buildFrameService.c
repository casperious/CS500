#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char *argv[])
{
	//printf("In buildFrameService \n");
	buildFrame(argv[1],argv[2]);
	return 0;
}

/*
Hardcode syn characters binary and prepend to inData
*/

void buildFrame(char *inData,char* fdOut_One){
	int fdOut;
	sscanf(fdOut_One,"%d",&fdOut);
	char syn[8] = "00010110";
	char frame[1025]="";
	strncat(frame,syn,8);
	strncat(frame,syn,8);
	strncat(frame,inData,strlen(inData));
	printf("Frame is %s\n",frame);
	FILE *fp;
	if(fdOut<7)
	{
		printf("fdOut[1] is %d\n",fdOut);
		fp = fopen("data.binf","a");
		fputs(frame,fp);
	}
	else
	{
		printf("fdIn[1] is %d\n",fdOut);
		fp = fopen("data.chck","a");
		fputs(frame,fp); 	
	}
	fclose(fp);
	write(fdOut,frame,sizeof(frame));						//write to pipe, be it fdOut or fdIn
	//printf("fdIn is %d,%d and fdOut is %d,%d\n",fdIn[0],fdIn[1],fdOut[0],fdOut[1]);
}
