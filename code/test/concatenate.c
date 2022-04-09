#include "syscall.h"

int main()
{
    int fileID1, fileID2, newFileID;
    char *fileName;
    char *content1, *content2;

    fileID1 = Open("testConcat1.txt");
    fileID1 = Open("testConcat2.txt");

    if (fileID1 != -1 || fileID2 != -1)
    {
        Read(content1, 12, fileID1);
        Read(content2, 12, fileID2);

        Create("concatinatedFile.txt");

        newFileID = Open("concatinatedFile.txt");

        Write(content1, 12, newFileID);
        Write(content2, 12, newFileID);

        Close(newFileID);
    }

    Close(fileID1);
    Close(fileID2);

    Halt();
}