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

int wipe(){
	system("clear");
	return 0;
}

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

int help(char* projectPath){

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
		
	char c = fgetc(readme);
	while(c != EOF){
		printf("%c", c);
		c = fgetc(readme);
	}
	return 0;
}

int mimic(char* sourcePath, char* destPath){

	struct stat sourceBuf;
	stat(sourcePath, &sourceBuf);
	if(S_ISREG(sourceBuf.st_mode) == 0){
		fprintf(stderr, "ERROR: Source is not file\n");
		return -1;
	}

	int sourceDescriptor = open(sourcePath, O_RDWR);
	int destDescriptor = open(destPath, O_CREAT | O_RDWR, S_IRWXU);

	struct stat toGetSize;
	fstat(sourceDescriptor, &toGetSize);
	int sourceSizeInBytes = toGetSize.st_size;

	if(sendfile(destDescriptor, sourceDescriptor, NULL, sourceSizeInBytes) == -1){
		fprintf(stderr, "ERROR: Invalid mimic destination\n");
		return -1;
	}

	return 0;
}

int erase(char* path){ 
	if(remove(path) == -1){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

int morph(char* sourcePath, char* destPath){

	struct stat destPathBuf;
	stat(destPath, &destPathBuf);
	int isDestDirectory = S_ISDIR(destPathBuf.st_mode);

	char* slash = "/";
	if(isDestDirectory != 0){
		char* sourceBaseName = basename(sourcePath);
		size_t destPathWithFileNameSize = strlen(destPath) + strlen(slash) + strlen(sourceBaseName);
		char* destPathWithFileName = (char*)(malloc)(destPathWithFileNameSize * sizeof(char));
		destPathWithFileName = strcat(destPathWithFileName, destPath);
		destPathWithFileName = strcat(destPathWithFileName, slash);
		destPathWithFileName = strcat(destPathWithFileName, sourceBaseName);
		destPath = destPathWithFileName;

	}

	if(rename(sourcePath, destPath) == -1){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

int mychdir(char* input){

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

int main(int argc, char** argv){

	char buf[MAX_BUFFER];
	char* args[MAX_ARGS];
	char** arg;
	char* prompt = "==>";
	int argNum;

	char* pathPnt = getenv("PWD");
	char path[strlen(pathPnt) + 1];
	for(int i = 0; i < (int)strlen(pathPnt); ++i){
		path[i] = pathPnt[i];
	}
	path[strlen(pathPnt)] = '\0';
	
	FILE* batchFile;
	if(argc == 2){
		batchFile = fopen(args[1], "r");
		if(batchFile == NULL)
			fprintf(stderr, "ERROR: %s", strerror(errno));
	}
	else if(argc == 1)
		batchFile = stdin;
	else
		fprintf(stderr, "ERROR: Unexpected arguments");
	

	while(!feof(stdin)){

		/* Prints prompt out */
		fputs(prompt, stdout);

		/* Waits for input after prompt and stores the input with max size of MAX_BUFFER in buf */
		if(fgets(buf, MAX_BUFFER, stdin)){

			char* fullInput = buf;

			argNum = 0;

			char* tkn = strtok(fullInput, SEPARATORS); 
			for(int i = 0; tkn != NULL; ++i){
				++argNum;
				args[i] = tkn;
				/* printf("%s\n", tkn); */
				tkn = strtok(NULL, SEPARATORS);
			}

			if(batchFile != NULL){
				for(int i = 0; i < argNum; ++i){
					printf("%s", args[i]);
				}
				printf("\n");
			}

			/* if(batchFile != NULL){ */
			/* 	printf("%s", prompt); */
			/* 	for(int i = 0; i < argNum; ++i){ */
			/* 		printf("%s ", args[i]); */
			/* 	} */
			/* 	printf("\n"); */
			/* } */
			/* else{  */
			/* 	printf("%s", prompt); */
			/* } */


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

			/* System 'ls -l [target]' where target may or may not be specified  */
			/* If target is a file, only print out that file */
			/* https://stackoverflow.com/questions/7920793/how-to-add-a-character-at-end-of-string# */
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

			/* System env
			 * Lists all of the environment strings */
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

			/* System echo with args */
			/* 'Echo [comment]' */
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

			/* System 'cp [src] [dst]' */
			/* Don't use system cp call */
			else if(!strcmp(args[0], "mimic")){
				if(argNum == 3)
					mimic(args[1], args[2]);
				else if(argNum < 3)
					fprintf(stderr, "ERROR: Too few arguments\n");
				else if(argNum > 3)
					fprintf(stderr, "ERROR: Too many arguments\n");
			}

			/* System 'rm [file]'	 */
			/* Don't use system rm call */
			else if(!strcmp(args[0], "erase")){
				if(argNum < 2)
					fprintf(stderr, "ERROR: Too few arguments\n");
				else
					for(int i = 1; i < argNum; ++i)
						erase(args[i]);
				
			}

			/* System 'mv [src] [dst]' */
			/* Don't use system call */
			else if(!strcmp(args[0], "morph")){
				if(argNum == 3)
					morph(args[1], args[2]);
				else if(argNum < 3)
					fprintf(stderr, "ERROR: Too few arguments\n");
				else if(argNum > 3)
					fprintf(stderr, "ERROR: Too many arguments\n");
			}

			/* System 'cd [path]' */
			/* If path is not specified, print current working directory */
			/* Update PWD environment variable using putenv function */
			else if(!strcmp(args[0], "chdir")){
				if(argNum == 1)
					printf("%s\n", getenv("PWD"));
				else if(argNum > 2)
					fprintf(stderr, "ERROR: Too many arguments\n");
				else
					mychdir(args[1]);
			}

			else{

				char* command = args[0];
				char* space = " ";

				for(int i = 1; i < argNum; ++i){
					size_t memToAllocate = strlen(command) + strlen(args[i]) + strlen(space);
					char* arrayOfCorrectSize = (char*)(malloc)(memToAllocate * sizeof(char));
					command = strcat(arrayOfCorrectSize, command);
					command = strcat(command, space);
					command = strcat(command, args[i]);
					/* free arrayOfCorrectSize; */
				}

				system(command);
			}

		}

	}
	return 0;
}
