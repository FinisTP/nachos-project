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
    int fileID1, fileID2, newFileID;
    char fileName1[MAX_FILE_LENGTH], fileName2[MAX_FILE_LENGTH], newFileName[MAX_FILE_LENGTH];
    char content1[MAX_CONTENT_LENGTH], content2[MAX_CONTENT_LENGTH];

    PrintString("Please enter the name of the 1st file: ");
    readUserInput(fileName1);

    PrintString("Please enter the name of the 2nd file: ");
    readUserInput(fileName2);

    fileID1 = Open(fileName1);
    fileID2 = Open(fileName2);

    if (fileID1 != -1 && fileID2 != -1)
    {
        PrintString("\nSuccessfully read two files.\n\n");

        Read(content1, MAX_CONTENT_LENGTH, fileID1);
        Read(content2, MAX_CONTENT_LENGTH, fileID2);
        Seek(-1, fileID1);
        Seek(-1, fileID2);

        PrintString("Please enter the name of the new file: ");
        readUserInput(newFileName);

        if (Create(newFileName) == -1)
            PrintString("\nCreate new file failed.\n");
        else
        {
            newFileID = Open(newFileName);

            if (newFileID == -1)
                PrintString("ERROR: Cannot open new file!\n\n");
            else
            {
                Write(content1, MAX_CONTENT_LENGTH, newFileID);
                Seek(MAX_CONTENT_LENGTH, newFileID);
                Write(content2, MAX_CONTENT_LENGTH, newFileID);
                PrintString("Successfully concatenated two files. Result is put into the new file.");

                Close(newFileID);
            }
        }
    }
    else
    {
        PrintString("Either of the file is unavailable, please check if the file name is correctly spelled.");
    }

    Close(fileID1);
    Close(fileID2);

    Halt();
}