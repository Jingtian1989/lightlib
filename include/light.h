#ifndef _LIGHT_HDR_H_
#define _LIGHT_HDR_H_

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "error_name"
#include "error_function.h"
#include "get_number.h"

typedef enum {false, true} boolean;

#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#endif

