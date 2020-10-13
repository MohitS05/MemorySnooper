#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<sys/stat.h>



int mapreader(char *procid, char str[])
{
	char ch;
	int i = 0;

	char fname[70];
	snprintf(fname, sizeof(fname), "/proc/%s/maps", procid);
	printf("\nThe file name to open : %s\n", fname);

	FILE *fp = fopen(fname, "rb+");
	if (fp == NULL) {
		fprintf(stderr, "File not found\n");
		perror("fopen()");
		return -1;
	}

	printf("\n\n\t\t\t\t\t\t~~~~ PRINTING FILE ~~~~ \n\n");
	while ((ch = fgetc(fp)) != EOF) {
		// printf("%c", ch);
		str[i] = ch;
		i++;
	}

	printf("~~~~Printing as string ~~~~\n%s", str);

	fclose(fp);

	return (0);

}
