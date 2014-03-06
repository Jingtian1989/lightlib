#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include "light.h"

static sig_segv_handler(int sig)
{
	int x;

	/* UNSAFE: This handler uses non-async-signal-safe functions (printf(), strsignal(), fflush();) */

	printf("Caught signal %d (%s)\n", sig, strsignal(sig));
	printf("Top of handler stack near	%10p\n", (void *)&x);
	fflush(NULL);

	_exit(EXIT_FAILURE);	/*	Can't return after SIGSEGV */
}

static void over_flow_stack(int call_num)
{
	char a[100000]; /* Make this stack frame large */

	printf("Call %4d - top of stack near %10p\n", call_num, &a[0]);
	over_flow_stack(call_num+1);
}


int main(int argc, char *argv[])
{
	stack_t sigstack;
	struct sigaction sa;
	int j;

	printf("Top of standard stack is near %10p\n", (void *)&j);

	/* Allocate alternate stack and inform kernel of its existence */
	sigstack.ss_sp = malloc(SIGSTKSZ);
	if (sigstack.ss_sp == NULL)
		err_exit("malloc");

	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	if (sigstack(&sigstack, NULL) == -1)
		err_exit("sigstack");
	printf("Alternate stack is at 	%10p-%p\n", sigstack.ss_sp, (char *)sbrk(0)-1);

	sa.sa_handler = sig_segv_handler; 	/* Establish handler for SIGSEGV */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_ONSTACK; 	/* Handler uses alternate stack */
	if (sigaction(SIGSEGV, &sa, NULL) == -1)
		err_exit("sigaction");

	over_flow_stack(1);
}