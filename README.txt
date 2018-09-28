Project 1
Mason Witcraft - mason.witcraft@ou.edu  
9/28/2018

My program acts as a shell, with some custom behavior on certain keywords. I tokenize the inputs, and compare the first token to a list of words. If the token matches any of the predefined words, then it behaves how I have defined, if the token does not match any of the predefined words, then it gets passed straight to the system call for the system to handle. Notable changes with regard to normal shell functionality:
    help - prints contents of this README
    chdir (with no arguments) = pwd
Other functions simply behave like other system commands, listed below:
    wipe = clear
    filez = ls -1
    mimic = cp
    erase = rm
    morph = mv
    chdir (with argument) = cd

Compile by running 'make' or 'make all'
If issues arise, run 'make clean' then 'make all'
Executable can be run a few ways:
    1. Without piping in or supplying as an argument a batch file
        This will run a shell that accepts user input and acts accordingly
    2. With piping in a batch file (./project1 < batch or cat batch | ./project1) or supplying a batch file as an argument (./project1 batch)
        This will run the shell with the commands outlined in batch, printing the input after the prompt and the output on line beneath prompt. Program exits on end of batch file.
        
Current bugs:
    Shell repeats input below prompt if run without supplying batch file
    
Sources:
https://oudalab.github.io/cs3113fa18/projects/project1
    Used for miscellaneous information pertaining to project
https://stackoverflow.com/questions/2218290/concatenate-char-array-in-c
    Used to learn how to concatenate char pointers in C
https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c
    Used to find out how to copy files using C
http://man7.org/linux/man-pages/man2/sendfile.2.html
    Used for information on sendfile function
https://www.geeksforgeeks.org/c-program-delete-file/
    Used to learn how to delete files in C
http://man7.org/linux/man-pages/man3/remove.3.html
    Used for information on remove function
https://stackoverflow.com/questions/7180293/how-to-extract-filename-from-path/19200767
    Used to learn how to get file name from path in C
http://man7.org/linux/man-pages/man3/basename.3.html
    Used for information on basename function
https://stackoverflow.com/questions/17438493/moving-a-file-on-linux-in-c
    Used to learn how to move a file in C
http://man7.org/linux/man-pages/man2/rename.2.html
    Used for information on rename function
https://stackoverflow.com/questions/1293660/is-there-any-way-to-change-directory-using-c-language
    Used to learn how to change working directory in C
http://man7.org/linux/man-pages/man2/chdir.2.html
    Used for information on chdir function
