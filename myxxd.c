#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/*void writeTestFile() {
    FILE *file = fopen("testfile.bin", "wb");
    unsigned char writeChar = 0;
    for (int i = 0; i < 256; ++i) {
        fwrite((const void*)&writeChar, 1, 1, file);
        ++writeChar;
    }
    fclose(file);
}*/
/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
    /*if (argc > 1) {
        if (strcmp(argv[1], "-f") == 0) {
            FILE *input = fopen(argv[2], "rb");
            if (argc == 4) {
                if (strcmp(argv[3], "-b") == 0 || strcmp(argv[3], "-bits") == 0) {
                    *bits = TRUE;
                }
            }
            else {
                *bits = FALSE;
            }
            return input;
        }
        if (strcmp(argv[1], "-writetestfile") == 0) {
            writeTestFile();
            exit(0);
        }
    }*/

    if (argc > 2) {
        printf("Usage: %s [-b|-bits]\n", argv[0]);
        exit(BAD_NUMBER_ARGS);
    }

    if (argc == 2) {
        if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0) {
            *bits = TRUE;
        }
    }
    else {
        *bits = FALSE;
    }

    return stdin;
}



/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size) {
    printf(" ");
    for (unsigned int i = 0; i < size; ++i) {
        if (i != 0 && i % 2 == 0) {
            printf(" %02x", data[i]);
        }
        else {
            printf("%02x", data[i]);
        }
    }
    if (size < 16) {
        for (unsigned int i = 0; i < 16 - size; ++i) {
            if (i != 0 && i % 2 == 0){
                printf("  ");
            }
            else {
                printf(" ");
            }
        }
    }
}

void printDataAsBits(unsigned char *data, size_t size) {
    char binary[8];
    int x = 0;
    printf(" ");
    for (int i = 0; i < size; ++i) {
        x = data[i];
        for (int j = 7; j >= 0; --j) {
            if (x % 2 == 1) {
                binary[j] = '1';
            } else {
                binary[j] = '0';
            }
            x = x / 2;
        }
        for (int j = 0; j < 8; ++j) {
            printf("%c", binary[j]);

        }
        printf(" ");
    }
    if (size < 6) {
        for (int i = 0; i < 6 - size; ++i) {
            printf("         ");
        }
        printf(" ");
    }
    else {
        printf(" ");
    }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
    for (unsigned int i = 0; i < size; ++i) {
        if (data[i] < 32 || data[i] > 126) {
            printf(".");
        }
        else {
            printf("%c", data[i]);
        }
    }
}

void readAndPrintInputAsHex(FILE *input) {
    unsigned char data[16];
    int numBytesRead = fread(data, 1, 16, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsHex(data, numBytesRead);
        printf("  ");
        printDataAsChars(data, numBytesRead);
        printf("\n");
        numBytesRead = fread(data, 1, 16, input);
    }
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
    unsigned char data[6];
    int numBytesRead = fread(data, 1, 6, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsBits(data, numBytesRead);
        //printf("  ");
        printDataAsChars(data, numBytesRead);
        printf("\n");
        numBytesRead = fread(data, 1, 6, input);
    }
}

int main(int argc, char **argv) {
    int bits = FALSE;
    FILE *input = parseCommandLine(argc, argv, &bits);
    if (bits == FALSE) {
        readAndPrintInputAsHex(input);
    } else {
        readAndPrintInputAsBits(input);
    }
    return 0;
}
