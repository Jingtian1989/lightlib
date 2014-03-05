#define _BSD_SOURCE /* Get major() and minor() from <sys/types.h> */
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "file_perms.h"
#include "light.h"

/*
 *	usage:
 *		$ echo 'All operating systems provide services for programs they run' > apue
 *		$ chmod g+s apue        Turn on set-group-ID bit; affects last status change time
 *		$ cat apue              Affects last file access time
 *		All operating systems provide services for programs they run
 *		$ ./t_stat apue
 *		File type:                regular file
 *		Device containing i-node: major=3   minor=11
 *		I-node number:            234363
 *		Mode:                     102644 (rw-r--r--)
 *  	 	special bits set:     set-GID
 *		Number of (hard) links:   1
 *		Ownership:                UID=1000   GID=100
 *		File size:                61 bytes
 *		Optimal I/O block size:   4096 bytes
 *		512B blocks allocated:    8
 *		Last file access:         Mon Jun  8 09:40:07 2011
 *		Last file modification:   Mon Jun  8 09:39:25 2011
 *		Last status change:       Mon Jun  8 09:39:51 2011
 *
 */

static void display_stat_info(const struct stat *sb)
{
	printf("File type:			");

	switch(sb->st_mode & S_IFMT)
	{
		case S_IFREG:
			printf("regular file\n");
			break;
		case S_IFDIR:
			printf("directory\n");
			break;
		case S_IFCHR:
			printf("character device\n");
			break;
		case S_IFBLK:
			printf("block device\n");
			break;
		case S_IFLNK:
			printf("symbolic (soft) link\n");
			break;
		case S_IFIFO:
			printf("FIFO or pipe\n");
			break;
		case S_IFSOCK:
			printf("socket\n");
			break;
		default:
			printf("unkown file type?\n");
			break;
	}
	printf("Device containing i-node: major=%ld minor=%ld\n",
		(long)major(sb->st_dev), (long)minor(sb->st_dev));
	printf("I-node number:		%ld\n", (long)sb->st_ino);

	printf("Mode:				%lo (%s)\n", 
		(unsigned long)sb->st_mode, file_perm_str(sb->st_mode, 0));

	if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX))
		printf("	special bits set: 	%s%s%s\n",
			(sb->st_mode & S_ISUID) ? "set-UID" : "",
			(sb->st_mode & S_ISGID) ? "set-GID" : "",
			(sb->st_mode & S_ISVTX) ? "sticky"  : ""
			);

	printf("Ownership:		UID=%ld	GID=%ld\n",
		(long) sb->st_uid, (long)sb->st_gid);

	if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode))
		printf("Device number (st_rdev): major=%ld; minor=%ld\n",
			(long)major(sb->strdev), (long)minor(sb->st_rdev));

	printf("File size:				%lld bytes\n", (long long) sb->st_size);
	printf("Optimal I/O block size:	%ld bytes\n", (long)sb->st_blksize);

	printf("512B blocks allocated:	%lld\n", (long long)sb->st_blocks);
	printf("Last file access:		%s\n", ctime(&sb->st_atime));
	printf("Last file modification: %s\n", ctime(&sb->st_mtime));
	printf("Last status change:		%s\n", ctime(&sb->st_ctime));

}

int main(int argc, char *argv[])
{
	struct stat *sb;
	boolean stat_link;
	int fname;

	stat_link = (argc > 1) && strcmp(argv[1], "-l") == 0;

	fname = stat_link ? 2 : 1;

	if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
		usage_err("%s [-l] file\n"
			" -l = use lstat() instead of stat()\n", argv[0]);

	if (stat_link)
	{
		if (lstat(argv[fname], &sb) == -1)
			err_exit("lstat");
	}else
	{
		if (stat(argv[fname], &sb) == -1)
			err_exit("stat");
	}

	display_stat_info(&sb);
	exit(EXIT_SUCCESS);
}