#ifndef _ERR_FUNC_H_
#define _ERR_FUNC_H_

void err_msg(const char *format, ...);

/*
 *	err_exit operates like err_msg, but also terminates the programs,
 *	either by calling exit() or, if the environment variables EF_DUMPCORE
 *	is defined with a nonempty string value, by calling abort() to produce
 *	a core dump file for use with debugger.
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

void err_exit_en(int errnum, const char *format, ...);

/*
 *	fatal function is used to diagnose general errors, including errors from 
 *	library functions that don't set errno. Its argument list is the same as 
 *	for printf, except that a terminating newline character is automatically 
 *	appended to the output string. It prints the formatted output on standard
 *	error and the terminate the program as with err_exit.
 */

void fatal(const char *format, ...);


/*
 *	usage_err function is used to diagnose errors in command-line argument
 *	usage. It takes an argument list in the style of printf and prints the
 *	string Usage: followed by the formatted output on standard error, and 
 *	then terminates the program by calling exit.
 */
void usage_err(const char *format, ...);

/*
 *	cmd_line_err function is similar to usage_err, but is intended for diagnosing 
 *	errors in the command-line arguments specified to program.
 */
void cmd_line_err(const char *format, ...);

#endif