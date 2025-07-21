#include <Utilities/Output.h>
#include <Utilities/Strings.h>

void output_stringN(const char* const string, size_t number) {
    __asm("syscall"
          :
          : "a"(1), "D"(1), "S"(string), "d"(number)
          : "rcx", "r11");
}

void output_string(const char* const string, bool newline) {
    output_stringN(string, strings_getLength(string));
    if (newline) output_stringN("\n", 1);
}

