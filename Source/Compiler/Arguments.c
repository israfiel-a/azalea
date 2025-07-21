#include <Compiler.h>

void compiler_arguments(int argc, char **argv, char **inputNames,
                        size_t *inputCount) {
    for (size_t i = 0; i < (size_t)argc; i++) {
        inputNames[*inputCount] = argv[i];
        (*inputCount)++;
    }
}

