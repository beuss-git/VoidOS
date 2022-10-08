#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H
typedef int pid_t;
#endif
/* errno.h: */
/* string.h: */
#ifndef _STRING_H
#define _STRING_H
#include <stddef.h>
#ifdef __cplusplus
extern "C"
{
#endif
    void *memcpy(void *, const void *, size_t);
    void *memset(void *, int, size_t);
    char *strcpy(char *, const char *);
    size_t strlen(const char *);

    // This is a hack for compiling GCC!
    typedef int intptr_t;
#ifdef __cplusplus
}
#endif
#endif