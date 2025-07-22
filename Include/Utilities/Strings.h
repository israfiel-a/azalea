#ifndef AZ_UTILITIES_STRINGS_H
#define AZ_UTILITIES_STRINGS_H

#define __need_size_t
#include <stddef.h>

bool strings_compare(const char* const a, const char* const b);

bool strings_compareN(const char* const a, const char* const b, size_t number);

bool strings_compareInsensitive(const char* const a, const char* const b);

size_t strings_getLength(const char* const string);

char strings_toLower(char character);

#endif  // AZ_UTILITIES_STRINGS_H
