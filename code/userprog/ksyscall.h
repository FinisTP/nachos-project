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
  // if there is no number inputted or it is not considered to be a number
  if (c == EOF || !isnumber(c)) {
    DEBUG(dbgSys, "Inputted number is invalid.");
    return 0;
  }
  i = 0;
  // iterate and get more digit until end of input
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
  // check if negative
  bool negative = number[0] == '-';
  int k = 0; long result = 0;
  if (negative) k = 1;
  while (k < i && number[k] == 0) k++;
  // form the number using the formula result * 10 + (c - '0')
  for (;k < i; ++k) {
    c = number[k];
    if (!isdigit(c)) {
      DEBUG(dbgSys, "Inputted number is invalid, " << c << " detected.");
      return 0;
    }
    result = result * 10 + (c - '0');
  }

  if (negative) result = -result;
  // The result must be contained within 32-bit integer
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
  // convert int to long to avoid int overflow
  long cover = number;
  // if negative number then make it positive and print - first
  if (cover < 0) {
    kernel->synchConsoleOut->PutChar('-');
    cover = -cover;
  }
  string result = "";
  // print digits backward
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

bool SysCreateFile(char* fileName) {
    bool success;
    int fileNameLength = strlen(fileName);

    if (fileNameLength == 0) {
        DEBUG(dbgSys, "\nFile name can't be empty");
        success = false;

    } else if (fileName == NULL) {
        DEBUG(dbgSys, "\nNot enough memory in system");
        success = false;

    } else {
        DEBUG(dbgSys, "\nFile's name read successfully");
        if (!kernel->fileSystem->Create(fileName)) {
            DEBUG(dbgSys, "\nError creating file");
            success = false;
        } else {
            success = true;
        }
    }

    return success;
}

int SysOpen(char* fileName, int fileMode) {
    if (fileMode == 2) return -1;

    int id = kernel->fileSystem->Open(fileName, fileMode);
    if (id == -1) return -1;
    DEBUG(dbgSys, "\nSuccessfully opened file.");
    return id;
}

int SysClose(int fileMode) { return kernel->fileSystem->Close(fileMode); }

int SysRead(char* buffer, int charCount, int openFileID) {
    if (openFileID == 0) {
        return kernel->synchConsoleIn->GetString(buffer, charCount);
    }
    return kernel->fileSystem->Read(buffer, charCount, openFileID);
}

int SysWrite(char* buffer, int charCount, int openFileID) {
    if (openFileID == 1) {
        return kernel->synchConsoleOut->PutString(buffer, charCount);
    }
    return kernel->fileSystem->Write(buffer, charCount, openFileID);
}

int SysSeek(int seekPos, int openFileID) {
    if (openFileID <= 1) {
        DEBUG(dbgSys, "\nError detected: attempting to seek in console mode.");
        return -1;
    }
    return kernel->fileSystem->Seek(seekPos, openFileID);
}

int SysRemove(char *fileName) { return kernel->fileSystem->Remove(fileName); }

#endif /* ! __USERPROG_KSYSCALL_H__ */
