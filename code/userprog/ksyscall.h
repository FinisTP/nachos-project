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
#define MAX_BUFFER_SIZE 255

char* User2System(int virtAddr, int limit) {
	int i, oneChar;
	char *kernelBuf = NULL;

	kernelBuf = new char[limit+1];
	if (kernelBuf == NULL) return kernelBuf;

	memset(kernelBuf, 0, limit+1);
	for (i = 0; i < limit; ++i) {
		kernel->machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0) break;
	}
	return kernelBuf;
}

int System2User(int virtAddr, int len, char* buffer) {
	if (len < 0) return -1;
	if  (len == 0) return len;
	int i = 0, oneChar = 0;
	do {
		oneChar = (int) buffer[i];
		kernel->machine->WriteMem(virtAddr+i,1,oneChar);
		i++;
	} while (i < len && oneChar != 0);

	return i;
}

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

int SysReadString(int address, int length) {
  char *buffer;
	int count = 0, i;

	if (length <= 0) return -1;

	buffer = new char[length + 1];
	buffer[length] = '\0';

	for (i = 0; i < length; i++)
	{
		char read = kernel->synchConsoleIn->GetChar();
		if (read == '\n' || read == '\0') break;
		buffer[i] = read;
		count += 1;
	}

	buffer[i] = '\0';
  

	System2User(address, length, buffer);

	delete[] buffer;
	return i;
}

int SysPrintString(int address) {
  char *buffer;
	int i = 0;

	buffer = User2System(address, MAX_BUFFER_SIZE + 1);

  cout << "HERLLO " << buffer << endl;

	while (buffer[i] != '\0')
		{
      kernel->synchConsoleOut->PutChar(buffer[i]);
      i++;
    }

	delete[] buffer;

	return i;
}

int SysCreate(char *fileName) {
  return kernel->fileTable->Create(fileName);
}

int SysRemove(char *fileName) {
  return kernel->fileTable->Remove(fileName);
}

int SysOpen(char *fileName, int openMode) {
  return kernel->fileTable->Open(fileName, openMode);
}

int SysRead(int address, int size, int id) {
  char *buffer = new char[size];
	buffer = User2System(address, size);
  int newPos, oldPos;
	oldPos = kernel->fileTable->GetPosOfFile(id);
	if (kernel->fileTable->Read(buffer, size, id) > 0) {
		newPos = kernel->fileTable->GetPosOfFile(id);

    int newSize = -1;
    for (int i = 0; i < size; ++i) {
      if (buffer[i] == '\0') {
        newSize = i;
      }
    }
    if (newSize > -1) {
      char *newBuffer = new char[newSize+1];
      for (int i = 0; i <= newSize; ++i) newBuffer[i] = buffer[i];
      delete [] buffer; buffer = NULL;
      buffer = new char[newSize+1];
      strcpy(buffer, newBuffer);
      size = newSize;
    }

		System2User(address, size, buffer);
		delete [] buffer;
		return newPos - oldPos + 1;
	} else {
    delete [] buffer;
    return -1;
  }
}

int SysWrite(int address, int size, int id) {
  char *buffer = new char[size];
	buffer = User2System(address, size);

  int newPos, oldPos;
	oldPos = kernel->fileTable->GetPosOfFile(id);
	if (kernel->fileTable->Write(buffer, size, id) > 0) {
		newPos = kernel->fileTable->GetPosOfFile(id);

    int newSize = -1;
    for (int i = 0; i < size; ++i) {
      if (buffer[i] == '\0') {
        newSize = i;
      }
    }
    if (newSize > -1) {
      char *newBuffer = new char[newSize+1];
      for (int i = 0; i <= newSize; ++i) newBuffer[i] = buffer[i];
      delete [] buffer; buffer = NULL;
      buffer = new char[newSize+1];
      strcpy(buffer, newBuffer);
      size = newSize;
    }

		System2User(address, size, buffer);
		delete [] buffer;
		return newPos - oldPos + 1;
	} else {
    delete [] buffer;
    return -1;
  }
}

int SysSeek(int position, int id) {
  return kernel->fileTable->Seek(position ,id);
}

int SysClose(int id) {
  return kernel->fileTable->Close(id);
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
