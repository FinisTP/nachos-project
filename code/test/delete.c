#include "syscall.h"

int main()
{
    int res;

    res = Remove("TanPhat.txt");

    Halt();
}