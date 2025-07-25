/**
 * @file Arguments.c
 * @authors Israfil Argos
 * @brief This file provides the implementation for the `compiler_getArguments`
 * function defined in the main Azalea compiler interface public header.
 *
 * @copyright (c) 2025 - Israfil Argos
 * @license AGPLv3
 * This source code is under the GNU Affero General Public License. This program
 * can be repackaged and distributed freely, so long as its license is made
 * clearly available and according to the terms of its license.
 */
#include <Compiler.h>
#include <Utilities/Output.h>
#include <Utilities/Strings.h>

/**
 * @brief The notice given to the user when a help menu is requested. It contains the
 * command usage format and a list of flags broken into sections.
 * @since v1.0.0.0
 */
#define HELP_NOTICE                                                                 \
    "Usage: compiler [OPTIONS] [DIRECTORY | "                                       \
    "FILE]\nOptions:\n\tINFORMATION:\n\t--help: Display this message and "          \
    "exit.\n\t--version: Display version information and exit.\n\t--license: "      \
    "Display copyright information and exit.\n\n\tCOMPILATION:\n\t--nasm: Use the " \
    "NASM assembly backend.\n\t--fasm: Use the FASM assembly "                      \
    "backend.\n\t--interpreted: Run the given target as a script file.\n\n"

/**
 * @brief The notice given to the user when version information is requested.
 * @since v1.0.0.0
 */
#define VERSION_NOTICE "Azalea compiler interface version " VERSION_STRING ".\n\n"

/**
 * @brief The notice given to the user when copyright/licensing information is
 * requested. It contains the license name, a summary of distribution guidelines, and
 * a link to the license itself.
 * @since v1.0.0.0
 */
#define LICENSE_NOTICE                                                              \
    "This source code is under the GNU Affero General Public License version 3.0. " \
    "It can be repackaged, redistributed, and modified according to the terms of "  \
    "the license. The AGPL can be found at "                                        \
    "<https://www.gnu.org/licenses/agpl-3.0.md>.\n\n"

/**
 * @brief Check the given argument to see if it's a request for any kind of
 * information, and respond accordingly.
 * @since v1.0.0.0
 *
 * @param[in] arg The argument to check.
 *
 * @returns A boolean flag representing whether or not the given argument was normal.
 *
 * @notice This function is not marked "inline" because it is used in multiple places
 * throughout the file. While it would increase speed by a couple microseconds, the
 * codesize downgrade would not be worth it.
 */
[[nodiscard]] [[gnu::nonnull(1)]]
static bool checkInformationRequest(const char *const arg)
{
    if (__builtin_expect(strings_compare(arg, "--help"), 0))
    {
        output_string(HELP_NOTICE, sizeof(HELP_NOTICE), false);
        return false;
    }
    else if (__builtin_expect(strings_compare(arg, "--version"), 0))
    {
        output_string(VERSION_NOTICE, sizeof(VERSION_NOTICE), false);
        return false;
    }
    else if (__builtin_expect(strings_compare(arg, "--license"), 0))
    {
        output_string(LICENSE_NOTICE, sizeof(LICENSE_NOTICE), false);
        return false;
    }
    return true;
}

/**
 * @brief Check to make sure we've gotten the proper amount of arguments and, if we
 * have, if the first one is or isn't an information request.
 * @since v1.0.0.0
 *
 * @param[in] argc The count of arguments as reported by the command line.
 * @param[in] argv The values of the arguments as reported by the command line.
 *
 * @returns A boolean flag representing whether or not the arguments were valid and
 * if checks should continue.
 */
[[gnu::always_inline]] [[gnu::flatten]] [[nodiscard]] [[gnu::nonnull(2)]]
static inline bool validateArguments(int argc, const char *const *const argv)
{
    output_string("\n", 1, false);
    if (__builtin_expect(argc <= 1, 0))
    {
        output_string("The compiler was not provided any arguments.\n\n", 46, false);
        return false;
    }

    // This is insurance in case the user only specifies an info request in
    // arguments, as our system of "always treat last arg as file/directory" will
    // fuck this up.
    return checkInformationRequest(argv[1]);
}

bool compiler_getArguments(int argc, const char *const *const argv,
                           compiler_arguments_t *arguments)
{
    if (__builtin_expect(!validateArguments(argc, argv), 0))
        return false;

    for (size_t i = 1; i < (size_t)argc - 1; ++i)
    {
        const char *const currentArg = argv[i];
        if (__builtin_expect(!checkInformationRequest(currentArg), 0))
            return false;

        if (strings_compare(currentArg, "--nasm"))
            arguments->backend = NASM_BACKEND;
        else if (strings_compare(currentArg, "--fasm"))
            arguments->backend = FASM_BACKEND;
        else if (strings_compare(currentArg, "--interpreted"))
            arguments->flags.interpreted = true;
        else
        {
            size_t messageLength = 39 + strings_getLength(currentArg);
            char message[messageLength];
            strings_concatenate(message, "Got unknown command line argument '$'.\n\n", currentArg);
            output_string(message, messageLength, false);
            return false;
        }
    }

    return true;
}

