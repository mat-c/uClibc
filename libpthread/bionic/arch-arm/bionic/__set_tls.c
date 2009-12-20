#include <unistd.h>
#include <sys/syscall.h>

int __set_tls(void *ptr)
{
	return INLINE_SYSCALL(_ARM_set_tls, ptr);
}
