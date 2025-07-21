#include <Utilities/Numbers.h>

size_t numbers_countDigits(size_t number) {
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

void numbers_toString(size_t number, size_t length, char* string) {
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

