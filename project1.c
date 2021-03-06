#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <libgen.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

extern char** environ;

// Clears terminal "system clear"
int wipe(){
	system("clear");
	return 0;
}

// Lists files and directories in target directory
// If no directory is specified then prints files and directories in current directory
int filez(char* target){

	char* command;

	if(target == NULL)
		command = "ls -1";
	else{
		command = "ls -1 ";
		size_t spaceNeeded = strlen(command) + strlen(target);
		char* spaceNeededStr = (char*)(malloc)(spaceNeeded * sizeof(char));
		command = strcat(spaceNeededStr, command);
		command = strcat(command, target);
	}

	return system(command);
}

// Prints program's README
int help(char* projectPath){

	// Navigates to program's home directory to open README.txt
	char* readmeFileName = "/README.txt";
	size_t readmePathSize = strlen(projectPath) + strlen(readmeFileName);
	char* readmePath = (char*)(malloc)(readmePathSize * sizeof(char));
	readmePath = strcat(readmePath, projectPath);
	readmePath = strcat(readmePath, readmeFileName);

	FILE* readme = fopen(readmePath, "r");
	if(readme == NULL){
		fprintf(stderr, "ERROR: %s; cannot find README\n", strerror(errno));
		return -1;
	}
		
	// Steps through and prints contents of README to stdout
	char c = fgetc(readme);
	while(c != EOF){
		printf("%c", c);
		c = fgetc(readme);
	}
	return 0;
}

// Copies file from sourcePath to destPath
int mimic(char* sourcePath, char* destPath){

	// Determines if source file is a file or not
	struct stat sourceBuf;
	stat(sourcePath, &sourceBuf);
	if(S_ISREG(sourceBuf.st_mode) == 0){
		fprintf(stderr, "ERROR: Source is not file\n");
		return -1;
	}

	// Opens files as descriptors to feed into sendfile function
	int sourceDescriptor = open(sourcePath, O_RDWR);
	int destDescriptor = open(destPath, O_CREAT | O_RDWR, S_IRWXU);

	// Gets size of source file to provide to sendfile
	struct stat toGetSize;
	fstat(sourceDescriptor, &toGetSize);
	int sourceSizeInBytes = toGetSize.st_size;

	// Copies file
	// http://man7.org/linux/man-pages/man2/sendfile.2.html
	if(sendfile(destDescriptor, sourceDescriptor, NULL, sourceSizeInBytes) == -1){
		fprintf(stderr, "ERROR: Invalid mimic destination\n");
		return -1;
	}

	return 0;
}

// Removes file pointed to by path
int erase(char* path){ 
	// http://man7.org/linux/man-pages/man3/remove.3.html
	if(remove(path) == -1){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

// Copies files from sourcePath to destPath
int morph(char* sourcePath, char* destPath){

	// Determines whether destPath points to directory or not
	struct stat destPathBuf;
	stat(destPath, &destPathBuf);
	int isDestDirectory = S_ISDIR(destPathBuf.st_mode);

	// If destPath points to directory, the filename of the file pointed to by sourcePath (found by basename function)
	// is appended to destPath, to preserve filename after move
	char* slash = "/";
	if(isDestDirectory != 0){
		// http://man7.org/linux/man-pages/man3/basename.3.html
		char* sourceBaseName = basename(sourcePath);
		size_t destPathWithFileNameSize = strlen(destPath) + strlen(slash) + strlen(sourceBaseName);
		char* destPathWithFileName = (char*)(malloc)(destPathWithFileNameSize * sizeof(char));
		destPathWithFileName = strcat(destPathWithFileName, destPath);
		destPathWithFileName = strcat(destPathWithFileName, slash);
		destPathWithFileName = strcat(destPathWithFileName, sourceBaseName);
		destPath = destPathWithFileName;

	}

	// Rename function moves the file at sourcePath to destPath by renaming it starting at the root directory
	// http://man7.org/linux/man-pages/man2/rename.2.html
	if(rename(sourcePath, destPath) == -1){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

// Changes the current working dirctory if an input is provided and updates the PWD environment variable
// If no input is provided, prints current working directory to stdout
int mychdir(char* input){

	// http://man7.org/linux/man-pages/man2/chdir.2.html
	if(chdir(input) == -1){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return -1;
	}

	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	char* updateEnv = "PWD=";
	size_t updateEnvSize = strlen(updateEnv) + strlen(cwd);
	char* updateEnvSizeMem = (char*)(malloc)(updateEnvSize * sizeof(char));
	updateEnv = strcat(updateEnvSizeMem, updateEnv);
	updateEnv = strcat(updateEnv, cwd);
	putenv(updateEnv);

	return 0;
}

// Main function where program begins
int main(int argc, char** argv){

	char buf[MAX_BUFFER];
	char* args[MAX_ARGS];
	char** arg;
	char* prompt = "==>";
	int argNum;

	// Gets program home on startup in order to be able to open README on 'help' 
	char* pathPnt = getenv("PWD");
	char path[strlen(pathPnt) + 1];
	for(int i = 0; i < (int)strlen(pathPnt); ++i){
		path[i] = pathPnt[i];
	}
	path[strlen(pathPnt)] = '\0';

	// Opens provided batch file and stores it in 'batchFile'
	FILE* batchFile = NULL;
	// If batch file is provided as an argument
	if(argc == 2){
		batchFile = fopen(argv[1], "r");
		stdin = batchFile;
		if(batchFile == NULL)
			fprintf(stderr, "ERROR: %s", strerror(errno));
	}
	// If batch file is piped in 
	else if(argc == 1){
		batchFile = stdin;
	}
	else
		fprintf(stderr, "ERROR: Unexpected arguments");

	// Loops until end of stdin
	while(!feof(stdin)){

		/* Prints prompt out */
		fputs(prompt, stdout);

		/* Waits for input after prompt and stores the input with max size of MAX_BUFFER in buf */
		if(fgets(buf, MAX_BUFFER, stdin)){

			char* fullInput = buf;

			// Keeps track of number of arguments
			argNum = 0;

			// Tokenizes input based on SEPARATORS
			char* tkn = strtok(fullInput, SEPARATORS); 
			for(int i = 0; tkn != NULL; ++i){
				++argNum;
				args[i] = tkn;
				tkn = strtok(NULL, SEPARATORS);
			}

			// Prints next to prompt the input from provided batch file
			// Currently buggy, as when batch file is not provided, it prints out on a new line the 
			// command entered
			if(batchFile != NULL){
				for(int i = 0; i < argNum; ++i){
						printf("%s ", args[i]);
				}
				printf("\n");
			}

			// ------------------------------------------------------------------------------------------
			// Below compares command(first token) to predefined keywords
			// if command matches, then it runs that funtion with provided arguments(following tokens)
			// If command does not	match any kewords, then all tokens are passed to system function,
			// allowing system to handle the command
			// ------------------------------------------------------------------------------------------

			/* System exit */
			if(!strcmp(args[0], "esc")){
				if(argNum > 1){
					fprintf(stderr, "ERROR: Too many arguments\n");
				}
				else{
					break;
				}
			}

			/* System clear */
			else if(!strcmp(args[0], "wipe")){
				if(argNum > 1)
					fprintf(stderr, "ERROR: Too many arguments\n");
				else
					wipe();
			}

			// Prints files according to 'filez' function
			else if(!strcmp(args[0], "filez")){
				if(argNum > 2)
					fprintf(stderr, "ERROR: Too many arguments\n");
				else{
					if(argNum == 1)
						filez(NULL);
					if(argNum ==2)
						filez(args[1]);
				}
			}

			 // Prints environment variables to stdout
			else if(!strcmp(args[0], "environ")){
				if(argNum > 1){
					fprintf(stderr, "ERROR: Too many arguments\n");
				}
				else{ 
					char** env = environ;
					while(*env){
						printf("%s\n", *env);
						*env++;
					}
				
				}
			}

			// Prints provided arguments to stdout
			else if(!strcmp(args[0], "ditto")){
				if(argNum != 1){
					for(int i = 1; i < argNum; ++i){
						printf("%s ", args[i]);
					}
					printf("\n");
				}
			}

			/* Prints README */
			else if(!strcmp(args[0], "help")){
				if(argNum > 1)
					fprintf(stderr, "ERROR: Too many arguments\n");
				else
					help(path);
			}

			// Copies file pointed to by 1st argument to 2nd argument
			else if(!strcmp(args[0], "mimic")){
				if(argNum == 3)
					mimic(args[1], args[2]);
				else if(argNum < 3)
					fprintf(stderr, "ERROR: Too few arguments\n");
				else if(argNum > 3)
					fprintf(stderr, "ERROR: Too many arguments\n");
			}

			// Deletes files pointed to by ith argument
			else if(!strcmp(args[0], "erase")){
				if(argNum < 2)
					fprintf(stderr, "ERROR: Too few arguments\n");
				else
					for(int i = 1; i < argNum; ++i)
						erase(args[i]);
				
			}

			// Moves file pointed to by 1st argument to location pointed to by 2nd argument
			else if(!strcmp(args[0], "morph")){
				if(argNum == 3)
					morph(args[1], args[2]);
				else if(argNum < 3)
					fprintf(stderr, "ERROR: Too few arguments\n");
				else if(argNum > 3)
					fprintf(stderr, "ERROR: Too many arguments\n");
			}

			// Changes current working directory if argument is provided
			// if not, prints current working directory
			else if(!strcmp(args[0], "chdir")){
				if(argNum == 1)
					printf("%s\n", getenv("PWD"));
				else if(argNum > 2)
					fprintf(stderr, "ERROR: Too many arguments\n");
				else
					mychdir(args[1]);
			}

			// Command not defined here, so pass to system
			else{

				char* command = args[0];
				char* space = " ";

				for(int i = 1; i < argNum; ++i){
					size_t memToAllocate = strlen(command) + strlen(args[i]) + strlen(space);
					char* arrayOfCorrectSize = (char*)(malloc)(memToAllocate * sizeof(char));
					command = strcat(arrayOfCorrectSize, command);
					command = strcat(command, space);
					command = strcat(command, args[i]);
				}

				system(command);
			}

		}

	}
	// Newline printed for formatting
	printf("\n");
	return 0;
}
