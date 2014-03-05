#include <pwd.h>
#include <grp.h>
#include "ugid_function.h"             
#include "light.h"

int main(int argc, char *argv[])
{
    uid_t uid;
    gid_t gid;
    int j;
    boolean err_fnd;
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usage_err("%s owner group [file...]\n"
                "        owner or group can be '-', "
                "meaning leave unchanged\n", argv[0]);

    if (strcmp(argv[1], "-") == 0) {            /* "-" ==> don't change owner */
        uid = -1;
    } else {                                    /* Turn user name into UID */
        uid = get_user_id_from_name(argv[1]);
        if (uid == -1)
            fatal("No such user (%s)", argv[1]);
    }
    if (strcmp(argv[2], "-") == 0) {            /* "-" ==> don't change group */
        gid = -1;
        } else {                                    /* Turn group name into GID */
        gid = get_group_id_from_name(argv[2]);
        if (gid == -1)
            fatal("No group user (%s)", argv[1]);
    }
    /* Change ownership of all files named in remaining arguments */
    err_fnd = false;
    for (j = 3; j < argc; j++) {
        if (chown(argv[j], uid, gid) == -1) {
            err_msg("chown: %s", argv[j]);
            err_fnd = true;
        }
    }
    exit(err_fnd ? EXIT_FAILURE : EXIT_SUCCESS);
}