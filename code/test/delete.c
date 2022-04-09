#include "syscall.h"

int main()
{
    int res;

    if (Remove("GauGau.txt") == -1)
        PrintString("Remove failed\n");
    else 
        PrintString("Successfully remove file GauGau.txt\n");

    Halt();
}