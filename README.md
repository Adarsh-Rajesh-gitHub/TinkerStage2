readme


for my testing I brainstormed som etest cases in the orignal tinker format then used last prog to convert it and then store in the array and test that input with my testing harness

# 

```nasm
.code
	not r1 r1
	ld r2 1
	out r2 r1
	priv r0, r0, r0, 0
```

test not

1. turn r1 into 1ULL << 63 through compliment of all 0s
2. put 1 into r2
3. print r1 by indicating 1 as output port though r2
4. Halt

in stdout expect 1ULL << 63

Adarsh Rajesh - ar77947

to run prog(input.tok is placehold, put actual name):
bash build.sh
/hw4 input.tok


to run testing:
gcc -DTESTING -std=c11 -Wall -Wextra -O2 Converter.c test.c -o test