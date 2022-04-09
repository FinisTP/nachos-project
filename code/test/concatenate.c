#include "syscall.h"

int main()
{
    int fileID1, fileID2, res;
    char fileName1[MAX_FILE_LENGTH], fileName2[MAX_FILE_LENGTH];
    char content1[MAX_CONTENT_LENGTH], content2[MAX_CONTENT_LENGTH];

    PrintString("Please enter the name of the 1st file: ");
    ReadString(fileName1, MAX_FILE_LENGTH);

    PrintString("Please enter the name of the 2nd file: ");
    ReadString(fileName2, MAX_FILE_LENGTH);

    fileID1 = Open(fileName1);
    fileID2 = Open(fileName2);

    if (fileID1 != -1 && fileID2 != -1)
    {
        Read(content2, MAX_CONTENT_LENGTH, fileID2);
        Seek(-1, fileID1);
        Write(content2, MAX_CONTENT_LENGTH, fileID1);
        PrintString("Successfully concatenated two files. Result is put into the first one.");
    } else {
        PrintString("Either of the file is unavailable, please check if the file name is correctly spelled.");
    }

    Close(fileID1);
    Close(fileID2);

    Halt();
}