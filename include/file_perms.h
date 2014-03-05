#ifndef _FILE_PERMS_H_
#define _FILE_PERMS_H_

#define STR_SIZE sizeof("rwxrwxrwx")
#define FP_SPECIAL 1

char *file_perm_str(mode_t perm, int flags);

#endif