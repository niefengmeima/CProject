#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(dir)) == NULL) //只能使用绝对路径名和相对路径名打开，如果使用文件名，那么这个文件必须在当前的工作目录下。
	{								 // 比如 hi/abc/  要想通过dir="abc"打开abc目录，则当前的工作目录必须在hi目录中。也说明了entry=>d_name是一个不包含路径的文件名。
		fprintf(stderr, "cannot open directory: %s\n", dir);
		return;
	}
	chdir(dir); //必须要改变当前工作目录，因为enttry->d_name是文件名，而不是全路径名。不改变当前目录的话，他就只会开启第一层目录而已，剩下的都不开。
	while ((entry = readdir(dp)) != NULL)
	{
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
				continue; //linux中目录项会保留.和..两个目录项，如果不把它们忽略掉，会无限递归。
			printf("%*s%s/\n", depth, "", entry->d_name);
			printdir(entry->d_name, depth + 4);
		}
		else
			printf("%*s%s\n", depth, "", entry->d_name);
	}
	chdir("..");
	closedir(dp);
}

int main(int argc, char *argv[])
{
	char *topdir = ".";
	if (argc >= 2)
		topdir = argv[1];

	printf("Directory scan of %s\n", topdir);
	printdir(topdir, 0);
	printf("done.\n");

	exit(0);
}