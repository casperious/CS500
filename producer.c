#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

//-1 return is fork failure
//-2 return is pipe failure

int main(){
	FILE* ptr;
	FILE* binf;
	FILE* outf;
	FILE* clearDone;
	char str[65];
	
	int pid;
	
	ptr = fopen("data.inpf","r");
	binf = fopen("data.binf","w");
	fclose(binf);
	outf = fopen("data.outf","w");
	fclose(outf);
	//clearDone = fopen("data.done","w");
	//fclose(clearDone);
	if(NULL== ptr){
		printf("File not found \n");
		return(-1);
	}
	
	int count = 0;
	char ch;
	int fdOut[2];									//p=>c
	int fdIn[2];									//c=>p
	int consPid;
	if(pipe(fdOut)==-1)
	{
		printf("fdOut pipe failed\n");
		return -2;
	}
	if(pipe(fdIn)==-1)
	{
		printf("fdIn pipe failed\n");
		return -2;
	}
	//printf("fdIn is %d,%d and fdOut is %d,%d\n",fdIn[0],fdIn[1],fdOut[0],fdOut[1]);
	consPid = fork();														//have to remove dependency on pipe fd in services
	if(consPid==0)
	{
		/*close(0);															//consumer will read fdOut through stdIn
		dup(fdOut[0]);														
		close(fdOut[1]);													//close write of fdOut
		close(1);
		dup(fdIn[1]);														//consumer will write through stdOut
		close(fdIn[0]);	*/													//close read of fdIn	
		close(fdOut[1]);													//close write of p=>c
		close(fdIn[0]);														//close read of c=>p
		//close(0);
		//dup(fdOut[0]);
		char arg1[4];
		sprintf(arg1,"%d",fdOut[0]);
		char arg2[4];
		sprintf(arg2,"%d",fdIn[1]);
		execl("consumer","consumer",arg1,arg2,NULL);
	}
	else if(consPid>0)
	{
		clearDone = fopen("data.done","w");
		fclose(clearDone);
		/*close(1);
		dup(fdOut[1]);														//producer will write to fdOut through stdOut
		close(fdOut[0]);													//close read of fdOut
		close(0);															//uncomment to get input from consumer to stdout
		dup(fdIn[0]);														//producer will read from fdIn through stdIn
		close(fdIn[1]);		*/		  										//close write of fdIn
		close(fdOut[0]);													//close read of p=>c
		close(fdIn[1]);														//close write of c=>p
		//printf("Producer parent\n");
	}
	else
	{
		printf("Failed to create consumer fork\n");
		return -1;
	}
	char arg[4];
	sprintf(arg,"%d",fdOut[1]);
	int newPid;
	while((ch = getc(ptr))!=EOF)
	{
		if(count<64){
			str[count]=ch;
			count++;
			if(count==64)
			{
				newPid = fork();
				if(newPid==0)
				{
					printf("Calling encoder on \n*************\n %s\n**************\n",str);
					execl("encoderService","encoderService",str,"64",arg,NULL);
				}
				else if(newPid>0)
				{
					wait(NULL);
					printf("In loop parent \n");
				}
				else
				{
					printf("Fork in loop failed \n");
				}
			}
		}
		else
		{
			//count = 0;
			memset(str,'\0',65);
			str[0]=ch;
			count=1;
			printf("str read is now %s\n",str);
		}
	}
	count -=1;
	char countStr[64];
	sprintf(countStr,"%d",count);
	int status,options;
	//waitpid(newPid,&status,options);
	if(count>0){
		pid=fork();
		if(pid==0){
			//printf("In child \n");
			printf("Pushing last ^^^^^^^^^^^^^^^^^^^^^^^^^^\n %s  \n^^^^^^^^^^^^^^^^^^^^^^^\n of length %s\n",str,countStr);
			//printf("fdIn is %d,%d and fdOut is %d,%d\n",fdIn[0],fdIn[1],fdOut[0],fdOut[1]);
			execl("encoderService","encoderService",str,countStr,arg,NULL);
		}
		else if(pid>0){
			wait(NULL);
			printf("In producer parent \n");
		}
		else
		{
			printf("No fork \n");
		}
	}
	char buff[1025];
	ssize_t capped;
	close(fdOut[1]);
	int lastPid;
	while((capped=read(fdIn[0],buff,sizeof(buff)))>0)
	{
		//printf("recieved capitalized %s of size %ld\n",buff,(strlen(buff)/8)-3);
		//char strLen[64];
		//sprintf(strLen,"%ld",(strlen(buff)/8)-3);
		char argLast[4];
		sprintf(argLast,"%d",-1);
		//int lastPid;
		lastPid=fork();
		if(lastPid==0){
			printf("deframing capitalized\n");
			execl("deframe","deframe",buff,argLast,NULL);
		}
		else if(lastPid>0)
		{
			wait(NULL);
		}
		else
		{
			printf("Last fork\n");
		}	
		//waitpid(lastPid,&status,options);
	}
	close(fdIn[0]);
	/*
	read(fdIn[0],buff,sizeof(buff));
	printf("recieved capitalized %s of size %ld\n",buff,(strlen(buff)/8)-3);
	//char strLen[64];
	//sprintf(strLen,"%ld",(strlen(buff)/8)-3);
	char argLast[4];
	sprintf(argLast,"%d",-1);
	int lastPid;
	lastPid=fork();
	if(lastPid==0){
		printf("deframing capitalized\n");
		execl("deframe","deframe",buff,argLast,NULL);
	}
	else if(lastPid>0)
	{
		wait(NULL);
	}
	else
	{
		printf("Last fork\n");
	}
	*/
	waitpid(lastPid,&status,options);
	waitpid(consPid,&status,options);				//wait on consumer process to finish
	printf("finished consumer ending producer\n");
	
	close(fdOut[1]);
	fclose(ptr);
	return 0;

}
