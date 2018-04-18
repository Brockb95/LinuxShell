#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

// different colors for shell
#define BLU "\x1B[34m"
#define GRE "\x1B[32m"
#define RESET "\x1B[0m"
// functions for interpreting and executing commands
void loop();
char * read_command();
char ** parse(char *);
int launch(char **);
int execute(char **);
// shell builtin functions declarations
int mycat(char ** args);
//int myls(char ** args);
int mycd(char **);
int mypwd();
int myexit();
// list of builtin commands
char * builtin_str[] = {
	"mycat",
//	"cp",
//	"ls",
	"mycd",
	"mypwd",
	"myexit"
};
// list of builtin function pointers
int (*builtin_func[]) (char **) = {
	&mycat,
//	&mycp,
//	&myls,
	&mycd,
	&mypwd,
	&myexit
};
// finds number of builtin functions
int num_builtins(){
	return sizeof(builtin_str) / sizeof(char *);
}




int mycp(char **);
	// handlers for mycp
	int mycpfiletofile(char **);
	int mycpfiletodir(char **);
	int mycpdirtodir(char **);



int main(int argc, char ** argv){
	// load config

	// run REPL loop
	loop();
	// cleanup
	return 0;
}






void loop(){
	char * line;
	char ** args; // array of strings
	int status;
	char cwd[1024];
	// read commands
	// parse the commands for commands, args, and options
	// execute 

	do{	
		getcwd(cwd, sizeof(cwd));
		printf(GRE "mysh:" RESET);
		printf(BLU "%s> " RESET, cwd);
		line = read_command();
		args = parse(line);
		status = execute(args);

		free(line);
		free(args);
	}while(status);
}


// takes the current line from stdin as a command
char * read_command(){
	char * line = NULL;
	ssize_t buffer_size = 0;
	getline(&line, &buffer_size, stdin);
	return line;
}

#define TOK_DELIM " \t\r\n\a"
// parse string (char *) line for arguments and options
char ** parse(char * line){
	int buffer_size = 64, position = 0;
	char ** tokens = malloc(sizeof(char *) * buffer_size);
	char * token;

	if(!tokens){
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	token = strtok(line, TOK_DELIM);
	while(token != NULL){
		tokens[position] = token;
		position++;
	
		if(position >= buffer_size) {
			buffer_size += buffer_size;
			tokens = realloc(tokens, sizeof(char *) * buffer_size);
			if(!tokens){
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;	 
}
// executes builtin command if passed. If the command is not a builtin calls
// launch to run program
int execute(char ** args){
	int i;
	if(args[0] == NULL)
		return 1;

	for(i = 0; i < num_builtins(); i++){
		if(strcmp(args[0], builtin_str[i]) == 0){
			return (*builtin_func[i])(args);
		}
	}

	return launch(args);
}
// execute external command passed by (char **) args
int launch(char ** args){
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid < 0) // failure to fork
		perror("mysh");
	else if(pid == 0){ // child process
		if(execvp(args[0], args) == -1){
			fprintf(stderr, "mysh: invalid command\n");
			exit(EXIT_FAILURE);
		}
	}else{ // parent process
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	
	return 1;
}
// builtin command mycd; changes current directory
int mycd(char ** args){
	if(args[1] == NULL)
		fprintf(stderr, "mysh: expected argument to \"mycd\"\n");
	else
		if(chdir(args[1]) != 0)
			perror("mysh");
	return 1;
}
// builtin command pwd; prints working directory
int mypwd() {
	char cwd[1024];
	if(getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	
	return 1;
}
// builtin command exit; exits shell
int myexit(){
	return 0;
}





int mycat(char ** args) {

	char buffer[256];
	// pipe
	int fd[2], a;
	if (pipe(fd) < 0)
        exit(1);

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
		printf("mycat: %s: No such file 3  - Space must be placed between redirection\n", args[1]);
	}
	close(fd[0]);
	close(fd[1]);
	return 1;
}


















