#include "syscall.h"

int main()
{
    int length;
    char *fileName;

    PrintString("Please enter the length of the file name: ");
    length = ReadNum();
    PrintChar('\n');
    PrintString("Now the file name: ");
    ReadString(fileName, length);

    if (Create(fileName) == 1)
    {
        PrintString("\nCreate file ");
        PrintString(fileName);
        PrintString(" successfully\n\n");
    }
    else
        PrintString("\nError: Cannot create file\n");

    Halt();
}