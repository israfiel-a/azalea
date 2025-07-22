#include <Utilities/Output.h>

void output_string(const char* const string, size_t number, bool newline) {
    __asm("syscall"
          :
          : "a"(1), "D"(1), "S"(string), "d"(number)
          : "rcx", "r11");
    if (newline)
        __asm("syscall" : : "a"(1), "D"(1), "S"("\n"), "d"(1) : "rcx", "r11");
}

