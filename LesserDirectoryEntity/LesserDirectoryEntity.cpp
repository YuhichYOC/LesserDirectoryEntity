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
    unique_ptr<wchar_t> path = move(wc.Value(fullPath).Append("\\*.*").ToWChar());
    ret = FindFirstFile((LPCWSTR)path.get(), fileInfo);
    if (ret != INVALID_HANDLE_VALUE) {
        do {
            if (fileInfo->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                continue;
            }
            else {
                unique_ptr<LesserFileEntity> add(new LesserFileEntity());
                add->Init(wc.Value(fullPath).Append("\\").Append(fileInfo->cFileName).ToString());
                files.push_back(move(add));
            }
        } while (FindNextFile(ret, fileInfo));
    }
}

bool LesserDirectoryEntity::Delete(string arg)
{
    HANDLE ret;
    LPWIN32_FIND_DATA fileInfo = new WIN32_FIND_DATA();

    WCharString wc;
    unique_ptr<wchar_t> path = move(wc.Value(arg).Append("\\*.*").ToWChar());
    ret = FindFirstFile((LPCWSTR)path.get(), fileInfo);
    if (ret != INVALID_HANDLE_VALUE) {
        do {
            if (fileInfo->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wc.WChar_tStartsWith(fileInfo->cFileName, string(".\0")) || wc.WChar_tStartsWith(fileInfo->cFileName, string(".."))) {
                    continue;
                }
                string deleteArg = wc.Value(arg).Append("\\").Append(fileInfo->cFileName).ToString();
                if (!Delete(deleteArg)) {
                    return false;
                }
            }
            else {
                string deleteArg = wc.Value(arg).Append("\\").Append(fileInfo->cFileName).ToString();
                if (DeleteFile((LPCWSTR)(wc.Value(deleteArg).ToWChar().get())) == 0) {
                    return false;
                }
            }
        } while (FindNextFile(ret, fileInfo));
    }
    if (RemoveDirectory((LPCWSTR)(wc.Value(arg).ToWChar().get())) == 0) {
        cout << wc.SysErrMessage() << "\n";
        return false;
    }
    return true;
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
            string to = arg;
            to.append("\\");
            to.append(fullPath.substr(fullPath.find_last_of("\\") + 1));
            DWORD attr = GetFileAttributes((LPCWSTR)wc.Value(to).ToWChar().get());
            if (!(attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))) {
                ret = CreateDirectory((LPCWSTR)wc.Value(to).ToWChar().get(), nullptr);
                if (ret == 0) {
                    return;
                }
            }
            to.append("\\");
            to.append(files.at(i)->GetFullPath().substr(files.at(i)->GetFullPath().find_last_of("\\") + 1));
            unique_ptr<wchar_t> wfrom = move(wc.Value(from).ToWChar());
            unique_ptr<wchar_t> wto = move(wc.Value(to).ToWChar());
            ret = CopyFile((LPCWSTR)wfrom.get(), (LPCWSTR)wto.get(), true);
            if (ret == 0) {
                return;
            }
        }
    }
}

void LesserDirectoryEntity::CopyRollback()
{
    Delete(dirCopyTo);
}

LesserDirectoryEntity::LesserDirectoryEntity()
{
}

LesserDirectoryEntity::~LesserDirectoryEntity()
{
}