#ifndef AZ_COMPILER_UTILITIES_OUTPUT_H
#define AZ_COMPILER_UTILITIES_OUTPUT_H

#define __need_size_t
#include <stddef.h>

void output_string(const char *const string, size_t size, bool newline);

#endif // AZ_COMPILER_UTILITIES_OUTPUT_H
