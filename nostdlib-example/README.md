# Compiling with `-nostdlib`
Source: https://blogs.oracle.com/linux/hello-from-a-libc-free-world-part-1-v2

This super-simple program
```c
int main(void) {
    char *hello = "Hello, world!";
    return 0;
}
```
Causes this warning upon compilation:
```bash
$ gcc main.c -nostdlib -o main
/usr/bin/ld: warning: cannot find entry symbol _start; defaulting to 0000000000001000
$ ./main
Segmentation fault
```
> From the perspective of the linker, by default `_start` is the actual entry point to your program, not `main`. It is normally defined in the `crt1.o` ELF relocatable.
Let's try to link our `main.o` file with `crt1.o`:
```bash
$ gcc -Os -c main.c # compiling without linking
$ ld /usr/lib/x86_64-linux-gnu/crt1.o main.o -o main # linking
ld: /usr/lib/x86_64-linux-gnu/crt1.o: in function `_start':
(.text+0x12): undefined reference to `__libc_csu_fini'
ld: (.text+0x19): undefined reference to `__libc_csu_init'
ld: (.text+0x26): undefined reference to `__libc_start_main'
```
The linker needs all the previously-mentioned functions to run the program.
