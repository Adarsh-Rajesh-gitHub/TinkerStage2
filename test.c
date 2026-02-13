#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <inttypes.h>
#include <assert.h>



void createBinaryFile(uint32_t *arr, int length) {
    FILE *fp = fopen("tmp.bin", "wb");
    fwrite(arr, sizeof(uint32_t), length, fp);
    fclose(fp);
}

bool confirm(char *got, char* expected) {
    char file_string[100];
    FILE *fp = fopen("result.txt", "r");
    fgets(file_string, 100, fp);
    memcpy(got, file_string, 100);
    return strcmp(expected, file_string) == 0;
}


int main(void) {
    int testsPassed = 0;
    int testsFailed = 0;
    //test individual instructions
    //execut
    uint32_t arrNot[] = {0x18420000,
0x10842000,
0xc8800000,
0x3880000c,
0xc8800000,
0x3880000c,
0xc8800000,
0x3880000c,
0xc8800000,
0x3880000c,
0xc8800000,
0x38800004,
0xc8800001,
0x78820004,
0x78000000};
    createBinaryFile(arrNot, 15);
    
    char got[100];
    char *ptr = got;
    system("./hw4 tmp.bin > result.txt");
    char *ans = "18446744073709551615\n";
    if(confirm(ptr, ans)) {
        printf("passed for not(opcode 3)\n");
        testsPassed++;
    }
    else {
        printf("failed for not(opcode 3) - expected: %s got: %s\n", ans, got);
        testsFailed++;
    }

    /// - fibonacci 
    int32_t arrFib1[] = {0x10421000,
0xc8400000,
0x3840000c,
0xc8400000,
0x3840000c,
0xc8400000,
0x3840000c,
0xc8400000,
0x3840000c,
0xc8400000,
0x38400004,
0xc8400001,
0x1739c000,
0xcf000000,
0x3f00000c,
0xcf000000,
0x3f00000c,
0xcf000000,
0x3f00000c,
0xcf000000,
0x3f00000c,
0xcf00010a,
0x3f000004,
0xcf000000,
0x67000000,
0xdfc00008,
0x10842000,
0xc8800000,
0x3880000c,
0xc8800000,
0x3880000c,
0xc8800000,
0x3880000c,
0xc8800000,
0x3880000c,
0xc8800000,
0x38800004,
0xc8800001,
0x78800004,
0x78000000,
0xdfc00008,
0xdfc00008,
0x9fc20000,
0x88020000,
0x807e0000,
0xcfc00008,
0xcfc00008,
0x68000000};
    createBinaryFile(arrFib1, 48);
    
    char gott[100];
    char *ptrr = gott;
    system("./hw4 tmp.bin > result.txt");
    char *anss = "1fs\n";
    if(confirm(ptrr, anss)) {
        printf("passed for not(Fib)\n");
        testsPassed++;
    }
    else {
        printf("failed for not(Fib) - expected: %s got: %s\n", anss, gott);
        testsFailed++;
    }
    

    //test combinations
    return 0;
}