#ifndef AZ_COMPILER_UTILITIES_NUMBERS_H
#define AZ_COMPILER_UTILITIES_NUMBERS_H

#define __need_size_t
#include <stddef.h>

size_t numbers_countDigits(size_t number);

void numbers_toString(size_t number, size_t length, char *string);

void numbers_fromString(char *string, size_t length, size_t *number);

#endif // AZ_COMPILER_UTILITIES_NUMBERS_H
