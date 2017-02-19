#include "stdafx.h"
#include "LesserFileEntity.h"

void LesserFileEntity::Init(string arg)
{
    fullPath = arg;
}

string LesserFileEntity::GetFullPath()
{
    return fullPath;
}

void LesserFileEntity::ReadFile()
{
    if (writePrepared) {
        return;
    }
    ifile = ifstream(fullPath, ios::in | ios::binary);
    ifile.seekg(0, istream::end);
    streamoff endPos = ifile.tellg();
    ifile.seekg(0, istream::beg);
    streamoff startPos = ifile.tellg();
    fileSize = (int)(endPos - startPos);
    readPrepared = true;
    char * readBuffer = new char[fileSize];
    ifile.read(readBuffer, fileSize);
    fileContents = readBuffer;
    ifile.close();
}

int LesserFileEntity::GetFileSize()
{
    return fileSize;
}

char * LesserFileEntity::GetFileContents()
{
    return fileContents;
}

void LesserFileEntity::SetFileSize(int arg)
{
    fileSize = arg;
}

void LesserFileEntity::SetFileContents(char * arg)
{
    fileContents = arg;
}

void LesserFileEntity::WriteFile()
{
    if (readPrepared) {
        return;
    }
    ofile = ofstream(fullPath, ios::out | ios::binary);
    writePrepared = true;
    ofile.write(fileContents, fileSize);
    ofile.close();
}

LesserFileEntity::LesserFileEntity()
{
    readPrepared = false;
    writePrepared = false;
}

LesserFileEntity::~LesserFileEntity()
{
}