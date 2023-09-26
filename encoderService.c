#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char *argv[])
{
	//printf("In encoder \n");
	//printf("string length passed is %s of size %ld\n",argv[2],strlen(argv[2]));
	encode(argv[1],argv[2],argv[3]);
	return 0;
}

int encode(char *inData,char* len, char* fdOut_One){
	char bin[1025]="";
	int length;
	sscanf(len,"%d",&length);
	printf("length of %s is %d\n",inData,length);
	int num = length;
	int j =6;
	char res[7]="0000000";
	while(num>0)
	{
		int mod = num%2;
		res[j]=mod+'0';
		num=num/2;
		j--;
	}
	//printf("Binary result of length is %s\n",res);
	strncat(bin,res,7);
	
	for(int i =0;i<length;i++){
		int ascii = inData[i];										//Get ascii int value of character
		int num = ascii;
		int j = 6;
		char result[7]="0000000";
		while(num>0)												//get binary representation of ascii value and store it in 		result
		{
			int mod = num%2;										//get remainder by dividing ascii by 2
			result[j] = mod+'0';									//store the character value of 0 or 1 into result starting from the highest bit
			num=num/2;												//divide ascii by 2
			j--;													//move to next lowest bit to be stored	
		}
		//printf("result for inData[%d] is %s\n",i,result);
		strncat(bin,result,7);
	}
	int pid;
	pid = fork();
	if(pid==0){
		//printf("In encoder pushing \n %s as \n %s to parityAddService\n",inData,bin);
		execl("parityAddService","parityAddService",bin,fdOut_One,NULL);
	}
	else if(pid>0)
	{
		wait(NULL);
		//printf("Sent binary to parity service\n");
	}
	else
	{
		printf("No fork\n");
	}
	
	return 0;

}
