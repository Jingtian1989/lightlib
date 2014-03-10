#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include "light.h"

int main(int argc, char *argv[])
{
	int sig, num_sigs, j, sig_data;
	union sigval sv;

	if (argc < 4 || strcmp(argv[1], "--help") == 0)
		usage_err("%s pid sig-num data [num-sigs]\n", argv[0]);

	/* Display our pid and uid, so that they can be compared with the correspoing
	 * fields of the siginfo_t argument supplied to the handler in the receiving
	 * process.
	 */

	 printf("%s: PID is %ld, UID is %ld\n", argv[0], (long)getpid(), (long)getuid());

	 sig = get_int(argv[2], 0, "sig-num");
	 sig_data = get_int(argv[3], GN_ANY_BASE, "data");
	 num_sigs = (argc > 4) ? get_int(argv[4], GN_GT_0, "num-sigs") : 1;

	 for (j = 0; j < num_sigs; j++)
	 {
	 	sv.sival_int = sig_data + j;
	 	if (sigqueue(get_long(argv[1], 0, "pid"), sig, sv) == -1)
	 		err_exit("sigqueue %d", j);
	 }

	 exit(EXIT_SUCCESS);
}