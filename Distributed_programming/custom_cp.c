#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc !=3){
		printf("Give us only two arguments ./count <file_name> <name_of_copy>\n");
		return 1;
	}
	
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("File not found\n");
		return 1;
	}

    FILE *fp2 = fopen("copy.txt", "w");
    if (fp2 == NULL) {
        printf("File not found\n");
        return 1;
    }

    int c;
    while ((c = fgetc(fp)) != EOF) {
        fputc(c, fp2);
    }

    fclose(fp);
    fclose(fp2);
}