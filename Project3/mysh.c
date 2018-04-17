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
//int mycat(char ** args);
//int myls(char ** args);
int mycd(char **);
int pwd();
int myexit();
// list of builtin commands
char * builtin_str[] = {
//	"cat",
//	"cp",
//	"ls",
	"cd",
	"pwd",
	"exit"
};
// list of builtin function pointers
int (*builtin_func[]) (char **) = {
//	&mycat,
//	&mycp,
//	&myls,
	&mycd,
	&pwd,
	&myexit
};
// finds number of builtin functions
int num_builtins(){
	return sizeof(builtin_str) / sizeof(char *);
}

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
int pwd(){
	char cwd[1024];
	if(getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	
	return 1;
}
// builtin command exit; exits shell
int myexit(){
	return 0;
}
