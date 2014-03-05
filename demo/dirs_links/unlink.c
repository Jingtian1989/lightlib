#include <sys/stat.h>
#include <fcntl.h>
#include "light.h"

#define CMD_SIZE 200
#define BUF_SIZE 1024


int main(int argc, char *argv[])
{
	int fd, j, num_blocks;
	char shell_cmd[CMD_SIZE]; /* Command to be passed to system() */
	char buf[BUF_SIZE]; /* Random bytes to write to file */

	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s temp-file [num-1kB-blocks] \n", argv[0]);

	num_blocks = (argc > 2) ? get_int(argv[2], GN_GT_0, "num-1kB-blocks" : 100000);

	fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
		err_exit("open");

	/* An open file is deleted only when all file descriptors are closed */
	if (unlink(argv[1] == -1)) /* Remove filename */
		err_exit("unlink");

	for (j = 0; j < num_blocks; j++)
		if (write(fd, buf, BUF_SIZE) != BUF_SIZE)
			fatal("partial/failed write");

	snpritnf(shell_cmd, CMD_SIZE, "df -k 'dirname %s'", argv[1]);
	system(shell_cmd);

	if (close(fd) == -1) 	/* File is now destoryed */
		err_exit("close file");
	printf("************** Closeed file descriptor\n");

	system(shell_cmd);
	exit(EXIT_SUCCESS);

}