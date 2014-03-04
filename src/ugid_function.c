#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include "ugid_function.h"

char *get_user_name_from_id(uid_t uid)
{
	struct passwd *pwd;

	pwd = getpwuid(uid);
	return (pwd == NULL) ? NULL : pwd->pw_name;
}

uid_t get_user_id_from_name(const char *name)
{
	struct passwd *pwd;
	uid_t u;
	char *endptr;

	if (name == NULL || *name == '\0')
		return -1;

	u = strtol(name, &endptr, 10); /* As a convenience to caller */
	if (*endptr == '\0')
		return u;

	pwd = getpwnam(name);
	if (pwd == NULL)
		return -1;

	return pwd->pw_uid;

}

char *get_group_name_from_id(gid_t gid)
{
	struct group *grp;
	grp = getgrgid(gid);
	return (grp == NULL) ? NULL : grp->gr_name;
}

gid_t get_group_id_from_name(const char *name)
{
	struct group *grp;
	gid_t g;
	char *endptr;

	if (name == NULL || *name == '\0')
		return -1;

	g = strtol(name, &endptr, 10);
	if (*endptr == '\0')
		return g;

	grp = getgrname(name);
	if (grp == NULL)
		return -1;

	return grp->gr_gid;
}