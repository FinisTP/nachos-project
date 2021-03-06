// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "machine.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

void IncrementProgramCounter() {
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	
	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
}



void ExAdd() {
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
		
	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);
	
	/* Modify return point */
	IncrementProgramCounter();

	ASSERTNOTREACHED();
}

void ExReadNum() {
	int num = SysReadNum();
	kernel->machine->WriteRegister(2, num);
	IncrementProgramCounter();
}

void ExPrintNum() {
	int num = kernel->machine->ReadRegister(4);
	SysPrintNum(num);
	IncrementProgramCounter();
}

void ExReadChar() {
	char character = SysReadChar();
	kernel->machine->WriteRegister(2, (int)character);
	IncrementProgramCounter();
}

void ExPrintChar() {
	char character = (char)kernel->machine->ReadRegister(4);
	SysPrintChar(character);
	IncrementProgramCounter();
}

void ExRandomNum() {
	int num = SysRandomNum();

	printf("%i", num);

	kernel->machine->WriteRegister(2, num);
	IncrementProgramCounter();
}

void ExReadString() {
	int address = kernel->machine->ReadRegister(4);
	int len = kernel->machine->ReadRegister(5);
	if (len > 1000) {
		DEBUG(dbgSys, "Inputted string is too long.");
		SysHalt();
	}
	int result = SysReadString(address, len);

	kernel->machine->WriteRegister(2, result);
	IncrementProgramCounter();
}

void ExPrintString() {
	int address = kernel->machine->ReadRegister(4);
	int result = SysPrintString(address);
	kernel->machine->WriteRegister(2, result);
	IncrementProgramCounter();
}

void ExCreate() {
	int address = kernel->machine->ReadRegister(4);
	// int fileMode = kernel->machine->ReadRegister(5);
	char *fileName = User2System(address, 256);

	int res = SysCreate(fileName);
	kernel->machine->WriteRegister(2, res);

	delete fileName;

	IncrementProgramCounter();
}

void ExOpen() {
	int address = kernel->machine->ReadRegister(4);
	// int fileMode = kernel->machine->ReadRegister(5);
	char *fileName = User2System(address, 256);

	int res = SysOpen(fileName, 0);
	
	kernel->machine->WriteRegister(2, res);

	IncrementProgramCounter();
}

void ExClose() {
	int id = kernel->machine->ReadRegister(4);

	int res = SysClose(id);
	
	kernel->machine->WriteRegister(2, res);

	IncrementProgramCounter();
}

void ExSeek() {
	int position = kernel->machine->ReadRegister(4);
	int id = kernel->machine->ReadRegister(5);

	int res = SysSeek(position, id);
	
	kernel->machine->WriteRegister(2, res);

	IncrementProgramCounter();
}

void ExRemove() {
	int address = kernel->machine->ReadRegister(4);
	char *fileName = User2System(address, 256);

	int res = SysRemove(fileName);
	
	kernel->machine->WriteRegister(2, res);

	IncrementProgramCounter();
}

void ExRead() {
	int address = kernel->machine->ReadRegister(4);
	int size = kernel->machine->ReadRegister(5);
	int id = kernel->machine->ReadRegister(6);

	int res = SysRead(address, size, id);

	kernel->machine->WriteRegister(2, res);

	IncrementProgramCounter();
}

void ExWrite() {
	int address = kernel->machine->ReadRegister(4);
	int size = kernel->machine->ReadRegister(5);
	int id = kernel->machine->ReadRegister(6);

	int res = SysWrite(address, size, id);

	kernel->machine->WriteRegister(2, res);

	IncrementProgramCounter();
}

void ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
    case SyscallException:
      switch(type) {
      	case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			SysHalt();
			ASSERTNOTREACHED();
			break;

      	case SC_Add:
			ExAdd();
			break;

		case SC_ReadNum:
			ExReadNum();
			return;
			break;

		case SC_PrintNum:
			ExPrintNum();
			return;
			break;

		case SC_ReadChar:
			ExReadChar();
			return;
			break;

		case SC_PrintChar:
			ExPrintChar();
			return;
			break;

		case SC_RandomNum:
			ExRandomNum();
			return;
			break;
		
		case SC_ReadString:
			ExReadString();
			return;
			break;

		case SC_PrintString:
			ExPrintString();
			return;
			break;

		case SC_Create:
			ExCreate();
			return;
			break;

		case SC_Open:
			ExOpen();
			return;
			break;

		case SC_Close:
			ExClose();
			return;
			break;

		case SC_Remove:
			ExRemove();
			return;
			break;

		case SC_Seek:
			ExSeek();
			return;
			break;

		case SC_Read:
			ExRead();
			return;
			break;

		case SC_Write:
			ExWrite();
			return;
			break;
			
      	default:
			cerr << "Unexpected system call " << type << "\n";
			break;
      }
      break;

	case PageFaultException:
	case ReadOnlyException:
	case BusErrorException:
	case AddressErrorException:
	case OverflowException:
	case IllegalInstrException:
	case NumExceptionTypes:
		cerr << "Unexpected error occurs: " << which << endl;
		SysHalt();
		ASSERTNOTREACHED();
	

    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
      break;
    }
    ASSERTNOTREACHED();
}
