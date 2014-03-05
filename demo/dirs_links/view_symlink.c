#include <sys/stat.h>
#include <limits.h>
#include "light.h"

#define BUF_SIZE PATH_MAX

/*
 *	usage:
 *		$ pwd                                   	Where are we?
 *		/home/mtk
 *		$ touch x                                   Make a file
 *		$ ln -s x y                                 and a symbolic link to it
 *		$ ./view_symlink y 
 *		readlink: y --> x
 *		realpath: y --> /home/mtk/x
 *
 */
int main(int argc, char *argv[])
{
	struct stat statbuf;
	char buf[BUF_SIZE];
	ssize_t num_bytes;

	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s pathname\n", argv[0]);

	if (lstat(argv[1], &statbuf) == -1)
		err_exit("lstat");

	if (!S_ISLNK(statbuf.st_mode))
		fatal("%s is not a symbolic link", argv[1]);

	num_bytes = readlink(argv[1], buf, BUF_SIZE-1);
	if (num_bytes == -1)
		err_exit("readlink");
	buf[num_bytes] = '\0';
	printf("readlink: %s --> %s\n", argv[1], buf);

	if (realpath(argv[1], buf) == NULL)
		err_exit("realpath");
	printf("realpath: %s --> %s\n", argv[1], buf);

	exit(EXIT_SUCCESS);

}