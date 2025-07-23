#ifndef COMPILER_H
#define COMPILER_H

#include <Types/AST.h>
#include <Types/Arguments.h>
#include <Types/Tokens.h>

#define COMPILER_ENTRY                                                \
    __asm(                                                            \
        ".global _start\n_start:\nmovl (%rsp), %edi\nlea 8(%rsp), "   \
        "%rsi\ncall main\nmovl %eax, %edi\nmovl $60, %eax\nsyscall"); \
    int main(int argc, char **argv)

bool compiler_arguments(int argc, char **argv, compiler_arguments_t *arguments);

void compiler_getToken(char **contents, compiler_token_t *token);

void compiler_generateAST(const char *const contents, compiler_ast_node_t **head);

bool compiler_interpret(char **contents);

#endif  // COMPILER_H
