#ifndef _ERR_FUNC_H_
#define _ERR_FUNC_H_

void err_msg(const char *format, ...);

/*
 *	err_exit operates like err_msg, but also terminates the programs,
 *	either by calling exit() or, if the environment variables EF_DUMPCORE
 *  is defined with a nonempty string value, by calling abort() to produce
 *  a core dump file for use with debugger.
 */

void err_exit(const char *format, ...);

/*
 *	different from the err_exit in:
 *
 *	1.It doesn't flush standard output before printing the error message.
 *	2.It terminates the process by calling _exit instead of exit(). This 
 *	  causes the process to terminate without flushing stdio buffers or
 *	  invoking exit handler. _err_exit is especially useful if we write a 
 *	  library function that crates a child process that needs to terminate 
 *	  because of an error. This termination should occur without flushing
 *	  the child's copy of the parent's stdio buffers and without invoking 
 *	  exit handlers established by the parent.
 */
void _err_exit(const char *format, ...);

/*
 *	err_exit_en function is the same as err_exit, except that instead of 
 *	printing the error text corresponding to the current value of errno, it
 *	prints the text corresponding to the error number given in the argument
 *	errnum.
 *	Mainly, we use err_exit_en in programs that empoly the posix threads api.
 *	Unlike traditional unix system calls, which return -1 on error, the posix
 *	threads functions diagnose an error by returning an error number as their
 *	function result.
 */

void err_exit_en(const char *format, ...);

void fatal(const char *format, ...);

void usage_err(const char *format, ...);

void cmd_line_err(const char *format, ...);

#endif