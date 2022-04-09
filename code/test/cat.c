#include "syscall.h"

int main()
{
    int fileID, fileLength, contentLength;
    char *content, *fileName;
    char c;

    PrintString("Please enter the length of the file name: ");
    fileLength = ReadNum();
    PrintChar('\n');
    PrintString("Now the file name: ");
    ReadString(fileName, fileLength);

    fileID = Open(fileName);

    if (fileID == -1)
        PrintString("Open file failed\n");
    else
    {
        PrintString("Open file successfully\n");

        ReadChar(); // omit the \n from user

        PrintString("\nEnter the length of content you want to read: ");
        contentLength = ReadNum();
        PrintChar('\n');

        if (Read(content, contentLength, fileID) == -1)
            PrintString("Read file failed\n");
        else
        {
            PrintString("\nFile content:\n");
            PrintString(content);
        }

        if (Close(fileID) == -1)
            PrintString("Close file failed\n");
        else
            PrintString("\nProgram executed beautifully\n\n");
    }

    Halt();
}