#ifndef AZ_UTILITIES_NUMBERS_H
#define AZ_UTILITIES_NUMBERS_H

#define __need_size_t
#include <stddef.h>

size_t numbers_countDigits(size_t number);

void numbers_toString(size_t number, size_t length, char* string);

#endif // AZ_UTILITIES_NUMBERS_H
