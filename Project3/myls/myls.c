#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
	DIR *dir;
	struct dirent *myfile;
	struct stat mystat;
	if(argc < 1){
		printf("not enough arguments");
		return 1;
	}
	//ls with -l
	if(argv[2] == "-l"){
		dir = opendir(get_current_dir_name());
		while((myfile = readdir(dir)) != NULL){
			stat(myfile->d_name, &mystat);
			if(myfile->d_name == argv[3]){//only print one file
				printf("%s", mystat.st_mode);//file permision
				printf("%s", mystat.st_uid);//user id name
				printf("%s", mystat.st_gid);//group id
				printf("%d", mystat.st_size);//size
				printf("%s", mystat.st_mtime);//last modified
				printf("%s", myfile->d_name);//name of file
			}		
		}
	}
	//ls into a directory
	else if(argv[2] != "-l" || argv[2] != "|" && argc > 1) {
		int count = 0;//helps keep the print out pretty
		dir = opendir(argv[2]);//open dir to view
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
	//ls | cat
	else if (argv[2] == "|"){
		dir = opendir(get_current_dir_name());//open current dir to view
		while((myfile = readdir(dir)) != NULL){
			stat(myfile->d_name, &mystat);
			printf("%s\n", myfile->d_name);
		}
		closedir(dir);
	}
	//standard ls
	else if (argc == 2) {
		int count = 0;//helps keep the print out pretty
		dir = opendir(get_current_dir_name());//open current dir to view
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
	//cat ($ls)
	else{
		printf("cat$ (ls)");
	}
	return 0;
}
