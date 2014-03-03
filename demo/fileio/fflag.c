#include <fcntl.h>
#include "light.h"

void printff(int flag, int bit, const char *s)
{
	if (flag & bit)
		printf("\t	%s\n", s);
}

/*
 *	usage:
 *		$ touch tfile 
 *		$ ./fflag tfile a 	//print file access mode
 *	
 */
int main(int argc, char *argv[])
{

	int fd, flags, access_mode;

	if (argc < 3 || strcmp(argc[1], "--help") == 0)
		usage_err("%s file {f | a}\n", argv[0]);

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		err_exit("open");

	flags = fcntl(fd, F_GETFL);
	if (flags == -1)
		err_exit("fcntl");

	switch(argv[2][0])
	{
		case 'f':
			printf("%s: status flags\n", argv[0]);
			printff(flags, O_APPEND, 	"O_APPEND");
			printff(flags, O_ASYNC,  	"O_ASYNC");
			printff(flags, O_DSYNC,  	"O_DSYNC");
			printff(flags, O_NONBLOCK,	"O_NONBLOCK");
			printff(flags, O_SYNC,  	"O_SYNC");
			break;
		case 'a':
			printf("%s: access mode\n", argv[0]);
			access_mode = flags & O_ACCMODE;
			if (access_mode == O_WRONLY)
				printf("\t	O_WRONLY");
			else if (access_mode == O_RDONLY)
				printf("\t	O_RDONLY");
			else if (access_mode == O_RDWR)
				printf("\t	O_RDWR");
			printf("\n");
			break;
		default:
			cmd_line_err("Argument must be [f|a]: %s\n", argv[2]);
	}

	if (close(fd) == -1)
		err_exit("close file");
	exit(EXIT_SUCCESS);
}