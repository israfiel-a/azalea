#ifndef UTILITIES_H
#define UTILITIES_H

#include <stddef.h>

// Most of these are defined within the target files, with some functionality like string manipulation implemented in the global Utilities.c file.

bool utilities_stringEqual(const char* const a, const char* const b);

// This actually edits the passed in string, replacing all delims with a NUL.
void utilities_stringSplitLines(char* string, char** storage, size_t *count);

size_t utilities_stringLength(const char* const string);

size_t utilities_stringCountCharacters(const char *const string, char character);

size_t utilities_numberLength(size_t number);

void utilities_numberToString(size_t number, size_t length, char* string);

void utilities_outputString(const char* const string, bool newline);

void utilities_getCWD(char* buffer, size_t bufferSize);

size_t utilities_getFileSize(const char* const name);

void utilities_loadFile(const char* const name, size_t size, char* contents);

#define ENTRY(name) __asm(".global _start\n_start:\nmovl (%rsp), %edi\nlea 8(%rsp), %rsi\ncall " #name "\nmovl %eax, %edi\nmovl $60, %eax\nsyscall"); int name(int argc, char** argv)

#endif // UTILITIES_H
