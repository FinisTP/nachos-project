#include "syscall.h"

#define MAX_FILE_LENGTH 32
#define MAX_CONTENT_LENGTH 160

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
    int fileID, contentLength;
    char content[MAX_CONTENT_LENGTH], fileName[MAX_FILE_LENGTH];
    char c;

    // Ask the user for the original file's name
    PrintString("Enter the file name: ");
    readUserInput(fileName);

    // Open the file using syscall Open
    fileID = Open(fileName);

    if (fileID == -1)
        PrintString("Open file failed\n");
    else
    {
        PrintString("Open file successfully\n");

        // Ask the user how many character to read
        PrintString("\nEnter the length of content you want to read: ");
        contentLength = ReadNum();
        PrintChar('\n');

        // Read and print out the content using syscall Read()
        if (Read(content, contentLength, fileID) == -1)
            PrintString("Read file failed\n");
        else
        {
            PrintString("\nFile content:\n");
            PrintString(content);
        }

        // Close the file using syscall Close()
        if (Close(fileID) == -1)
            PrintString("Close file failed\n");
        else
            PrintString("\nProgram executed beautifully\n\n");
    }

    Halt();
}