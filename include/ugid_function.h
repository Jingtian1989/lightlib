#ifndef _UGID_FUNCTION_H_
#define _UGID_FUNCTION_H_

char *get_user_name_from_id(uid_t uid);

uid_t get_user_id_from_name(const char *name);

char *get_group_name_from_id(gid_t gid);

gid_t get_group_id_from_name(const char *name);

#endif

