#include "syscall.h"

int main() {
    int number;

    number = ReadNum();

    PrintNum(number);

    Halt();
}