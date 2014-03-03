#include <stdarg.h>
#include "light.h"
#include "error_name.h"
#include "error_function.h"


static void terminate(boolean use_exit3)
{

	char *s;
	/*
 	 *	dump core if EF_DUMPCORE environment variable is defined and is 
 	 *	a nonempty string; otherwise call exit(3) or _exit(2), depending
 	 *	on the value of 'use_exit3'
	 */

	 s = getenv("EF_DUMPCORE");

	 if (s != NULL && *s != '\0')
	 	abort();
	 else if(use_exit3)
	 	exit(EXIT_FAILURE);
	 else
	 	_exit(EXIT_FAILURE);
}

static void output_err(boolean use_err, int err, boolean flush_stdout, const char *format, va_list ap)
{
#define BUF_SIZE 500
	char buf[BUF_SIZE], user_msg[BUF_SIZE], err_text[BUF_SIZE];
	
	vsnprintf(user_msg, BUF_SIZE, format, ap);

	if (use_err)
		snprintf(err_text, BUF_SIZE, "[%s %s]", (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKOWN?", strerror(err));
	else
		snprintf(err_text, BUF_SIZE, ":");

	if (flush_stdout)
		fflush(stdout);
	fputs(buf, stderr);
	fflush(stderr);
}

void err_msg(const char *format)
{
	va_list arglist;
	int saved_errno = errno;

	va_start(arglist, format);
	output_err(true, errno, true, format, arglist);
	va_end(arglist);

	errno = saved_errno;

}

void err_exit(const char *format, ...)
{
	va_list arglist;

	va_start(arglist, format);
	output_err(true, errno, true, format, arglist);
	va_end(arglist);

	terminate(true);
}

void _err_exit(const char *format, ...)
{
	va_list arglist;

	va_start(arglist, format);
	output_err(true, errno, false, format, arglist);
	va_end(arglist);

	terminate(false);

}

void err_exit_en(int errnum, const char *format, ...)
{
	va_list arglist;

	va_start(arglist, format);
	output_err(true, errnum, true, format, arglist);
	va_end(arglist);

	terminate(true);
}

void fatal(const char *format, ...)
{
	va_list arglist;

	va_start(arglist, format);
	output_err(false, 0, true, format, arglist);
	va_end(arglist);

	terminate(true);
}

void usage_err(const char *format, ...)
{
	va_list arglist;

	fflush(stdout);
	fprintf(stderr, "Usage : ");
	va_start(arglist, format);
	vfprintf(stderr, format, arglist);
	va_end(arglist);

	fflush(stderr);
	exit(EXIT_FAILURE);
}


void cmd_line_err(const char *format, ...)
{
	va_list arglist;

	fflush(stdout);
	fprintf(stderr, "Command-line usage error : ");
	va_start(arglist, format);
	vfprintf(stderr, format, arglist);
	va_end(arglist);
	fflush(stderr);
	exit(EXIT_FAILURE);
}

