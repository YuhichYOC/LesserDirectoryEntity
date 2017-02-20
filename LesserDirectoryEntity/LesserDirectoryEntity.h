#pragma once

#include "stdafx.h"

#include "WCharString.h"

#include "LesserFileEntity.h"

class LesserDirectoryEntity
{
private:

    string fullPath;

    vector<unique_ptr<LesserFileEntity>> files;

    string dirCopyTo;

public:

    void SetFullPath(string arg);

    void Fill();

    bool Delete(string arg);

    void Copy(string arg);

    void CopyRollback();

    LesserDirectoryEntity();

    ~LesserDirectoryEntity();
};
