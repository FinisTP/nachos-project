#include "syscall.h"

int main() {
    char* c;

    ReadString(c, 5);

    Halt();
}