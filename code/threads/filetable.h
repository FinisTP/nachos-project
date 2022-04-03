#include "filesys.h"
#include "bitmap.h"
#include "kernel.h"

#define MAX_FILE_COUNT 10

class FileTable{
public:
    FileTable() {
        openFile = new OpenFile*[MAX_FILE_COUNT];
        fileMode = new int[MAX_FILE_COUNT];
        openedFileMask = new Bitmap[MAX_FILE_COUNT];
        openedFileMask->Mark(0);
        openedFileMask->Mark(1);
        fileNames = new char*[MAX_FILE_COUNT];
    }

    ~FileTable() {
        for (int i = 0; i < MAX_FILE_COUNT; ++i) {
            delete [] openFile[i];
            delete [] fileNames[i];
        }
        delete[] openFile;
        delete[] fileMode;
        delete[] fileNames;
        delete[] openedFileMask;
    }

    int Create(char* fileName) {
       OpenFile *newFile = kernel->fileSystem->Create(fileName);
       return newFile ? 0 : -1;
    }

    int Open(char *fileName, int fileMode) {
        int availableFile = openedFileMask->FindAndSet();
        if (availableFile == -1) return -1;
        OpenFile *newFile = kernel->fileSystem->Open(fileName);
        if (newFile == NULL) return -1;
        openFile[availableFile] = newFile;
        fileNames[availableFile] = new char[strlen(fileName)+1];
        strcpy(fileNames[availableFile], fileName);
        fileMode[availableFile] = fileMode;
        return availableFile;
    }

    int Close(int id) {
        if (id <= 1) return -1;
        bool res = openedFileMask->Test(id);
        if (!res) return -1;
        kernel->fileSystem->Close(openFile[id]);
        delete openFile[id];
        delete [] fileNames[id];
        fileMode[id] = -1;
        openedFileMask->Clear();
        openFile[id] = NULL;
        fileNames[id] = NULL;
        return 0;
    }

    int Remove(char* name) {
        int id = -1;
        for (int i = 2; i < MAX_FILE_COUNT; ++i) {
            if (strcmp(fileNames[i], name) == 0) {
                id = i;
                break;
            }
        }
        if (id == -1) return -1;
        return Close(id);
    }

    int Read(char *buffer, int size, int id) {
        if (id <= 0 || id >= MAX_FILE_COUNT) return -1;
        if (fileMode[id] == 2) return -1;

        return openfile[id]->Read(buf, charcount);
    }

    int Write(char *buffer, int size, int id) {
        if (id < 0 || id >= MAX_FILE_COUNT || id == 1) return -1;
        if (fileMode[id] == 1) return -1;

        return openfile[id]->Write(buf, charcount);
    }

    int Seek(int position, int id)
    {
        if (id <= 1 || id >= MAX_FILE_COUNT) return -1;
        openFile[id]->Seek(position);
        return position;
    }

    int GetPosOfFile(int id) {
        if (id < 0 || id >= MAX_FILE_COUNT) return -1;
        return openFile[id]->CurrentPos;
    }

private:
    OpenFile** openFile;
    int* fileMode;
    char** fileNames;
    Bitmap* openedFileMask;
};