#ifndef _CREATE_PID_FILE_H_
#define _CREATE_PID_FILE_H_

#define CPF_CLOEXEC 01

int create_pid_file(const char *prog_name, const char *pid_file, int flags);

#endif 