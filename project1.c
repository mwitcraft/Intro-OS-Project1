#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

extern char** environ;

int wipe(){
	system("clear");
	return 0;
}

int filez(){
	return 0;
}

int mimic(int sourceDescriptor, int destDescriptor){
	struct stat toGetSize;
	fstat(sourceDescriptor, &toGetSize);
	int sourceSizeInBytes = toGetSize.st_size;
	
	int sendfileStatus = sendfile(destDescriptor, sourceDescriptor, NULL, sourceSizeInBytes);
	

	return sendfileStatus;
}

int main(int argc, char** argv){

	char buf[MAX_BUFFER];
	char* args[MAX_ARGS];
	char** arg;
	char* prompt = "==>";
	int argNum;https://www.google.com/search?q=c+open+permission+denied&ie=utf-8&oe=utf-8&client=firefox-b-1-ab

	/* #<{(| printf("argc: %i", argc); |)}># */
	/* for(int i = 0; i < argc; ++i){ */
	/* 	fputs("==>", stdout); */
	/* 	printf("Arg[%i]: %s\n", i, argv[i]); */
	/* } */

	while(!feof(stdin)){

		/* Prints prompt out */
		fputs(prompt, stdout);

		/* Waits for input after prompt and stores the input with max size of MAX_BUFFER in buf */
		if(fgets(buf, MAX_BUFFER, stdin)){
			
			/* arg = args; */
			/* *arg++ = strtok(buf, SEPARATORS); */
			/* while((*arg++ = strtok(NULL, SEPARATORS))); */

			/* if(args[0]){ */
                        /*  */
			/* } */

			argNum = 0;

			char* tkn = strtok(buf, SEPARATORS); 
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
			if(!strcmp(args[0], "wipe")){
				wipe();
			}

			/* System 'ls -l [target]' where target may or may not be specified  */
			/* If target is a file, only print out that file */
			/* https://stackoverflow.com/questions/7920793/how-to-add-a-character-at-end-of-string# */
			if(!strcmp(args[0], "filez")){
				/* printf("\t%i\n", argNum); */
				/* printf("Filez\n"); */
				char* command;
				if(argNum == 2){

					command = "ls -1 ";

					/* command = strcat(command, args[1]); */
					/* printf("\t\tCommand: %s", command); */

					size_t len = strlen(command) + strlen(args[1]);
					char* ret = (char*)(malloc)(len * sizeof(char) + 1);
					/* *ret = '\0'; */
					command = strcat(strcat(ret, command), args[1]);
					/* printf("\t\tCommand: %s\n", command); */
				} else {
					command = "ls -1";
				}
				/* system("ls -l ../"); */
				system(command);
			}

			/* System env
			 * Lists all of the environment strings */
			if(!strcmp(args[0], "environ")){
				printf("environ\n");
				char** env = environ;
				while(*env){
					if(strstr(*env, "PWD") != NULL && strstr(*env, "OLDPWD") == NULL){
						/* *env = "PWD=I/CHANGED/IT"; */
						printf("\t\t%s\n", *env);
					}
					*env++;
					/* printf("%s\n", *env++); */

				}
				
			}

			/* System echo with args */
			/* 'Echo [comment]' */
			if(!strcmp(args[0], "ditto")){
				char* comment;
				for(int i = 1; i < argNum; ++i){
					printf("%s ", args[i]);
				}
				printf("\n");
			}

			/* Prints README */
			if(!strcmp(args[0], "help")){
				printf("help\n");
			}

			/* System 'cp [src] [dst]' */
			/* Don't use system cp call */
			if(!strcmp(args[0], "mimic")){
				int sourceDescriptor = open(args[1], O_RDWR);
				int destDescriptor = open(args[2], O_CREAT | O_RDWR, S_IRWXU);
								
				mimic(sourceDescriptor, destDescriptor);
			}

			/* System 'rm [file]'	 */
			/* Don't use system rm call */
			if(!strcmp(args[0], "erase")){
				printf("erase\n");
			}

			/* System 'mv [src] [dst]' */
			/* Don't use system call */
			if(!strcmp(args[0], "morph")){
				printf("morph\n");
			}

			/* System 'cd [path]' */
			/* If path is not specified, print current working directory */
			/* Update PWD environment variable using putenv function */
			if(!strcmp(args[0], "chdir")){
				printf("chdir\n");
			}

		}

	}
	return 0;
}
