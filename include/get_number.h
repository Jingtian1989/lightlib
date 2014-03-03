#ifndef _GET_NUM_H_
#define _GET_NUM_H_

#define GN_NONEG 01 /* value must be >= 0 */
#define GN_GT_0  02


#define GN_ANY_BASE 0100 
#define GN_BASE_8 	0200
#define GN_BASE_16 	0400

long get_long(const char *arg, int flags, const char *name);

int get_int(const char *arg, int flags, const char *name);

#endif

