#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

void stuff(){
	DIR *dir;
	struct dirent *myfile;
	struct stat mystat;
		dir = opendir(".");
		while((myfile = readdir(dir)) != NULL){
			stat(myfile->d_name, &mystat);
			char *name = myfile->d_name;
			//if(name == argv[3]){//only print one file
				printf( (S_ISDIR(mystat.st_mode)) ? "d" : "-");//directory
				printf( (mystat.st_mode & S_IRUSR) ? "r" : "-");//user permissions
				printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
				printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
				printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");//group permissions
				printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
				printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
				printf( (mystat.st_mode & S_IROTH) ? "r" : "-");//other permissions
				printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
				printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");
				printf(" %ld", (long)mystat.st_uid);//user id name
				printf(" %ld", (long)mystat.st_gid);//group id
				printf(" %d", (int)mystat.st_size);//size			correct
				printf(" %d", ctime(&mystat.st_mtime));//last modified
				printf(" %s\n", myfile->d_name);//name of file			correct
			//}		
		}
		closedir(dir);
}

void directory(char *direct){
	DIR *dir;
	struct dirent *myfile;
	struct stat mystat;
printf("FUCK");
		int count = 0;//helps keep the print out pretty
		dir = opendir(direct);//open dir to view
		while((myfile = readdir(dir)) != NULL){
			stat(myfile->d_name, &mystat);
			printf("%s    ", myfile->d_name);
			count++;
			if((count = count % 5) == 0)
				printf("\n");//print new line after 5 files
		}
		printf("\n");//keeps things pretty
		closedir(dir);
}

void lsCAT(){
	DIR *dir;
	struct dirent *myfile;
	struct stat mystat;
printf("FUCK");
		dir = opendir(".");//open current dir to view
		while((myfile = readdir(dir)) != NULL){
			stat(myfile->d_name, &mystat);
			printf("%s\n", myfile->d_name);
		}//done
		closedir(dir);
}

void ls(){
	DIR *dir;
	struct dirent *myfile;
	struct stat mystat;
printf("FUCK");
		int count = 0;//helps keep the print out pretty
		dir = opendir(".");//open current dir to view
		while((myfile = readdir(dir)) != NULL){
			stat(myfile->d_name, &mystat);
			printf("%s\t", myfile->d_name);
			count++;
			if((count = count % 5) == 0)
				printf("\n");//print new line after 5 files
		}
		printf("\n");//keeps things pretty
		closedir(dir);
}

void cat(){
	DIR *dir;
	struct dirent *myfile;
	struct stat mystat;
printf("FUCK");
		char getter;
		char *name;
		dir = opendir(".");//open current dir to view
		while((myfile = readdir(dir)) != NULL){
			stat(myfile->d_name, &mystat);
			name = myfile->d_name;
			if(S_ISREG(mystat.st_mode)){
			FILE *fptr;
			fptr = fopen(name, "r");//open file
			getter = fgetc(fptr);//collect contents from the file
			while(getter != EOF){
				printf("%d", getter);//print character
				getter = fgetc(fptr);//continue file
			}
			fclose(fptr);
			printf("\n");//keep things pretty
			}
		}
		closedir(dir);
}
int main(int argc, char *argv[]){
	
	if(argc < 1){
		printf("not enough arguments");
		return 1;
	}
	//ls with -l
	if(argc == 4){
		//stuff();
	}//good enough

	//ls into a directory
	else if(argv[2] != "-l" || argv[2] != "|" && argc > 1) {
		//directory(argv[2]);
	}//done

	//ls | cat
	else if (argv[2] == NULL || argv[2] == "|"){//this works but its really weird get other tests
		//lsCAT();
	}//done?

	//standard ls
	else if (argc == 2) {
		//ls();
	}//done

	//cat ($ls)
	else{
		cat();
	}

	return 0;
}
