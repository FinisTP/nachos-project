#include "filetable.h"
#include "main.h"

FileTable::FileTable() {
    openFile = new OpenFile*[MAX_FILE_COUNT];
    fileMode = new int[MAX_FILE_COUNT];
    openedFileMask = new Bitmap(MAX_FILE_COUNT);
    openedFileMask->Mark(0);
    openedFileMask->Mark(1);
    fileNames = new char*[MAX_FILE_COUNT];
}

FileTable::~FileTable() {
    for (int i = 0; i < MAX_FILE_COUNT; ++i) {
        if (openFile[i] != NULL) delete [] openFile[i];
        if (fileNames[i] != NULL) delete [] fileNames[i];
    }
    
    if (openFile != NULL) delete[] openFile;
    if (fileMode != NULL) delete[] fileMode;
    
    if (fileNames != NULL) delete[] fileNames;
    if (openedFileMask != NULL) delete openedFileMask;
}

    int FileTable::Create(char* fileName) {
       bool newFile = kernel->fileSystem->Create(fileName);
       return newFile;
    }

    int FileTable::Open(char *fileName, int fileMod) {
        int availableFile = openedFileMask->FindAndSet();
        if (availableFile == -1) return -1;
        OpenFile *newFile = kernel->fileSystem->Open(fileName);
        if (newFile == NULL) return -1;
        openFile[availableFile] = newFile;
        fileNames[availableFile] = new char[strlen(fileName)+1];
        strcpy(fileNames[availableFile], fileName);
        fileMode[availableFile] = fileMod;
        return availableFile;
    }

    int FileTable::Close(int id) {
        if (id <= 1) return -1;
        bool res = openedFileMask->Test(id);
        if (!res) return -1;
        // kernel->fileSystem->Remove(fileNames[id]);
        delete openFile[id];
        delete [] fileNames[id];
        fileMode[id] = -1;
        openedFileMask->Clear(id);
        openFile[id] = NULL;
        fileNames[id] = NULL;
        return 0;
    }

    int FileTable::Remove(char* name) {
        
        int id = -1;
        for (int i = 2; i < MAX_FILE_COUNT; ++i) {
            if (fileNames[i] != NULL && strcmp(fileNames[i], name) == 0) {
                id = i;
                break;
            }
        }
        if (id != -1) return -1;
        
        kernel->fileSystem->Remove(name);
        return 0;
    }

    int FileTable::Read(char *buffer, int size, int id) {
        if (id <= 0 || id >= MAX_FILE_COUNT) return -1;
        if (fileMode[id] == 2) return -1;
        if (openFile[id] == NULL) return -1;

        return openFile[id]->Read(buffer, size);
    }

    int FileTable::Write(char *buffer, int size, int id) {
        if (id < 0 || id >= MAX_FILE_COUNT || id == 1) return -1;
        if (fileMode[id] == 1) return -1;
        if (openFile[id] == NULL) return -1; 

        return openFile[id]->Write(buffer, size);
    }

    int FileTable::Seek(int position, int id)
    {
        if (id <= 1 || id >= MAX_FILE_COUNT) return -1;
        if (openFile[id] == NULL) return -1;
        openFile[id]->Seek(position);
        return openFile[id]->CurrentPos();
    }

    int FileTable::GetPosOfFile(int id) {
        if (id < 0 || id >= MAX_FILE_COUNT) return -1;
        if (openFile[id] == NULL) return -1;
        return openFile[id]->CurrentPos();
    }