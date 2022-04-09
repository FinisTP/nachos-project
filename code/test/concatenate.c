#include "syscall.h"

int main()
{
    int fileID1, fileID2;
    char* fileName;
    char* content1, *content2;

    fileID1 = Open("testConcat1.txt");
    fileID1 = Open("testConcat2.txt");


    
    Halt();
}