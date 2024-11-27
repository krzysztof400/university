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
	        perror("Error opening file");
	        return 2; 
	}

	int charCount = 0;
    	char ch;

	for ( ch = getc(fp); ch != EOF; ch = getc(fp)){
		charCount++;
	}
	
	fclose(fp);

	printf("Number of characters in the file: %d\n", charCount);

	return 0;	
}
