#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char* argv[])
{
	//printf("In toUpper\n");
	toUpper(argv[1],argv[2]);
}

int toUpper(char* str, char* len)
{
	int length;
	sscanf(len,"%d",&length);
	printf("Checking chars in %s of length %d and converting to upper\n", str,length);
}
