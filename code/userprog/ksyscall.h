/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"
#include "limits.h"
#include "machine.h"
#include <bitset>

/* Helper functions */

// void bubbleSort(bool ascending, int *&arr, int length) {
//   for (int i = 0; i < length - 1; ++i) {
//     for (int j = 1; j < length; ++j) {
//       if (ascending && arr[i] > arr[j]) swap(arr[i], arr[j]);
//       else if (!ascending && arr[i] < arr[j]) swap(arr[i], arr[j]);
//     }
//   }
//   SysPrintString("Sorted array: \n");
//   for (int i = 0; i < length; ++i) 
//   {
//     SysPrintNum(arr[i]);
//     SysPrintChar(' ');
//   }
//   SysPrintChar('\n');
// }

bool isnumber(char c) {
  return isdigit(c) || c == '.' || c == '-' || c == '+';
}


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysReadNum() {
  char number[13]; int i = 0;
  for (i = 0; i < 13; ++i) number[i] = 0;
  char c = kernel->synchConsoleIn->GetChar();

  if (c == EOF || !isnumber(c)) {
    DEBUG(dbgSys, "Inputted number is invalid.");
    return 0;
  }
  i = 0;
  while (c != EOF && isnumber(c)) {
    number[i] = c;
    i++;
    if (i > 11) {
      DEBUG(dbgSys, "Number exceeds integer capacity");
      return 0;
    }
    c = kernel->synchConsoleIn->GetChar();
    // i is now the length of inputted integer
  }
  bool negative = number[0] == '-';
  int k = 0; long result = 0;
  if (negative) k = 1;
  while (k < i && number[k] == 0) k++;
  for (;k < i; ++k) {
    c = number[k];
    if (!isdigit(c)) {
      DEBUG(dbgSys, "Inputted number is invalid, " << c << " detected.");
      return 0;
    }
    result = result * 10 + (c - '0');
  }

  if (negative) result = -result;
  if (result < INT_MIN) result = INT_MIN;
  else if (result > INT_MAX) result = INT_MAX;

  return (int)result;
}

void SysPrintNum(int number) {
  if (number == 0) 
  {
    kernel->synchConsoleOut->PutChar('0');
    return;
  }

  long cover = number;
  if (cover < 0) {
    kernel->synchConsoleOut->PutChar('-');
    cover = -cover;
  }
  string result = "";
  while (cover > 0) {
    result = (char)((cover % 10) + '0') + result;
    cover /= 10;
  }
  for (int i = 0; i < result.length(); ++i)
    kernel->synchConsoleOut->PutChar(result[i]);

  // SysHalt();
}

char SysReadChar() {
  char c = kernel->synchConsoleIn->GetChar();
  return c;
}

void SysPrintChar(char character) {
  kernel->synchConsoleOut->PutChar(character);
  // SysHalt();
}

int SysRandomNum() {
  // srand(time(NULL));
  // return (int)(INT_MIN + (long)rand() % (INT_MAX - INT_MIN));
  return random() % (INT_MAX - INT_MIN) + INT_MIN;
}

void SysReadString(char*& buffer, int length) {
  if (buffer != NULL) delete[] buffer;
  buffer = new char[length + 1];
  buffer[length] = '\0';
  for (int i = 0; i < length; ++i) 
    buffer[i] = kernel->synchConsoleIn->GetChar();
  // SysHalt();
}

void SysPrintString(char* buffer) {
  int i = 0;
  while (buffer[i] != '\0') kernel->synchConsoleOut->PutChar(buffer[i++]);
  // SysHalt();
}

// void SysPrintHelp() {
//   SysPrintChar('\n');
//   SysPrintString("<-----HELP DOCUMENT----->\n");
//   SysPrintChar('\n');
//   SysPrintString("Group members: \n");
//   SysPrintString("+ 19125064 - Tu Tan Phat\n");
//   SysPrintString("+ 19125086 - Tran Hai Duong\n");
//   SysPrintChar('\n');
//   SysPrintString("Description of the [ascii] and [sort] functions: \n");
//   SysPrintString("- [ascii] and [sort] are two system calls (or functions).\n");
//   SysPrintString("- [ascii] allows users to see the ASCII table, including the character, decimal and hexadecimal values (Note that some characters cannot be printed out).\n");
//   SysPrintString("- [sort] allows users to input an array and sort them either ascending or descending.\n");
//   SysPrintChar('\n');
// }

// void SysPrintASCII() 
// {
//   SysPrintString("CHAR --> DEC --> HEX\n");
//   for (int i = 33; i <= 126; ++i) {
//     SysPrintChar((char)i);
//     SysPrintString(" --> ");
//     SysPrintNum(i);
//     SysPrintString(" --> ");
//     SysPrintHex(i);
//     SysPrintChar('\n');
//   }
// }

// void SysSort() {
//   SysPrintString("Please input the length of the array: ");
//   int len = SysReadNum();
//   while (len == 0 || len > 100) {
//     SysPrintString("Length of the array must be a valid non-zero integer that is less than or equal to 100).\n");
//     SysPrintString("Please input the length of the array: ");
//     len = SysReadNum();
//   }
//   int *arr = new int[len];
//   for (int i = 0; i < len; ++i) {
//     SysPrintString("Please input the next integer: ");
//     arr[i] = SysReadNum();
//   }
//   bool sortAscending = true;
//   SysPrintString("Do you want to sort this array ascending? ('y': ascending, 'n': descending) ");
//   char c = SysReadChar();
//   while (tolower(c) != 'y' && tolower(c) != 'n') {
//     SysPrintString("Do you want to sort this array ascending? ('y': ascending, 'n': descending) ");
//     c = SysReadChar();
//   }
//   if (tolower(c) == 'n') sortAscending = false;
  
//   bubbleSort(sortAscending, arr, len);
// }





#endif /* ! __USERPROG_KSYSCALL_H__ */
