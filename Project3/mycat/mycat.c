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
	int size = 1000;
	char buffer[size];
	/* pipe */
	int fd[2];
	if (pipe(fd) < 0){
        	perror("mycat: ");
		return 1;
	}

	/* mycat textfile */
	if(argc == 2) {
		if (open(args[1], O_RDWR) != -1) {
			fd[0] = open(args[1], O_RDWR);
			read(fd[0], buffer, 1000);
			printf("%s", buffer);
		}
		else {
			printf("FILE NOT FOUND\n");
			return 1;
		}
	}

	/* mycat < textfile 
	 mycat < textfile > textfile */
	else if (argc == 1) {
    	fgets(buffer, size , stdin);
    	printf("%s", buffer);
	}
	else {
		printf("mycat: %s: ERROR\n", args[1]);
	}
	close(fd[0]);
	close(fd[1]);
	return 1;
}
