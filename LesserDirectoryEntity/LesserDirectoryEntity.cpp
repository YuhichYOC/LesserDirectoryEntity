#include "stdafx.h"

#include "LesserDirectoryEntity.h"

void LesserDirectoryEntity::SetFullPath(string arg)
{
    fullPath = arg;
}

void LesserDirectoryEntity::Fill()
{
    HANDLE ret;
    LPWIN32_FIND_DATA fileInfo = new WIN32_FIND_DATA();

    WCharString wc;
    unique_ptr<wchar_t> path = move(wc.Value(fullPath).ToWChar());
    ret = FindFirstFile((LPCWSTR)path.get(), fileInfo);
    if (ret != INVALID_HANDLE_VALUE) {
        do {
            if (!(fileInfo->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                unique_ptr<LesserFileEntity> add(new LesserFileEntity());
                add->Init(wc.Value(fileInfo->cFileName).ToString());
                files.push_back(move(add));
            }
        } while (FindNextFile(ret, fileInfo));
    }
}

void LesserDirectoryEntity::Delete(string arg)
{
    HANDLE ret;
    LPWIN32_FIND_DATA fileInfo = new WIN32_FIND_DATA();

    WCharString wc;
    unique_ptr<wchar_t> path = move(wc.Value(arg).ToWChar());
    ret = FindFirstFile((LPCWSTR)path.get(), fileInfo);
    if (ret != INVALID_HANDLE_VALUE) {
        do {
            if (fileInfo->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wc.WChar_tStartsWith(fileInfo->cFileName, string(".\0")) || wc.WChar_tStartsWith(fileInfo->cFileName, string(".."))) {
                    continue;
                }
                Delete(wc.Value(fileInfo->cFileName).ToString());
                RemoveDirectory((LPCWSTR)(wc.Value(fileInfo->cFileName).ToWChar().get()));
            }
            else {
                DeleteFile((LPCWSTR)(wc.Value(fileInfo->cFileName).ToWChar().get()));
            }
        } while (FindNextFile(ret, fileInfo));
    }
}

void LesserDirectoryEntity::Copy(string arg)
{
    dirCopyTo = arg;
    WCharString wc;
    unique_ptr<wchar_t> path = move(wc.Value(arg).ToWChar());
    int ret = CreateDirectory((LPCWSTR)path.get(), nullptr);
    if (ret != 0) {
        for (size_t i = 0; i < files.size(); i++) {
            string from = files.at(i)->GetFullPath();
            string to =
        }
    }
}

LesserDirectoryEntity::LesserDirectoryEntity()
{
}

LesserDirectoryEntity::~LesserDirectoryEntity()
{
}