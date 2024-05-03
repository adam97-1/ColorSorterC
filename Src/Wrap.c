#include "Wrap.h"

void	__wrap_free (void *s)
{
	memset(s, 0xAA, malloc_usable_size(s));
	__real_free(s);
}
