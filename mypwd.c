#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>

void get_inode(const char *path, ino_t *inode)
{
	struct stat info;
	if(lstat(path,&info)!=0){
		perror("lstat");
		exit(1);
	}
	*inode=info.st_ino;
}

void get_dir_name(ino_t inode,const char *path,char *name,size_t len){
	DIR *dir;
	struct dirent *entry;

	if((dir=opendir(path))==NULL){
		perror("opendir");
		exit(1);
	}
	while((entry=readdir(dir))!=NULL){
		if(entry->d_ino == inode){
			strncpy(name,entry->d_name,len);
			name[len-1]='\0';
			closedir(dir);
			return;
		}
	}
	closedir(dir);
	fprintf(stderr,"Error:Directory name not found\n");
	exit(1);
}
void print_path(ino_t current_inode){
	char path[PATH_MAX];
	char name[NAME_MAX];
	ino_t parent_inode;

	if(current_inode ==0){
		fprintf(stderr,"Error:current inode is 0\n");
		exit(1);
	}
	path[0]='\0';
	while(1){
		get_inode("..",&parent_inode);
		chdir("..");

		if(parent_inode == current_inode){
			break;
		}

		get_dir_name(current_inode, ".",name,sizeof(name));
		current_inode=parent_inode;
		size_t len = strlen(path);
		memmove(path + strlen(name) +1,path,len+1);
		memcpy(path,"/",1);
		memcpy(path+1,name,strlen(name));
	}

	printf("/%s\n",path);
}
int main(int agrc,char *argv[]){
	ino_t current_inode;
	get_inode(".",&current_inode);
	print_path(current_inode);

	return 0;
}





