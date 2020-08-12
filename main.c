#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "dtmf.h"

#define MAXDIGIT 15

void gethelp();

void test(char* text, int innum){
	printf("%s\n", text);
	printf("%d\n", innum);
}

int main(int argc, char *argv[]) {
	int dtmfnum;

	if (argc != 3 || strstr(argv[1], ".wav") == NULL || strstr(argv[1], "-h")){
		gethelp();
		return 1;
	}

	dtmfnum = atoi(argv[2]);
	test(argv[1], dtmfnum);
	
	return 0;
}

void gethelp(void) {
	printf("Usage: cre_dtmf [Options] [Output File name] [number]...\n\n");
	printf("OPTION\t\tLong Option\tExplantion\n");
	printf("-h\t\t--help\t\tShow help and exit\n");
}
