/**
 * @file Compiler.h
 * @authors Israfil Argos
 * @brief This file provides the public-facing C API for the standard, default
 * Azalea compilation interface. It only contains strictly necessary
 * functionality to compile an Azalea file.
 *
 * @copyright (c) 2025 - Israfil Argos
 * @license AGPLv3
 * This source code is under the GNU Affero General Public License. This program
 * can be repackaged and distributed freely, so long as its license is made
 * clearly available.
 */
#ifndef COMPILER_H
#define COMPILER_H

#include <Types/AST.h>
#include <Types/Arguments.h>
#include <Types/Tokens.h>

/**
 * @def COMPILER_ENTRY
 * @brief This defines what one would call the "main" function of any GLibC
 * application; command line arguments are moved into the place of the first and
 * second function arguments and as such provided to the compiler client. If you
 * are compiling with GlibC, this is not needed and wastes clock cycles.
 * @since v1.0.0.0
 */
#define COMPILER_ENTRY                                                              \
    __asm(".global _start\n_start:\nmovl (%rsp), %edi\nlea 8(%rsp), "               \
          "%rsi\ncall main\nmovl %eax, %edi\nmovl $60, %eax\nsyscall");             \
    int main(int argc, const char *const *const argv)

bool compiler_getArguments(int argc, const char *const *const argv,
                           compiler_arguments_t *arguments);

bool compiler_getToken(char **contents, compiler_token_t *token);

bool compiler_generateAST(const char *const contents, compiler_ast_node_t **head);

bool compiler_interpretUnit(const compiler_ast_node_t *const head);

#endif // COMPILER_H
