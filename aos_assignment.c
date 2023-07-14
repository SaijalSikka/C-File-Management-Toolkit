#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
void create_file(char *path, char *permissions){   //for creating a new file , if a file already exists and we try creating it , it will create a new file replacing the old file
	int x=0;
	sscanf(permissions,"%o",&x);
	int fd=creat(path,x);    //giving read, write and execute permissions to user, group and others for newly created file
	if(fd==-1){
		printf("\nError in creating file\n");
		return;
	}
	printf("\nFile created successfully\n");
	int dummy=close(fd);
	if(dummy==-1)
	printf("\nError in closing the file\n");
	printf("\nFile closed successfully\n");
}
void read_file(char*path,ssize_t nbytes,off_t offset){   //reading nbytes from a file from a particular offset
	int fd=open(path,O_RDONLY);
	if(fd==-1){
		printf("\nError in opening file\n");
		return;
	}
	printf("\nFile opened successfully\n");
	if(lseek(fd,offset,SEEK_SET)==-1)   //setting offset of a file
	printf("\nError in setting file offset\n");
	char buff[1024];
	ssize_t s=read(fd,buff,nbytes);  //reads nbytes from file to buff
	if(s==-1){
		printf("\nError in reading file\n");
		int dummy=close(fd);
		if(dummy==-1)
		printf("\nError in closing the file\n");
		return;
	}
	buff[s] = '\0';    //terminating the character array
    printf("\nRead %zd bytes from file: %s\n", s, buff);
    int dummy = close(fd);
    if (dummy == -1)
        printf("\nError in closing the file\n");
}
void write_file(char *path1,ssize_t nbytes,off_t offset){   //writes from file path1 to file path2 where file path1 offset is set to offset and no of bytes to be read is nbytes
	int fd1=open(path1,O_WRONLY|O_CREAT);
	if(fd1==-1){
		printf("\nError in Opening file 1");
		return;
	}
	printf("\nFile 1 opened successfully\n");
	if(lseek(fd1,offset,SEEK_SET)==-1)           //setting file1 offset
	printf("\nError in setting file offset\n");
	char buffer[1024];
	ssize_t numread=read(0,buffer,nbytes);
	ssize_t num_write=write(fd1,buffer,nbytes);
	int dummy1 = close(fd1);
    if (dummy1 == -1)
        printf("\nError in closing the file 1\n");
    printf("File 1 closed successfully\n");
}
void display_info(char *path)
{
    struct stat fileStat;
    int fd = stat(path, &fileStat);
	
    //error message
	
    if(fd<0)
    {
        printf("\nError in accessing the info regarding file\n");
        return;
    }
    printf("Statistical Information of %s\n", path);
    printf("\nFile Size: \t\t%li bytes\n", fileStat.st_size);
    printf("Number of Links: \t%lu\n", fileStat.st_nlink);
    printf("File inode: \t\t%lu\n", fileStat.st_ino);

    printf("File Permissions: \t");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
	
    printf("\n");
}
void copy_file(char *src_path, char *dst_path) {      //copy source file to destination file
    int fd1 = open(src_path, O_RDONLY);
    if (fd1 == -1) {
        printf("\nError opening source file\n");
        return;
    }
	printf("\nSource file opened successfully\n");
    int fd2 = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);     //if dest file already exists , overwrite content from source file to dest file, if dest file doesnt exist create new file and then copy content
    if (fd2 == -1) {
        printf("\nError opening destination file\n");
        int dummy2=close(fd2);
		if(dummy2==-1)
		printf("\nError in closing the file\n");
        return;
    }
	printf("Dest file opened successfully\n");
    char buf[1024];
    ssize_t num_read;
    while ((num_read = read(fd1, buf, sizeof(buf))) > 0) {
        if (write(fd2, buf, num_read) != num_read) {
            printf("\nError writing to destination file\n");
        int dummy1=close(fd1);
		int dummy2=close(fd2);
		if(dummy1==-1)
		printf("\nError in closing the file\n");
		if(dummy2==-1)
		printf("\nError in closing the file\n");
        return;
        }
    }

    if (num_read == -1) {
        printf("\nError reading from source file\n");
    }
    int dummy1 = close(fd1);
    if (dummy1 == -1)
        printf("\nError in closing the source file\n");
    printf("Source File closed successfully\n");
	int dummy2 = close(fd2);
    if (dummy2 == -1)
        printf("\nError in closing the dest file\n");
    printf("Dest File closed successfully\n");
}
int main(int argc,char **argv){
	if(argc<3)
	{
    	printf("\nNumber of command line arguments are too less\n");
    	return 0;
	}
	// values for argv[1] can be create for first func, read for second, write for third, info for fourth and copy for fifth
	if(argc==3){
		if(strcmp(argv[1],"info")==0){
    		display_info(argv[2]);
		}
		else if(strcmp(argv[1],"create")==0){
    		create_file(argv[2],"0777");
    	}
		else{
			printf("\nError\n");
		}
	}
	else if(argc==4){
		if(strcmp(argv[1],"copy")==0){
			copy_file(argv[2],argv[3]);
		}
		else if(strcmp(argv[1],"create")==0){
    		create_file(argv[2],argv[3]);
    	}
		else{
			printf("\nError\n");
		}
	}
	else if (argc==5){
		if(strcmp(argv[1],"read")==0){
		    read_file(argv[2],atoi(argv[3]),atoi(argv[4]));
		}
		else if(strcmp(argv[1],"write")==0){
			write_file(argv[2],atoi(argv[3]),atoi(argv[4]));
		}
	}
	else{
		printf("\nError\n");
	}
	return 0;
}
