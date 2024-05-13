#include <string.h>
#include <malloc.h>

void __real_free(void *s);
void __wrap_free (void *s)
{

	memset(s, 0xAA, malloc_usable_size(s));
	__real_free(s);
}
