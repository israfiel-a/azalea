#ifndef AZ_UTILITIES_OUTPUT_H
#define AZ_UTILITIES_OUTPUT_H

#define __need_size_t
#include <stddef.h>

void output_stringN(const char* const string, size_t size);
void output_string(const char* const string, bool newline);

#endif // AZ_UTILITIES_OUTPUT_H
