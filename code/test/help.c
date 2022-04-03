#include "syscall.h"

int main()
{
    PrintChar('\n');
    PrintString("<-----HELP DOCUMENT----->\n");
    PrintChar('\n');
    // Information of the team
    PrintString("Group members: \n");
    PrintString("+ 19125064 - Tu Tan Phat\n");
    PrintString("+ 19125086 - Tran Hai Duong\n");
    PrintChar('\n');

    // Short description of two functions
    PrintString("Description of the [ascii] and [sort] functions: \n");
    PrintString("- [ascii] and [sort] are two tem calls (or functions).\n");
    PrintString("- [ascii] allows users to see the ASCII table, including the character, decimal and hexadecimal values (Note that some characters cannot be printed out).\n");
    PrintString("- [sort] allows users to input an array and sort them either ascending or descending.\n");
    PrintChar('\n');
    Halt();
}