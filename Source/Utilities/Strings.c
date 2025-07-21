#include <Utilities/Strings.h>

bool strings_compare(const char* const a, const char* const b) {
    char *av = (char*)a, *bv = (char*)b;
    while (*av == *bv) {
        if (*av == 0) return true;
        av++;
        bv++;
    }
    return false;
}

bool strings_compareN(const char* const a, const char* const b,
                            size_t number) {
    char *av = (char*)a, *bv = (char*)b;
    size_t checked = 0;
    while (*av == *bv) {
        if (*av == 0 || checked == number) return true;
        av++;
        bv++;
        checked++;
    }
    return false;
}

size_t strings_getLength(const char* const string) {
    char* stringPointer = (char*)string;
    while (*stringPointer++ != 0) __asm("");
    return stringPointer - string;
}

