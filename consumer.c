#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char *argv[])
{
	consumer(argv[1],argv[2]);
	return 0;
}

int consumer(char* fdOut_Zero,char* fdIn_One)
{
	int fdOut;
	sscanf(fdOut_Zero,"%d",&fdOut);
	//printf("fdOut[0] is %d\n",fdOut);
	int fdIn;
	sscanf(fdIn_One, "%d", &fdIn);
	//printf("fdIn[1] is %d\n",fdIn);
	char buff[1025];
	read(fdOut,buff,1025);
	printf("Recieved %s in consumer\n",buff);
	FILE* fp;
	fp = fopen("consumer.txt","w");
	fputs(buff,fp);
	fclose(fp);
	int pid1;
	pid1 = fork();
	if(pid1==0)
	{
		execl("deframe","deframe",buff,fdIn_One,NULL);
	}
	else if (pid1>0)
	{
		wait(NULL);
	}
	else
	{
		printf("Failed fork");
	}
	return 0;
	/*int pid;
	pid = fork();
	if(pid==0)
	{
		//printf("executing deframer on %s\n",buff);
		execl("deframe","deframe",buff,NULL);
	}
	else if(pid>0)
	{
		wait(NULL);
		printf("Finished calling deframe\n");
		//return 0;
	}
	else {
		printf("Failed fork\n");
	}*/
	return 0;

}
