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
#include "Converter.h"



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

        //badMem tests
    assert(badMem(0x0FFF,4)==true);
    assert(badMem(0x1000,4)==false);
    assert(badMem(0x1001,4)==true);
    assert(badMem(0x1004,4)==false);
    assert(badMem(512*1024-4,4)==false);
    assert(badMem(512*1024-3,4)==true);
    assert(badMem(512*1024,4)==true);
    assert(badMem(512*1024-8,8)==false);
    assert(badMem(512*1024-7,8)==true);

    //readStrict tests (feed stdin via a temp file + freopen)
    {
        FILE *f=fopen("tmp_stdin.txt","wb");
        assert(f);
        fputs("0\n18446744073709551615\n00012\n",f);
        fclose(f);

        assert(freopen("tmp_stdin.txt","rb",stdin));
        uint64_t x=999;

        assert(readStrict(&x)==1&&x==0ULL);
        assert(readStrict(&x)==1&&x==18446744073709551615ULL);
        assert(readStrict(&x)==1&&x==12ULL);
    }
    {
        FILE *f=fopen("tmp_stdin.txt","wb");
        assert(f);
        fputs("-1\n+1\n1.0\n123abc\n18446744073709551616\n\n",f);
        fclose(f);

        assert(freopen("tmp_stdin.txt","rb",stdin));
        uint64_t x=0;

        assert(readStrict(&x)==0);
        assert(readStrict(&x)==0);
        assert(readStrict(&x)==0);
        assert(readStrict(&x)==0);
        assert(readStrict(&x)==0);

        //EOF case: should fail too
        while(readStrict(&x)==1){}
        assert(readStrict(&x)==0);
    }




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
    char *anss = "1\n";
    if(confirm(ptrr, anss)) {
        printf("passed for not(Fib)\n");
        testsPassed++;
    }
    else {
        printf("failed for not(Fib) - expected: %s got: %s\n", anss, gott);
        testsFailed++;
    }
    
    //test individual instructions with execute
    uint64_t pc;
    // and
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=6;
    registers[2]=3;
    assert(execute((0u<<27)|(3u<<22)|(1u<<17)|(2u<<12),&pc)==0);
    assert(registers[3]==2);

    //  or
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=4;
    registers[2]=1;
    assert(execute((1u<<27)|(3u<<22)|(1u<<17)|(2u<<12),&pc)==0);
    assert(registers[3]==5);

    // xor
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=7;
    registers[2]=3;
    assert(execute((2u<<27)|(3u<<22)|(1u<<17)|(2u<<12),&pc)==0);
    assert(registers[3]==4);

    // not
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=0;
    assert(execute((3u<<27)|(2u<<22)|(1u<<17)|(0u<<12),&pc)==0);
    assert(registers[2]==~0ULL);

    // shr
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=8;
    registers[2]=1;
    assert(execute((4u<<27)|(3u<<22)|(1u<<17)|(2u<<12),&pc)==0);
    assert(registers[3]==4);

    // shri 
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=8;
    assert(execute((5u<<27)|(1u<<22)|(0u<<17)|(0u<<12)|2u,&pc)==0);
    assert(registers[1]==2);

    // shl 
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=1;
    registers[2]=3;
    assert(execute((6u<<27)|(3u<<22)|(1u<<17)|(2u<<12),&pc)==0);
    assert(registers[3]==8);

    //shli 
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=2;
    assert(execute((7u<<27)|(1u<<22)|(0u<<17)|(0u<<12)|3u,&pc)==0);
    assert(registers[1]==16);

    //add
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=2;
    registers[2]=5;
    assert(execute((24u<<27)|(3u<<22)|(1u<<17)|(2u<<12),&pc)==0);
    assert(registers[3]==7);

    // addi
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[1]=10;
    assert(execute((25u<<27)|(1u<<22)|(0u<<17)|(0u<<12)|5u,&pc)==0);
    assert(registers[1]==15);

    // mov rd l
    memset(registers,0,sizeof(registers));
    pc=0x1000;
    registers[5]=0x12345000ULL; 
    assert(execute((18u<<27)|(5u<<22)|(0u<<17)|(0u<<12)|0xABCu,&pc)==0);
    assert(registers[5]==0x12345ABCu);

    remove("tmp_stdin.txt");
    puts("Passed all unit tests");
    return 0;
}