#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc !=2){
		printf("Give us only one argument ./count <file_name>\n");
		return 1;
	}
	
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("File not found\n");
		return 1;
	}
	
	int c;
	while ((c = fgetc(fp)) != EOF) {
		if (c >= 32 && c <= 255) {
			printf("%c [%d]\n", c, c);
		} else {
			printf("[%d]\n", c);
		}
	}

	fclose(fp);
	return 0;
}
