#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char* argv[]){
	int inputFd, outputFd, openFlags;
	int filePerms;
	int numRead;
	char buf[BUF_SIZE];

	inputFd = open("Resume_2018.pdf", O_RDONLY);

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	outputFd = open("New.pdf", openFlags, filePerms);

	while((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
		if(write(outputFd, buf, numRead) != numRead)
			return 1;
	if(numRead == -1)
		return 1;

	close(inputFd);

	close(outputFd);

	return 0;
}



/* If commands read in from file, print out commands */
/* chdir needs to change environment variable  */
/* If you move a directory into another directory, the directory being moved becomes a child of the destination director */
/* If destination is a file, raise error */
