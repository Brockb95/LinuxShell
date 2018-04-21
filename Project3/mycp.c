
int mycp(char **);
	// handlers for mycp
	int mycpfiletofile(char **);
	int mycpfiletodir(char **);
	int mycpdirtodir(char **);

// builtin command mycp; copies the contents of the first argument to name of 
// second argument
int mycp(char ** args){
	if(args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "mysh: expected usage: mycp <src> foo <dest> foo1\n");
		return 1;
	}

	struct stat stat0;
	struct stat stat1;

	if((stat(args[1], &stat0)) == -1){
		perror("mycp <src>");
		return 1;
	}
	if((stat(args[2], &stat1)) == -1){
		stat1 = stat0;
	}

	if(S_ISREG(stat0.st_mode) && S_ISREG(stat1.st_mode))
		mycpfiletofile(args);
	else if(S_ISREG(stat0.st_mode) && S_ISDIR(stat1.st_mode))
		mycpfiletodir(args);
	else if(S_ISDIR(stat0.st_mode) && S_ISDIR(stat1.st_mode))
		mycpdirtodir(args);
	else
		fprintf(stderr, "mycp: cannot copy dir to file\n");

	return 1;
}
// called by mycp if <src> and <dest> are both files
int mycpfiletofile(char ** args){
	int files[2];
	char buffer[1024];
	ssize_t count;

	if((files[0] = open(args[1], O_RDONLY)) == -1){
		perror("mysh <src>");
		return 1;
	}
	if((files[1] = open(args[2], O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR)) == -1){
		perror("mysh <target>");
		return 1;
	}

	while((count = read(files[0], buffer, sizeof(buffer))) != 0)
		write(files[1], buffer, count);
}
//called by mycp if <src> is a file and <dest> is directory
int mycpfiletodir(char ** args){
