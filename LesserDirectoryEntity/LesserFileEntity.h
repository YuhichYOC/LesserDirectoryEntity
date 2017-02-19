#pragma once

#include "stdafx.h"

#include "WCharString.h"

class LesserFileEntity
{
private:

    string fullPath;

    ifstream ifile;

    ofstream ofile;

    int fileSize;

    char * fileContents;

    bool readPrepared;

    bool writePrepared;

public:

    void Init(string arg);

    string GetFullPath();

    void ReadFile();

    int GetFileSize();

    char * GetFileContents();

    void SetFileSize(int arg);

    void SetFileContents(char * arg);

    void WriteFile();

    LesserFileEntity();

    ~LesserFileEntity();
};
