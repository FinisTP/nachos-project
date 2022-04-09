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
#define MAX_BUFFER_SIZE 512

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
  bool negative = false;
  int res = 0;
  char inp = kernel->synchConsoleIn->GetChar();
  while (inp == ' ') inp = kernel->synchConsoleIn->GetChar();

  if (inp == '-') {
    negative = true;
    inp = kernel->synchConsoleIn->GetChar();
  }

  while (true) {
    if (inp >= '0' && inp <= '9') res = res * 10 + (int)(inp - '0');
    else if (inp == ' ' || inp == '\n') break;
    else return 0;

    inp = kernel->synchConsoleIn->GetChar();
  }

  if (negative) return res * (-1);
  else return res;
}

void SysPrintNum(int num) {
  if (num == 0) {
    kernel->synchConsoleOut->PutChar('0');
    return;
  }

  if (num < 0) {
    kernel->synchConsoleOut->PutChar('-');
    num = num * (-1);
  }

  int temp = num, countDigit = 0;
  while (temp > 0) {
    countDigit += 1;
    temp /= 10;
  }

  char digits[11] = {0};
  for (int i = countDigit - 1; i >= 0; --i) {
    digits[i] = (num % 10) + '0';
    num = num / 10;
  }

  for (int i = 0; i < countDigit; ++i)
    kernel->synchConsoleOut->PutChar(digits[i]);
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
	int count = 0, i = 0;

	if (length <= 0) return -1;

	buffer = new char[length + 1];
	buffer[length] = '\0';

	for (; i < length; i++)
	{
		char read = kernel->synchConsoleIn->GetChar();
		if (read == '\n' || read == '\0') break;
		buffer[i] = read;
		count += 1;
	}

	buffer[i] = '\0';
  

	System2User(address, length, buffer);

	delete[] buffer;
	return count;
}

int SysPrintString(int address) {
  char *buffer;
	int i = 0;

	buffer = User2System(address, MAX_BUFFER_SIZE + 1);

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
        break;
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
  if (buffer == NULL) return -1;

  int newSize = -1;
  for (int i = 0; i < size; ++i) {
    if (buffer[i] == '\0') {
      newSize = i;
      break;
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

  int newPos, oldPos;
	oldPos = kernel->fileTable->GetPosOfFile(id);
	if (kernel->fileTable->Write(buffer, size, id) > 0) {
		newPos = kernel->fileTable->GetPosOfFile(id);
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
