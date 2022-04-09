#include "syscall.h"
#include "copyright.h"

#define MAX_FILE_LENGTH 32

int main() {
    char content[MAX_FILE_LENGTH + 1];
    int a,b;
    if (Create("TanPhat.txt") == 1) {
        a = Open("HaiDuong.txt");
        b = Open("TanPhat.txt");
        Read(content, MAX_FILE_LENGTH, a);
        Write(content, MAX_FILE_LENGTH, b);
        Close(a);
        Close(b);
        PrintString("\nCreate file successfully\n");
        
    } else {
        PrintString("\nError\n");
    }

    Halt();
}