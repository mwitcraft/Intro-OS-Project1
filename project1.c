#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

int wipe(){
	system("clear");
	return 0;
}

int main(int argc, char** argv){

	char buf[MAX_BUFFER];
	char* args[MAX_ARGS];
	char** arg;
	char* prompt = "==>";
	int argNum;

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
			if(!strcmp(args[0], "filez")){
				printf("Filez\n");
			}

			/* System env
			 * Lists all of the environment strings */
			if(!strcmp(args[0], "environ")){
				printf("environ\n");
			}

			/* System echo with args */
			/* 'Echo [comment]' */
			if(!strcmp(args[0], "ditto")){
				printf("'ditto' entered\n");
				system("echo mason");
			}

			/* Prints README */
			if(!strcmp(args[0], "help")){
				printf("help\n");
			}

			/* System 'cp [src] [dst]' */
			/* Don't use system cp call */
			if(!strcmp(args[0], "mimic")){
				printf("mimic\n");
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
