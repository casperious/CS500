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
	consPid = fork();														//have to remove dependency on pipe fd in services
	if(consPid==0)
	{
		close(fdOut[1]);													//close write of p=>c
		close(fdIn[0]);														//close read of c=>p
		
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
		close(fdOut[0]);													//close read of p=>c
		close(fdIn[1]);														//close write of c=>p
	}
	else
	{
		printf("Failed to create consumer fork\n");
		return -1;
	}
	char arg[4];
	sprintf(arg,"%d",fdOut[1]);
	int newPid;
	int numFrames=0;
	while((ch = getc(ptr))!=EOF)
	{
		if(count<64){
			str[count]=ch;
			count++;
			if(count==64)
			{
				numFrames++;
				if(numFrames==3)
				{
					newPid=fork();
					if(newPid==0)
					{
						execl("errorService","errorService",str,"64",arg,NULL);
					}
					else if(newPid>0)
					{
						wait(NULL);
					}
					else
					{
						printf("Fork in error failed\n");
					}
				}
				else{
					newPid = fork();
					if(newPid==0)
					{
						
						execl("encoderService","encoderService",str,"64",arg,NULL);
					}
					else if(newPid>0)
					{
						wait(NULL);
					}
					else
					{
						printf("Fork in loop failed \n");
					}
				}
			}
		}
		else
		{
			memset(str,'\0',65);
			str[0]=ch;
			count=1;
		}
	}
	count -=1;
	char countStr[64];
	sprintf(countStr,"%d",count);
	int status,options;
	if(count>0){
		numFrames++;
		if(numFrames==3)
		{
			pid = fork();
			if(pid==0)
			{
				execl("errorService","errorService",str,countStr,arg,NULL);
			}
			else if(pid>0)
			{
				wait(NULL);
			}
			else
			{
				printf("Failed fork in error\n");
			}
		}
		else
		{
			pid=fork();
			if(pid==0){
				execl("encoderService","encoderService",str,countStr,arg,NULL);
			}
			else if(pid>0){
				wait(NULL);
			}
			else
			{
				printf("No fork \n");
			}
		}
	}
	char buff[1025];
	ssize_t capped;
	close(fdOut[1]);
	int lastPid;
	while((capped=read(fdIn[0],buff,sizeof(buff)))>0)
	{
		char argLast[4];
		sprintf(argLast,"%d",-1);
		lastPid=fork();
		if(lastPid==0){
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
	}
	close(fdIn[0]);
	waitpid(lastPid,&status,options);
	waitpid(consPid,&status,options);				//wait on consumer process to finish
	printf("finished consumer ending producer\n");
	close(fdOut[1]);
	fclose(ptr);
	return 0;

}
