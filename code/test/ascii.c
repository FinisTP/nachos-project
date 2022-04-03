#include "syscall.h"

void PrintHex(int num)
{
  // Converter tools from integer to hexadecimal
  int hexDigit2 = num & 0xF;
  int hexDigit1 = num >> 4;

  switch (hexDigit1)
  {
  case 10:
    PrintChar('A');
    break;
  case 11:
    PrintChar('B');
    break;
  case 12:
    PrintChar('C');
    break;
  case 13:
    PrintChar('D');
    break;
  case 14:
    PrintChar('E');
    break;
  case 15:
    PrintChar('F');
    break;
  default:
    PrintNum(hexDigit1);
    break;
  }
  switch (hexDigit2)
  {
  case 10:
    PrintChar('A');
    break;
  case 11:
    PrintChar('B');
    break;
  case 12:
    PrintChar('C');
    break;
  case 13:
    PrintChar('D');
    break;
  case 14:
    PrintChar('E');
    break;
  case 15:
    PrintChar('F');
    break;
  default:
    PrintNum(hexDigit2);
    break;
  }
}

int main()
{
    int i;
    // Print all the char in readable range
    PrintString("CHAR --> DEC --> HEX\n");
    for (i = 33; i <= 126; ++i)
    {
        PrintChar((char)i);
        PrintString(" --> ");
        PrintNum(i);
        PrintString(" --> ");
        PrintHex(i);
        PrintChar('\n');
    }
    Halt();
}