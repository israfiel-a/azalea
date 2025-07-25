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

typedef struct compiler_arguments
{
    struct
    {
        bool interpreted : 1;
    } flags;
    enum
    {
        NASM_BACKEND,
        FASM_BACKEND
    } backend;
    const char *const target;
} compiler_arguments_t;

#endif // AZ_COMPILER_TYPES_ARGUMENTS_H
