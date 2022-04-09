#include "syscall.h"

int main()
{
    int fileIdSrc, fileIdDes;
    char *content;
    int lengthFileSrc, lengthFileDes, contentLength;
    char *fileName, *fileNameNewFile;

    // User enter the source file name
    PrintString("Please enter the length of the file name: ");
    lengthFileSrc = ReadNum();
    PrintString("Now the file name: ");
    ReadString(&fileName, lengthFileSrc + 1);

    PrintString(&fileName);
    PrintChar('\n');

    // Open file
    fileIdSrc = Open(&fileName);

    if (fileIdSrc == -1)
        PrintString("Open file failed\n");
    else
    {
        PrintString("Open file successfully\n");

        // Get content length
        PrintString("How much characters in the file do you want to copy? ");
        contentLength = ReadNum();

        if (Read(&content, contentLength, fileIdSrc) == -1)
            PrintString("\nCopy failed");
        else
        {
            PrintString("\nCopy successfully\n");
            PrintString(&content);

            // New file name
            PrintString("\nPlease enter the length of the new-file name: ");
            lengthFileDes = ReadNum();
            
            PrintString("Now the new-file name: ");
            ReadString(&fileNameNewFile, lengthFileDes + 1);
            //ReadChar(); // omit the \n from user 

            // Create the new file
            if (!Create(&fileNameNewFile))
                PrintString("\nCreate new file failed\n");
            else
            {
                PrintString("\nCreate new file successfully\n");

                fileIdDes = Open(&fileNameNewFile);

                if (fileIdDes == -1)
                    PrintString("\nOpen new file failed\n");
                else
                {
                    PrintString("\nOpen new file successfully\n");
                    PrintString(&content);

                    // Copy the content into the new file
                    if (Write(&content, contentLength, fileIdDes) == -1)
                        PrintString("\nPaste failed\n");
                    else
                        PrintString("\nPaste successfully\n");

                    if (Close(fileIdDes) == -1)
                        PrintString("\nClose file failed\n");
                    else
                        PrintString("\nClose file\n");
                }
            }
        }
        if (Close(fileIdSrc) == -1)
            PrintString("\nClose file failed\n");
        else
            PrintString("\nProgram executed beautifully\n");
    }

    Halt();
}