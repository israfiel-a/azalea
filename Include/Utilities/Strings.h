#ifndef AZ_COMPILER_UTILITIES_STRINGS_H
#define AZ_COMPILER_UTILITIES_STRINGS_H

#define __need_size_t
#include <stddef.h>

bool strings_compare(const char* const a, const char* const b);

bool strings_compareN(const char* const a, const char* const b, size_t number);

bool strings_compareInsensitive(const char* const a, const char* const b);

size_t strings_getLength(const char* const string);

char strings_toLower(char character);

void strings_concatenate(char *storage, const char* const format, ...);

void strings_concatenateN(char *storage, const char *const format, ...);

#endif  // AZ_COMPILER_UTILITIES_STRINGS_H
