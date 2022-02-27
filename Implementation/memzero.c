#include <util.h>

void memzero(void *const pnt, const size_t len) {
    _memset(pnt, 0x0, len);
}