#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

int mycat(int argc, char ** args);


int main(int argc, char ** argv) 
{ 
	return mycat(argc, argv);
}




int mycat(int argc, char ** args) {

	char buffer[256];
	// pipe
	int fd[2], a;
	if (pipe(fd) < 0){
        	perror("mycat: ");
		return 1;
	}
	
	if(args[1] == NULL)
		fprintf(stderr, "mysh: expected argument to \"mycat\"\n");

	// check if file exists
	else if(strcmp(args[1], "<") == 0 && open(args[2], O_RDWR) == -1) {
		printf("mycat: %s: No such file  - Space must be placed between redirection\n", args[2]);
	}

	// for the command "mycat < file"
	else if(strcmp(args[1], "<") == 0 && args[3] == NULL) {
		fd[0] = open(args[2], O_RDWR);
		read(fd[0], buffer, 256);
		printf("%s", buffer);
	}

	// check if file exists
	else if(open(args[1], O_RDWR) == -1 && args[2] == NULL) {
		printf("mycat: %s: No such file \n", args[1]);
	}

	// for the command "mycat < file > file"
	else if(strcmp(args[1], "<") == 0 && open(args[2], O_RDWR) != -1 && strcmp(args[3], ">") == 0) {
		if (open(args[4], O_RDWR) == -1) {
			remove(args[4]);
		}
		// open file to write to buffer
		fd[0] = open(args[2], O_RDWR);
		// read content of file and write to buffer
		a = read(fd[0], buffer, 256);
		// open file to write from buffer
		fd[1] = open(args[4], O_RDWR | O_CREAT, 0777);
		// write to second file
		write(fd[1], buffer, a-1);
	}

	
	// for the command "mycat filename"
	else if (open(args[1], O_RDWR) != -1 && args[2] == NULL) {
		fd[0] = open(args[1], O_RDWR);
		read(fd[0], buffer, 256);
		printf("%s", buffer);
	}


	else {
		printf("mycat: %s: No such file - Space must be placed between redirection\n", args[1]);
	}
	close(fd[0]);
	close(fd[1]);
	return 1;
}
