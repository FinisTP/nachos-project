#ifndef FILETABLE_H
#define FILETABLE_H
#include "openfile.h"
#include "bitmap.h"

#define MAX_FILE_COUNT 10

class FileTable{
public:
    FileTable();

    ~FileTable();

    int Open(char *fileName, int fileMod);
    int Close(int id);
    int Create(char* fileName);

    int Remove(char* name);

    int Read(char *buffer, int size, int id);
    int Write(char *buffer, int size, int id);

    int Seek(int position, int id);

    int GetPosOfFile(int id);

private:
    OpenFile** openFile;
    int* fileMode;
    char** fileNames;
    Bitmap* openedFileMask;
};

#endif