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
 * clearly available and according to the terms of its license.
 */
#ifndef COMPILER_H
#define COMPILER_H

#include <Types/AST.h>
#include <Types/Arguments.h>
#include <Types/Tokens.h>

/**
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

/**
 * @brief This function retrieves and formats provided command-line arguments into
 * something the client should use to tailor its execution. It does not retrieve the
 * target, but since the target is always the last arguments clients are expected to
 * grab it with `argv[argc - 1]`.
 * @since v1.0.0.0
 *
 * @param[in] argc The count of arguments as taken from the command line.
 * @param[in] argv The argument values as taken from the command line.
 * @param[out] arguments The arguments formatted into their proper structure.
 *
 * @returns A boolean flag representing whether or not the compiler should continue
 * execution. Note that this does not necessarily mean an error occurred, in the case
 * of a "--help", "--version", or "--license" flag, this just simply means "quit
 * trying to compile".
 */
bool compiler_getArguments(int argc, const char *const *const argv,
                           compiler_arguments_t *arguments);

bool compiler_getToken(char **contents, compiler_token_t *token);

bool compiler_generateAST(const char *const contents, compiler_ast_node_t **head);

bool compiler_interpretUnit(const compiler_ast_node_t *const head);

#endif // COMPILER_H
