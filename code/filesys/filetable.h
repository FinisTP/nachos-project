#ifndef FILETABLE_H
#define FILETABLE_H
#include "openfile.h"
#include "sysdep.h"

#define MAX_FILE_COUNT 10
#define INPUT 0
#define OUTPUT 1

class FileTable {
   private:
    OpenFile** openFile;
    int* fileOpenMode;
    char** fileNames;

   public:
    FileTable() {
        openFile = new OpenFile*[MAX_FILE_COUNT];
        fileOpenMode = new int[MAX_FILE_COUNT];
        fileNames = new char*[MAX_FILE_COUNT];
        for (int i = 0; i < MAX_FILE_COUNT; ++i) {
            fileOpenMode[i] = -1;
            openFile[i] = NULL;
            fileNames[i] = NULL;
        }
        fileOpenMode[INPUT] = 1;
        fileOpenMode[OUTPUT] = 2;
        fileNames[INPUT] = "INPUT.txt";
        fileNames[OUTPUT] = "OUTPUT.txt";
    }

    int Open(char* fileName, int openMode) {
        int availableFile = -1;
        int fileAttribute = -1;
        for (int i = 2; i < MAX_FILE_COUNT; i++) {
            if (openFile[i] == NULL) {
                availableFile = i;
                break;
            }
        }
        if (availableFile == -1) return -1;
        if (openMode == 0) fileAttribute = OpenForReadWrite(fileName, FALSE);
        if (openMode == 1) fileAttribute = OpenForRead(fileName, FALSE);
        if (fileAttribute == -1) return -1;
        
        openFile[availableFile] = new OpenFile(fileAttribute);
        fileOpenMode[availableFile] = openMode;
        fileNames[availableFile] = new char[strlen(fileName) + 1];
        strcpy(fileNames[availableFile], fileName);

        return availableFile;
    }

    int Close(int fileIndex) {
        if (fileIndex < 2 || fileIndex >= MAX_FILE_COUNT) return -1;
        if (openFile[fileIndex] != NULL) {
            delete openFile[fileIndex];
            openFile[fileIndex] = NULL;
            return 0;
        }
        return -1;
    }

    int Read(char* buffer, int charCount, int fileIndex) {
        if (fileIndex >= MAX_FILE_COUNT) return -1;
        if (openFile[fileIndex] == NULL || fileOpenMode[fileIndex] == 2) return -1;
        int result = openFile[fileIndex]->Read(buffer, charCount);
        if (result != charCount) return -2;
        return result;
    }

    int Write(char* buffer, int charCount, int fileIndex) {
        if (fileIndex >= MAX_FILE_COUNT) return -1;
        if (openFile[fileIndex] == NULL || fileOpenMode[fileIndex] == 1) return -1;
        return openFile[fileIndex]->Write(buffer, charCount);
    }

    int Seek(int position, int fileIndex) {
        // Check index in bound, from console out onward
        if (fileIndex <= 1 || fileIndex >= MAX_FILE_COUNT) return -1;
        if (openFile[fileIndex] == NULL) return -1;
        if (position == -1) position = openFile[fileIndex]->Length();
        if (position < 0 || position > openFile[fileIndex]->Length()) return -1;
        return openFile[fileIndex]->Seek(position);
    }

    bool Remove(char *fileName) {
        if (strcmp(fileName, "INPUT.txt") == 0 || strcmp(fileName, "OUTPUT.txt") == 0) return false;
        for (int i = 2; i < MAX_FILE_COUNT; ++i) {
            if (strcmp(fileName, fileNames[i]) == 0) {
                if (openFile[i] != NULL) {
                    delete openFile[i];
                    openFile[i] = NULL;
                    return true;
                }
            }
        }
        return false;
    }

    ~FileTable() {
        for (int i = 0; i < MAX_FILE_COUNT; i++) {
            if (openFile[i] != NULL) delete openFile[i];
        }
        if (openFile != NULL){
            delete[] openFile;
            openFile = NULL;
        }
        if (fileOpenMode != NULL) {
            delete[] fileOpenMode;
            fileOpenMode = NULL;
        }
    }
};

#endif