#include <stdlib.h>
#include "light.h"


/*
 *	demonstrate multiple malloc descent.
 */
struct test_struct
{
	int test;
};

int main(int argc, char *argv[])
{
	struct test_struct *p;
	/*
	 *	void *calloc(size_t numitems, size_t size);
	 *		return pointer to allocated memory on success, or NULL on error
	 */
	p = calloc(1000, sizeof(struct test_struct)); 
	if (p == NULL)
		err_exit("calloc");
	free(p);


	/*
	 *	void *memalign(size_t boundary, size_t size);
	 *		return pointer to allocated memory on success, or NULL on error
	 */
	p = memalign(4096, sizeof(struct test_struct));

	if (p == NULL)
		err_exit("memalign");
	free(p);

	exit(EXIT_SUCCESS);
	
}