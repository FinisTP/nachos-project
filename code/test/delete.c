#include "syscall.h"

int main()
{
    char fileName[MAX_FILE_LENGTH];
    PrintString("Please enter the name of the file to remove: ");
    ReadString(fileName, MAX_FILE_LENGTH);

    if (Remove(fileName) == -1)
        PrintString("Remove failed\n");
    else 
        PrintString("Successfully remove file GauGau.txt\n");

    Halt();
}