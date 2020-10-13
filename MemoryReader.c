

#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>


//#define BUFFER_LENGHT 4005





int reader(char *rpid, char *S_addr, char *E_addr)
{


// PARSE CLI ARGS

	pid_t pid = strtol(rpid, NULL, 10);
	printf(" * Launching with a target PID of: %d\n", pid);

	void *remotePtr = (void *) strtol(S_addr, NULL, 0);
	printf(" * Launching with a target address of 0x%llx\n",remotePtr);

	size_t bufferLength = strtol("4000", NULL, 10);
	printf(" * Launching with a buffer size of %ld bytes.\n", bufferLength);

	unsigned long int strtadd, endadd, sizetoread, readalready = 0;

	strtadd = strtol(S_addr, NULL, 0);
	endadd = strtol(E_addr, NULL, 0);
	sizetoread = endadd - strtadd;

	int myptr = 0;
// Build iovec structs
	struct iovec local[1];
	local[0].iov_base = calloc(bufferLength, sizeof(char));
	local[0].iov_len = bufferLength;

	struct iovec remote[1];
	remote[0].iov_base = remotePtr;
	remote[0].iov_len = bufferLength;

// Call process_vm_readv - handle any error codes
	char dumpfname[25];
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	/* Creating the dump file name as Mday_Month_Hour_Min_Pid */
	snprintf(dumpfname, sizeof(dumpfname), "%d_%d_%d_%d_Proc%s.txt"
		 ,timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_hour, timeinfo->tm_min,
		 rpid);
	int filedesc =
	    open(dumpfname, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (!filedesc) {
		fprintf(stderr, "Reader failed to create file\n");
		perror("open()");
		return -1;
	}
	ssize_t nread;
	char mystuff[bufferLength];
      readmore:
	if (myptr) {
		remote[0].iov_base = remotePtr + 4000 * myptr;
	}
	nread = process_vm_readv(pid, local, 2, remote, 1, 0);
	if (nread < 0) {
		switch (errno) {
		case EINVAL:
			printf("ERROR: INVALID ARGUMENTS.\n");
			break;
		case EFAULT:
			printf
			    ("ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n");
			break;
		case ENOMEM:
			printf("ERROR: UNABLE TO ALLOCATE MEMORY.\n");
			break;
		case EPERM:
			printf
			    ("ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n");
			break;
		case ESRCH:
			printf("ERROR: PROCESS DOES NOT EXIST.\n");
			break;
		default:
			printf("ERROR: AN UNKNOWN ERROR HAS OCCURRED.\n");
		}

		return -1;
	}
	myptr++;


	if (sizetoread >= 4000) {
		write(filedesc, local[0].iov_base, nread);	//Writing to new file

		sizetoread -= nread;
		goto readmore;
	}

/*If there is block less then 4000 left, reading that too...*/


	if (sizetoread > 0)
		write(filedesc, local[0].iov_base, nread);


//Cleanup
	close(filedesc);
//Converting to Hex dump for better redaibility
	char convert[70];
	snprintf(convert,sizeof(convert),"xxd %s %sConverted",dumpfname,dumpfname);
	system(convert);


	return 0;


}
