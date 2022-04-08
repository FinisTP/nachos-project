#include "syscall.h"

int main()
{
    int fileID;
    char *content;
    int write_result;
    int res;

    fileID = Open("leGiaBao.txt");

    write_result = Write("gau gau gau", 11, fileID);


    res = Close(fileID);

    Halt();
}