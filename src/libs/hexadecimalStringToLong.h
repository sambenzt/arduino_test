#include <string.h>

#ifndef hexadecimalStringToLong_h
#define hexadecimalStringToLong_h

long hexadecimalStringToLong(String recv) {
    char c[recv.length() + 1];
    recv.toCharArray(c, recv.length() + 1);
    return strtol(c, NULL, 16); 
}

#endif /*hexadecimalStringToLong_h*/