#include "platform.h"
#include "string.h"
#include <sys/stat.h>

int _fstat(int file, struct stat* st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _open(const char* name, int flags, int mode)
{
    return -1;
}

int _write(int file, char* c, int len)
{
    for (int i = 0; i < len; ++i) {
        while (UART0.txdata < 0)
            ;
        UART0.txdata = c[i];
    }
    return len;
}

int _read(int file, char* c, int len)
{
    return -1;
}

char* _sbrk(int incr)
{
    extern char _heap; /* Defined by the linker */
    extern char _eheap; /* Defined by the linker */
    static char* heap_ptr = &_heap;
    if (heap_ptr + incr <= &_eheap) {
        char* base = heap_ptr;
        heap_ptr += incr;
        return base;
    }
    return (char*)-1;
}
