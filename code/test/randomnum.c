#include "syscall.h"

int main() {
    int number;

    number = RandomNum();

    PrintNum(number);

    Halt();
}