#include "syscall.h"

#define MAX_FILE_LENGTH 32

void readUserInput(char *content)
{
    char c;
    int i;
    i = 0;
    do
    {
        c = ReadChar();
        if (c == '\n')
        {
            content[i] = '\0';
            break;
        }
        content[i] = c;
        ++i;
    } while (c != '\n');
}

int main()
{
    // Init
    char fileName[MAX_FILE_LENGTH];
    PrintString("Please enter the name of the file to remove: ");
    readUserInput(fileName);

    // Deleting the file using syscall Remove()
    if (Remove(fileName) == -1)
        PrintString("Remove failed\n");
    else {
        PrintString("Successfully remove file ");
        PrintString(fileName);
        PrintString("\n\n");
    }
    Halt();
}