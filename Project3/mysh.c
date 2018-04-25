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
int mycat(char **);
void mycp(const char*, const char*);
//int myls(char **);
int mycd(char **);
int mypwd();
int myexit();
// list of builtin commands
char * builtin_str[] = {
	"mycat",
	"mycp",
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

/*int mycp(char **);
	// handlers for mycp
	int mycpfiletofile(char **);
	int mycpfiletodir(char **);
	int mycpdirtodir(char **);
*/

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
	if (pipe(fd) < 0){
        	perror("mycat: ");
		return 1;
	}
	
	if(args[1] == NULL)
		fprintf(stderr, "mysh: expected argument to \"mycat\"\n");

	// check if file exists
	else if(strcmp(args[1], "<") == 0 && open(args[2], O_RDWR) == -1) {
		printf("mycat: %s: No such file  - Space must be placed between redirection\n",
args[2]);
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
	else if(strcmp(args[1], "<") == 0 && open(args[2], O_RDWR) != -1 && strcmp(args[3], ">") == 0)
{
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
		printf("mycat: %s: No such file 3  - Space must be placed between redirection\n",
args[1]);
	}
	close(fd[0]);
	close(fd[1]);
	return 1;
}

/*int mycp(char** args)
{
  int argc, in, out;
  char *from, *to;
  char buffer[100];
  ssize_t bytes;

  // check for argument
  if(argc < 3) {
    fprintf(stderr, "usage: %s source destination\n", args[0]);
    return 0;
  } 

  // open the files
  in = open(args[1], O_RDONLY);
  //check for errors
  if(!in){
	fprintf(stderr, "%s Cannot open file", args[1]);
   	return 1;
   }

  out = open(args[2], O_WRONLY|O_CREAT|O_TRUNC, 00644);
  if(!out){
	fprintf(stderr, "%s Cannot open file", args[2]);
	return 2;
  }

  // loop through and copy
  while ((bytes = read(in, buffer, 512)) > 0) {
    	if(write(out, buffer, bytes) < bytes){
		fprintf(stderr, "Cannot write to file");
		return 3;
	}
  }

  //must ends at 0
  if (bytes < 0) {
    fprintf(stderr, "error reading file");
    return 4;
  }

  // close the files
  close(in);
  close(out);
  return 5;
}*/


// to judge if a file is a directory or regular files   
// return 1, a dir; 0, regular file; -1, error   
int isdir(const char * path)   
{   
    struct stat bf;   
    // read status of the file   
    if(stat(path, &bf) == -1){   
        printf("isdir(%s), stat(%s) error!\n",path, path);   
        return -1;   
    }   
    if((S_IFMT & bf.st_mode) == S_IFDIR) {   
        return 1;   
    }   
    else   
        return 0;   
}   
   
   
// copy a file using open,creat,read and write   
// from and to are both absolute path   
// return 1, success; 0, error   
int cpfile(const char * from, const char * to)   
{   
    int f1, f2, n;   
    char buf[BUFSIZ];   
    struct stat old_mode;     
    // read status of the old file   
    if(stat(from, &old_mode) == -1){   
        printf("cpfile(%s, %s), stat(%s) error!\n", from, to, from);   
        return 0;   
    }   
    // open the old file   
    if( (f1 = open(from, O_RDONLY)) == -1){   
        printf("cpfile(%s, %s), can't open %s.\n", from, to, from);   
        return 0;   
    }   
    // create new file   
    if( (f2 = creat(to, old_mode.st_mode)) == -1){   
        printf("cpfile(%s, %s), can't create %s.\n",  from, to, to);   
        close(f1);   
        return 0;   
    }   
    // int fchmod(int fd, mode_t mode)   
    if(fchmod(f2, old_mode.st_mode) == -1){   
        printf("cpfile(%s, %s), fchmod(%s) error!\n", from, to, to);   
        return 0;   
    }   
    // read and write   
    while((n = read(f1, buf, BUFSIZ)) > 0){   
        if(write(f2, buf, n) != n){   
            printf("cpfile(%s, %s), write(%s) error!\n", from, to, to);   
            close(f1);   
            close(f2);   
            return 0;   
        }   
    }   
    close(f1);   
    close(f2);   
    return 1;   
}   
   
// get file name or directory name   
// NAME is stored in name   
void getfilename(char * bf, char * name)   
{   
    int i, n, j;   
    n = strlen(bf);   
    for(i = n - 1; i >=0 ; i--){   
        if(bf[i]=='/'){   
            break;   
        }   
    }   
    for(i++, j = 0; i < n; i++, j++)   
        name[j] = bf[i];   
    name[j] = '\0';   
}   
// copy a directory, including the files and sub-directories, into a directory that exists   
void cpdir(const char * from, const char * to)   
{   
     char bffrom[BUFSIZ],  bfto[BUFSIZ]; // use to store filepath of the source and destination   
     char name[BUFSIZ];   
     int flag;    
     flag = isdir(from);   
   
     strcpy(bffrom, from);   
     strcpy(bfto, to);   
   
     if(flag == 0) // regular file   
     {   
         getfilename(bffrom, name);  // get file name    
         strcat(bfto, "/");   
         strcat(bfto, name);   
             cpfile(bffrom, bfto);   
         return;   
     }   
     else    
     if(flag == 1) // directory   
     {   
        // make the same dir   
        getfilename(bffrom, name);  // get dir name    
        strcat(bfto, "/");   
        strcat(bfto, name);   
  
        if(strcmp(name, ".")==0 || strcmp(name, "..")==0 ){  
            return ; // current and parent!  
        }   
        struct stat old_mode;    // get the old_mode   
   
        if(stat(from, &old_mode) == -1){   
            printf("mkdir(%s), stat(%s) error!\n", bfto, bffrom);   
            return;   
        }   
   
        mkdir(bfto, old_mode.st_mode); // make dir bfto   
   
        chmod(bfto, old_mode.st_mode); // change mode of bfto   
   
   
        // copy the files and subdir in the dir   
        DIR * pdir;   
        struct dirent * pdirent;   
   
        pdir = opendir(bffrom);   
        while(1) {   
            pdirent = readdir(pdir) ;   
            if(pdirent == NULL)   
                break;   
            else{   
                strcpy(bffrom, from);//key   
                strcat(bffrom, "/");   
                strcat(bffrom, pdirent->d_name); // subfile or subdir path   
                cpdir(bffrom, bfto)  ;   // nested   
            }   
        }   
        closedir(pdir);  
        return ;   
     }   
     else   
        return ;   
}  
  
int main(int argc, char ** argv){
         // load config
 
         // run REPL loop
         loop();
         // cleanup
         // commandline   
         if(argc != 3) {
         printf("usage: %s from to\nwe should use quotation marks if from or to has a space.\n", argv[0]);
         return -1;
         }
         // call    
         cpdir(argv[1], argv[2]);
         return 0;
}

