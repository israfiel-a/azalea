/**
 * @file Files.h
 * @brief This file contains the standalone compiler abstractions for
 * platform-specific file operations like opening a file handle and such.
 * @authors Israfil Argos
 *
 * @copyright (c) 2025 - the Azalea Project
 * @license AGPLv3
 */
#ifndef AZ_COMPILER_UTILITIES_FILES_H
#define AZ_COMPILER_UTILITIES_FILES_H

#define __need_size_t
#include <stddef.h>

#define FILES_APPEND 0x400
#define FILES_WRITE 0x001
#define FILES_READ 0x00

#define FILES_CREATE 0x040

int files_open(const char *const path, int flags, unsigned int permissions);
void files_close(unsigned int descriptor);
size_t files_size(unsigned int descriptor);
bool files_read(unsigned int descriptor, size_t size, char *contents);
void files_write(unsigned int descriptor, size_t size, const char *const contents);
void files_getCWD(size_t size, char *cwd);

#endif // AZ_COMPILER_UTILITIES_FILES_H 
