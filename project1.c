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

	/* if(argNum == 1) */
	/* 	command = "ls -1"; */
	/* if(argNum == 2){ */
	/* 	command = "ls -1 "; */
	/* 	size_t spaceNeeded = strlen(command) + strlen(args[1]); */
	/* 	char* spaceNeededStr = (char*)(malloc)(spaceNeeded * sizeof(char)); */
	/* 	command = strcat(spaceNeededStr, command); */
	/* 	command = strcat(command, args[1]); */
	/* } */
	/* #<{(| system(command); |)}># */
        /*  */
	/* system(command); */
	
	/* return 0; */
}

int help(char* projectPath){

	char* readmeFileName = "/README.txt";
	size_t readmePathSize = strlen(projectPath) + strlen(readmeFileName);
	char* readmePath = (char*)(malloc)(readmePathSize * sizeof(char));
	readmePath = strcat(readmePath, projectPath);
	readmePath = strcat(readmePath, readmeFileName);

	FILE* readme = fopen(readmePath, "r");
		
	char c = fgetc(readme);
	while(c != EOF){
		printf("%c", c);
		c = fgetc(readme);
	}
	return 0;
}

int mimic(int sourceDescriptor, int destDescriptor){

	struct stat toGetSize;
	fstat(sourceDescriptor, &toGetSize);
	int sourceSizeInBytes = toGetSize.st_size;
	
	int sendfileStatus = sendfile(destDescriptor, sourceDescriptor, NULL, sourceSizeInBytes);
	

	return sendfileStatus;
}

int morph(char* sourcePath, char* destPath){
	DIR* sourceDir = opendir(sourcePath);
	if(errno == ENOENT)
		return -1;

	rename(sourcePath, destPath);
	return 0;
}

int mychdir(char* input){

	chdir(input);
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	printf("CWD: %s\n", cwd);
	char* updateEnv = "PWD=";
	size_t updateEnvSize = strlen(updateEnv) + strlen(cwd);
	char* updateEnvSizeMem = (char*)(malloc)(updateEnvSize * sizeof(char));
	updateEnv = strcat(updateEnvSizeMem, updateEnv);
	updateEnv = strcat(updateEnv, input);
	putenv(updateEnv);

	return 0;
}

/* int chdir(char* path){ */
/*  */
/* 	#<{(| printf("\t path: %s\n", path); |)}># */
/* 	char* cdCommand = "cd "; */
/* 	char* putEnvArg = "PWD="; */
/* 	if(path[0] == '/' || path[0] == '\\'){ */
/* 		size_t pathSize =  strlen(putEnvArg) + strlen(path); */
/* 		char* envArgMalloc = (char*)(malloc)(pathSize * sizeof(char)); */
/* 		putEnvArg = strcat(envArgMalloc, putEnvArg); */
/* 		printf("PutEnvArg: %s\n", putEnvArg); */
/* 		putEnvArg = strcat(putEnvArg, path); */
/* 		#<{(| free(envArgMalloc); |)}># */
/* 		printf("New Dir = %s\n", putEnvArg); */
/* 		putenv(putEnvArg); */
/* 	} */
/* 	return 0; */
/* } */

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

	while(!feof(stdin)){

		/* Prints prompt out */
		fputs(prompt, stdout);
		/* Waits for input after prompt and stores the input with max size of MAX_BUFFER in buf */
		if(fgets(buf, MAX_BUFFER, stdin)){

			char* fullInput = buf;

			/* arg = args; */
			/* *arg++ = strtok(buf, SEPARATORS); */
			/* while((*arg++ = strtok(NULL, SEPARATORS))); */

			/* if(args[0]){ */https://www.linuxquestions.org/questions/programming-9/get-the-absolute-path-of-file-using-file-descriptor-214080/
                        /*  */
			/* } */

			/* printf("%s\n", fullInput); */
			/* printf("Buf: %s\n", buf); */

			argNum = 0;

			char* tkn = strtok(fullInput, SEPARATORS); 
			for(int i = 0; tkn != NULL; ++i){
				++argNum;
				args[i] = tkn;
				/* printf("%s\n", tkn); */
				tkn = strtok(NULL, SEPARATORS);
			}

			/* System exit */
			if(!strcmp(args[0], "esc")){
				break;
			}

			/* System clear */
			else if(!strcmp(args[0], "wipe")){
				wipe();
			}

			/* System 'ls -l [target]' where target may or may not be specified  */
			/* If target is a file, only print out that file */
			/* https://stackoverflow.com/questions/7920793/how-to-add-a-character-at-end-of-string# */
			else if(!strcmp(args[0], "filez")){
			
				if(argNum == 1)
					filez(NULL);
				if(argNum ==2)
					filez(args[1]);
				if(argNum > 2)
					fprintf(stderr, "Error, too many arguments");
				/* char* command; */
				/* #<{(| system(command); |)}># */
                                /*  */
                                /*  */
				/* if(argNum == 1) */
				/* 	command = "ls -1"; */
				/* if(argNum == 2){ */
				/* 	command = "ls -1 "; */
				/* 	size_t spaceNeeded = strlen(command) + strlen(args[1]); */
				/* 	char* spaceNeededStr = (char*)(malloc)(spaceNeeded * sizeof(char)); */
				/* 	command = strcat(spaceNeededStr, command); */
				/* 	command = strcat(command, args[1]); */
				/* } */
				/* #<{(| system(command); |)}># */
                                /*  */
				/* system(command); */

				/* if(argNum == 2){ */
				/* 	if(!strcmp(&args[1][0], "/") || !strcmp(&args[1][0], "\\")){ */
				/* 		char* cwd = getenv("PWD"); */
				/* 	} */
				/* } */
                                /*  */
				/* char* cwd = getenv("PWD"); */
				/* #<{(| printf("\t%s\n", cwd); |)}># */
                                /*  */
				/* char* newDir = strcat(cwd, "/"); */
				/* chc refer to where program livesar* to = strcat(newDir, args[1]); */
				/* printf("\t%s\n", to); */

				/* #<{(| printf("\t%i\n", argNum); |)}># */
				/* #<{(| printf("Filez\n"); |)}># */
				/* char* command; */
				/* if(argNum == 2){ */
                                /*  */
				/* 	command = "ls -1 "; */
                                /*  */
				/* 	#<{(| command = strcat(command, args[1]); |)}># */
				/* 	#<{(| printf("\t\tCommand: %s", command); |)}># */
                                /*  */
				/* 	size_t len = strlen(command) + strlen(args[1]); */
				/* 	char* ret = (char*)(malloc)(len * sizeof(char) + 1); */
				/* 	#<{(| *ret = '\0'; |)}># */
				/* 	command = strcat(strcat(ret, command), args[1]); */
				/* 	#<{(| printf("\t\tCommand: %s\n", command); |)}># */
				/* } else { */
				/* 	command = "ls -1"; */
				/* } */
				/* #<{(| system("ls -l ../"); |)}># */
				/* system(command); */
			}

			/* System env
			 * Lists all of the environment strings */
			else if(!strcmp(args[0], "environ")){
				/* putenv("PWD=I/Changed/It"); */
				/* printf("environ\n"); */
				char** env = environ;
				while(*env){
					printf("%s\n", *env);
					/* if(strstr(*env, "PWD") != NULL && strstr(*env, "OLDPWD") == NULL){ */
					/* 	#<{(| *env = "PWD=I/CHANGED/IT"; |)}># */
					/* 	printf("\t\t%s\n", *env); */
					/* } */
					*env++;
					/* printf("%s\n", *env++); */

				}
				
			}

			/* System echo with args */
			/* 'Echo [comment]' */
			else if(!strcmp(args[0], "ditto")){
				/* char* comment; */
				for(int i = 1; i < argNum; ++i){
					printf("%s ", args[i]);
				}
				printf("\n");
			}

			/* Prints README */
			else if(!strcmp(args[0], "help")){
				help(path);
			}

			/* System 'cp [src] [dst]' */
			/* Don't use system cp call */
			else if(!strcmp(args[0], "mimic")){
				int sourceDescriptor = open(args[1], O_RDWR);
				int destDescriptor = open(args[2], O_CREAT | O_RDWR, S_IRWXU);
								
				mimic(sourceDescriptor, destDescriptor);
			}

			/* System 'rm [file]'	 */
			/* Don't use system rm call */
			else if(!strcmp(args[0], "erase")){
				printf("erase\n");
				remove(args[1]);
				
			}

			/* System 'mv [src] [dst]' */
			/* Don't use system call */
			else if(!strcmp(args[0], "morph")){
				morph(args[1], args[2]);
			}

			/* System 'cd [path]' */
			/* If path is not specified, print current working directory */
			/* Update PWD environment variable using putenv function */
			else if(!strcmp(args[0], "chdir")){
				/* char* cwd = getenv("PWD"); */
				/* char* cwddash = strcat(cwd, "/"); */
				/* char* to = strcat(cwddash, args[1]); */
				/* printf("\t%s\n", to); */
				/* if(chdir(to) == -1){ */
				/* 	printf("Error\n"); */
				/* } */
				mychdir(args[1]);
				/* chdir(args[1]); */
				/* char cwd[256]; */
				/* getcwd(cwd, sizeof(cwd)); */
				/* printf("CWD: %s\n", cwd); */
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
