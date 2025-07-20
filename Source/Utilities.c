#include <Utilities.h>

bool utilities_stringEqual(const char* const a, const char* const b) {
    char *av = (char*)a, *bv = (char*)b;
    while (*av == *bv) {
        if (*av == 0) return true;
        av++;
        bv++;
    }
    return false;
}

bool utilities_stringEqualUntil(const char* const a, const char* const b,
                                char until) {
    char *av = (char*)a, *bv = (char*)b;
    while (*av == *bv) {
        if (*av == 0 || *av == until) return true;
        av++;
        bv++;
    }
    return false;
}

bool utilities_stringEqualN(const char* const a, const char* const b,
                            size_t number) {
    char *av = (char*)a, *bv = (char*)b;
    size_t checked = 0;
    while (*av == *bv) {
        if (*av == 0 || checked == number) return true;
        av++;
        bv++;
        checked++;
    }
    return false;
}

static void stripLeading(char** string) {
    while (**string == '\t' || **string == ' ') (*string)++;
}

void utilities_stringSplitLines(char* string, char** storage, size_t* count) {
    char* stringCopy = (char*)string;
    char* segmentBeginning = stringCopy;
    while (*stringCopy != 0) {
        if (*stringCopy == '\n') {
            stripLeading(&segmentBeginning);
            if (stringCopy - segmentBeginning != 0 &&
                (*segmentBeginning != '/' && *(segmentBeginning + 1) != '/')) {
                *storage = segmentBeginning;
                storage++;
            } else
                (*count)--;
            *stringCopy = 0;
            segmentBeginning = stringCopy + 1;
        }
        stringCopy++;
    }
}

size_t utilities_stringLength(const char* const string) {
    char* stringPointer = (char*)string;
    while (*stringPointer++ != 0) __asm("");
    return stringPointer - string;
}

size_t utilities_stringCountCharacters(const char* const string,
                                       char character) {
    size_t count = 0;
    char* stringCopy = (char*)string;
    while (*stringCopy != 0) {
        if (*stringCopy == character) count++;
        stringCopy++;
    }
    return count;
}

size_t utilities_stringFindCharacter(const char* const string, char character,
                                     bool front) {
    char* stringCopy = (char*)string;
    if (front) {
        while (*stringCopy != 0 && *stringCopy != character) stringCopy++;
        return stringCopy - string;
    }

    size_t length = 0;
    while (*stringCopy != 0) {
        stringCopy++;
        length++;
    }

    while (*stringCopy != character && length > 0) {
        stringCopy--;
        length--;
    }
    return length;
}

size_t utilities_numberLength(size_t number) {
    if (number >= 100000) {
        if (number >= 10000000) {
            if (number >= 1000000000) return 10;
            if (number >= 100000000) return 9;
            return 8;
        }
        if (number >= 1000000) return 7;
        return 6;
    } else {
        if (number >= 1000) {
            if (number >= 10000) return 5;
            return 4;
        } else {
            if (number >= 100) return 3;
            if (number >= 10) return 2;
            return 1;
        }
    }
}

void utilities_numberToString(size_t number, size_t length, char* string) {
    for (size_t i = 0; i < length; i++) {
        switch (number % 10) {
            case 0:
                string[length - i - 1] = '0';
                break;
            case 1:
                string[length - i - 1] = '1';
                break;
            case 2:
                string[length - i - 1] = '2';
                break;
            case 3:
                string[length - i - 1] = '3';
                break;
            case 4:
                string[length - i - 1] = '4';
                break;
            case 5:
                string[length - i - 1] = '5';
                break;
            case 6:
                string[length - i - 1] = '6';
                break;
            case 7:
                string[length - i - 1] = '7';
                break;
            case 8:
                string[length - i - 1] = '8';
                break;
            default:
                string[length - i - 1] = '9';
                break;
        }
        number /= 10;
    }
    string[length] = 0;
}
