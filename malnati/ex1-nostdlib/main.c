#define SYSCALL_EXIT 60
typedef long long int64;

void syscall_1(int64 syscall_number, int64 arg1) {
  asm(
      "mov %0, %%rax\n"
      "mov %1, %%rdi\n"
      "syscall"
      :
      : "r"(syscall_number), "r"(arg1)
      : "%rax", "%rdi"
  );
}

int main(void);

void exit(int return_code) {
  syscall_1(SYSCALL_EXIT, return_code);
}

void _start(void) {
    exit(main());
}

int main(void) {
    char *hello = "Hello, world!";
    return 0;
}