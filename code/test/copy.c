#include "syscall.h"

#define MAX_FILE_LENGTH 32
#define MAX_CONTENT_LENGTH 512

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
    int fileIdSrc, fileIdDes;
    char content[MAX_CONTENT_LENGTH];
    char fileName[MAX_FILE_LENGTH], fileNameNewFile[MAX_FILE_LENGTH];

    // User enter the source file name
    PrintString("Enter the file name to copy from: ");
    readUserInput(fileName);

    // Open file
    fileIdSrc = Open(fileName);

    if (fileIdSrc == -1)
        PrintString("\nOpen file failed\n");
    else
    {
        PrintString("\nOpen file successfully\n");

        if (Read(content, MAX_CONTENT_LENGTH, fileIdSrc) == -1)
            PrintString("\nCopy failed\n");
        else
        {
            PrintString("\nCopy successfully\n");

            // New file name
            PrintString("\nEnter a file name for the new file: ");
            readUserInput(fileNameNewFile);

            // Create the new file
            if (!Create(fileNameNewFile))
                PrintString("\nCreate new file failed\n");
            else
            {
                PrintString("\nCreate new file successfully\n");

                fileIdDes = Open(fileNameNewFile);

                if (fileIdDes == -1)
                    PrintString("\nOpen new file failed\n");
                else
                {
                    PrintString("\nOpen new file successfully\n");

                    // Copy the content into the new file
                    if (Write(content, MAX_CONTENT_LENGTH, fileIdDes) == -1)
                        PrintString("\nPaste the content into the new file failed\n");
                    else
                        PrintString("\nPaste the content into the new file successfully\n");

                    if (Close(fileIdDes) == -1)
                        PrintString("\nClose the new file failed\n");
                    else
                        PrintString("\nClose the new file file\n");
                }
            }
        }
        if (Close(fileIdSrc) == -1)
            PrintString("\nClose the source file failed\n");
        else
            PrintString("\nProgram executed beautifully\n\n");
    }

    Halt();
}