#include <Utilities.h>
#include <stdint.h>
#include <sys/stat.h>

// INLINE ASSEMBLY BLUEPRINT
// ORDER:
//      1. TEMPLATE: The template string from which the actual assembly is
//      generated.
//      2. OUTPUT: A comma-separated list of the C variables modified by the
//      instructions in the generated assembly.
//      3. INPUT: A comma-separated list of the C expressions to be used within
//      templating.
//      4. CLOBBERS: A comma-separated list of the registers changed bu the
//      generated assembly, exluding those in OUTPUT.
//
// EXAMPLES:
//      1. What does a template string look like, beyond actual assembly
//      instructions?
//         mov %[input_variable], $1
//         This will move the value "1" into the input variable. In order to
//         properly understand what input_variable means, it should be included
//         within the OUTPUT section. A double % in the template string is the
//         exact same as a \\ in a normal string; it just outputs a single %. A
//         list of other useful escapes are as follows;
//         %{, %|, %}: Each of these simply outputs their described character.
//         When unescaped, they can be used to determine different dialects of
//         assembly (in the syntax of { d1 | d2 }).
//      2. What does an output section look like?
//         : [one]"=a"(foo)
//         The "one" enclosed in brackets is the output's symbolic name,
//         referenced as "%[one]" in the assembly template. This is completely
//         optional. The "a" in quotes is the constraint of the output. A list
//         of these constraints can be found at
//         <gcc.gnu.org/onlinedocs/gcc/Constraints.html The (foo) is the actual
//         variable value of the symbol. This must be a valid C variable defined
//         earlier in the program.
//
// The syscall instruction clobbers RCX and R11.

void utilities_outputStringN(const char* const string, size_t number) {
    __asm("syscall"
          : 
          : "a"(1),            // rax
            "D"(1),            // rdi
            "S"(string),       // rsi
            "d"(number)        // rdx
          : "rcx", "r11");
}

void utilities_outputString(const char* const string, bool newline) {
    size_t stringLength = utilities_stringLength(string);

    utilities_outputStringN(string, stringLength);
    if (!newline) return;

    char* output = "\n";
    __asm("syscall"
          : 
          : "a"(1), "D"(1), "S"(output), "d"(1)
          : "rcx", "r11");
}

void utilities_getCWD(char* buffer, size_t bufferSize) {
    long returnValue;
    __asm("syscall"
          : "=a"(returnValue)
          : "a"(79),
            "r"(buffer),     // rdi
            "r"(bufferSize)  // rsi
          : "rcx", "r11", "memory");
}

size_t utilities_getFileSize(const char* const name) {
    struct {
        uint8_t p1[48];
        uint32_t size;
        uint8_t p2[92];
    } statBuffer = {0};

    __asm("syscall"
          :
          : "a"(4), "D"(name), "S"(&statBuffer)
          : "rcx", "r11", "memory");
    return statBuffer.size;
}

void utilities_loadFile(const char* const name, size_t size, char* contents) {
    int fd;
    __asm("syscall"
          : "=a"(fd)
          : "a"(2), "D"(name), "S"(0), "d"(0)
          : "rcx", "r11");

    __asm("syscall"
          :
          : "a"(0), "D"(fd), "S"(contents), "d"(size)
          : "rcx", "r11");
    contents[size] = 0;
}
