/**
 * @file Arguments.h
 * @authors Israfil Argos
 * @brief This file provides the structure into which provided command line arguments
 * will be deserialized into by the Azalea compiler.
 *
 * @copyright (c) 2025 - Israfil Argos
 * @license AGPLv3
 * This source code is under the GNU Affero General Public License. This program
 * can be repackaged and distributed freely, so long as its license is made
 * clearly available and according to the terms of its license.
 */
#ifndef AZ_COMPILER_TYPES_ARGUMENTS_H
#define AZ_COMPILER_TYPES_ARGUMENTS_H

/**
 * @brief The structure containing the configuration arguments given to the
 * compiler.
 * @since v1.0.0.0
 */
typedef struct compiler_arguments
{
    struct
    {
        /**
         * @brief A flag representing whether or not the compiler is to interpret the
         * given target or compile it.
         * @since v1.0.0.0
         */
        bool interpreted : 1;
    }
    /**
     * @brief A container for simple boolean flags used to tweak certain
     * black-and-white behaviors in the compiler.
     * @since v1.0.0.0
     */
    flags;

    enum
    {
        /**
         * @brief The Netwide Assembler backend.
         * @since v1.0.0.0
         *
         * @see https://www.nasm.us
         */
        NASM_BACKEND,
        /**
         * @brief The flat assembler [sic] backend.
         * @since v1.0.0.0
         *
         * @see https://flatassembler.net
         */
        FASM_BACKEND
    }
    /**
     * @brief An enumerator describing which backend has been specified for use in
     * the compiler.
     * @since v1.0.0.0
     */
    backend;

    /**
     * @brief The compiler's target. This can be a file or a directory, depending on
     * the context (interpreted, etc). 
     * @since v1.0.0.0
     */
    const char *const target;
} compiler_arguments_t;

#endif // AZ_COMPILER_TYPES_ARGUMENTS_H
