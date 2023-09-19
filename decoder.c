#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "encDec.h"

int main(int argc, char* argv[])
{
	//printf("In decoder\n");
	//printf("Argv[1] is %s",argv[1]);
	decode(argv);
}

int decode(char** blocks)
{
	printf("First block is %s\n",blocks[1]);
	// convert bit string to characters, then send to toUpperService
}
