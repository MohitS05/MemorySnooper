#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include<errno.h>
#include "mapread2.h"
#include "MemoryReader.h"

#define MAXCHAR 3000
#define MAXSIZE 100

//char buffer[MAXSIZE];

/* Trial 1 Failed....
int main(int argc, char *argv[])
{
	char str[MAXCHAR] = {0},ch;
	char str1[10],fname[20];
	int i = 0;
    
    snprintf(fname,sizeof(fname),"/proc/%s/mem",argv[1]);
    printf("\n%s\n",fname);

    FILE * fp = fopen(fname, "rb+");
    if (fp == NULL) {
    	fprintf(stderr, "File not found\n");
        perror("fopen()");
        return EXIT_FAILURE;}

    printf("\n\n\t\t\t\t\t\t~~~~ PRINTING MEMORY ~~~~ \n\n");
    while((ch = fgetc(fp)) != EOF){

      printf("%c", ch);
     }
      
 
  return 0;  
 }
 */

// Trial 2 , Changing Python code to read the whole heap...
int main(int argc, char **argv)
{
 if (argc < 3) {
        printf("Usage: %s <Proc_id> <heap/stack>\n", argv[0]);
        return 0;
    }

char str[MAXCHAR];
/*
str to be filled with /proc/{pid}/map 
code goes here
*/
char * pid = argv[1];
if(mapreader(pid,str))
	fprintf(stderr, " Failed to read map\n");


//char longString[] = "This is a long string.\nIt has multiple lines of text in it.\nWe want to examine each of these lines separately.\nSo we will do that.";
char * curLine = str;
while(curLine){

      char * nextLine = strchr(curLine, '\n');
      if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
      if(strstr(curLine,argv[2]))
      	break;
      //printf("curLine=[%s]\n", curLine);
      if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy    
      curLine = nextLine ? (nextLine+1) : NULL;
}

printf("The %s is [%s]", argv[2], curLine);

char * addr1 = strtok(curLine," ");
char * addr = strtok(addr1,"-");
addr1 = strtok(NULL,"-");
printf("\nThe addresses are %s \t-  %s\n",addr, addr1);


//int rdr= reader(pid,addr,addr1);

//void * ptr = (void *) strtol(argv[3],NULL,0);
//addr= "55e9b20562a0";
//addr1= "55e9b20562b2";
char addr_[17],addr1_[17];
snprintf(addr_,sizeof(addr_),"0x%s",addr);
snprintf(addr1_,sizeof(addr1_),"0x%s",addr1);
int rdr= reader(pid,addr_,addr1_);
switch (rdr)
{
  case 0:
    printf("\nSuccefully created file\n");
    break;
  case 1:
    fprintf(stderr,"\nFailed to open Memory\n");
    break;
  case 2:
    fprintf(stderr,"\nFailed to create file_dump\n");
    break;
  case 3:
    fprintf(stderr, "Failed to seek to file location\n");
    break;
  default:
    fprintf(stderr, "Unkown error recieved %d \n", rdr);
}


/*
char fname[50];
FILE * fp = fopen (fname,"rb+");
*/

return 0;

}