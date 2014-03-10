#include <dlfcn.h>
#include "light.h"


int main(int argc, char *argv)
{
	void *lib_handle;
	void (*funcp)(void);
	char *err;

	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		usage_err("%s lib-path func-name\n", argv[0]);

	/* Load the shared library and get handle for later use */
	lib_handle = dlopen(argv[1], RTLD_LAZY);
	if (lib_handle == NULL)
		fatal("dlopen: %s", dlerror());

	/* Search libaray for symbol named in argv[2] */
	dlerror();
	funcp = dlsym(lib_handle, argv[2]);
	err = dlerror();
	if (err != NULL)
		fatal("dlsym: %s", err);
	/* If the address returned by dlsym() is non-NULL, try calling it as a function 
	 * that takes no arguments.
	 */

	 if (funcp == NULL)
	 	printf("%s is NULL\n", argv[2]);
	 else
	 	funcp();

	 dlclose(lib_handle);
	 exit(EXIT_SUCCESS);
}