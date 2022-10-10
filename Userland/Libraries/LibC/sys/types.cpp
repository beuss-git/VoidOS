#include <sys/types.h>


void* memcpy(void* dst, const void* src, size_t num) {
    char* dest = (char*)dst;
    const char* source = (const char*)src;
    while (num) {
        *(dest++) = *(source++);
        num--;
    }
    return dest;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}
