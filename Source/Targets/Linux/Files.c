#include <Utilities/Files.h>

void files_getCWD(size_t size, char* cwd) {
    __asm("syscall" :"=a"(cwd) : "a"(79), "D"(cwd), "S"(size) : "rcx", "r11", "memory");
}

size_t files_size(unsigned int descriptor) {
    struct {
        char p1[48];
        int size;
        char p2[92];
    } statBuffer = {0};

    __asm("syscall"
          :
          : "a"(5), "D"(descriptor), "S"(&statBuffer)
          : "rcx", "r11", "memory");
    return statBuffer.size;
}

int files_open(const char* const path, int flags, unsigned int permissions)
{
    int descriptor;
    __asm("syscall"
          : "=a"(descriptor)
          : "a"(2), "D"(path), "S"(flags), "d"(permissions)
          : "rcx", "r11");
   return descriptor; 
}

void files_close(unsigned int descriptor)
{
    __asm("syscall" : : "a"(3), "D"(descriptor) : "rcx", "r11");
}

bool files_read(unsigned int descriptor, size_t size, char* contents) {
    long read;
    __asm("syscall"
          : "=a"(read)
          : "a"(0), "D"(descriptor), "S"(contents), "d"(size)
          : "rcx", "r11");
    contents[size] = 0;

    return read == (long)size;
}

void files_write(unsigned int descriptor, size_t size, const char* const contents)  {
    __asm("syscall"
          :
          : "a"(1), "D"(descriptor), "S"(contents), "d"(size)
          : "rcx", "r11");
}

