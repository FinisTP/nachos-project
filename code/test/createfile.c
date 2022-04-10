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

    // Ask the user for the file name
    PrintString("Enter the file name (maximum 32 characters): ");
    readUserInput(fileName);

    // Creating file using system call Create()
    if (Create(fileName) == 1)
    {
        // Success
        PrintString("\nCreate file ");
        PrintString(fileName);
        PrintString(" successfully\n\n");
    }
    else
        // Failed
        PrintString("\nError: Cannot create file\n");

    Halt();
}