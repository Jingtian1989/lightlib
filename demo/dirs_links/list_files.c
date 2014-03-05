#include <dirent.h>
#include "light.h"

static void list_files(const char *dirpath)
{
	DIR *dirp;
	struct dirent *dp;
	boolean is_current; /* true if 'dirpath' is "." */

	is_current = strcmp(dirpath, '.') == 0;

	dirp = opendir(dirpath);
	if (dirp == NULL)
	{
		err_msg("opendir failed on '%s'", dirpath);
		return;
	}

	/* For each entry in this directory, print directory + filename */
	for (;;)
	{
		errno = 0;
		dp = readdir(dirp);
		if (dp == NULL)
			break;
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue; 	/* Skip . and .. */
	
		if (!is_current)
			printf("%s/", dirpath);
		printf("%s\n", dp->d_name);
	}

	if (errno != 0)
		err_exit("readdir");

	if (closedir(dirp) == -1)
		err_msg("closedir");

}

int main(int argc, char *argv[])
{
	if (argc > 1 && strcmp(argv[1], "--help") == 0)
		usage_err("%s [dir...]\n", argv[0]);

	if (argc == 1) /* No arguments - use current directory */
		list_files('.');
	else
		for (argv++; *argv; argv++)
			list_files(*argv);

	err_exit(EXIT_SUCCESS);

}