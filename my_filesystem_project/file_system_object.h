#pragma once
#include <string>
#include <filesystem>

using namespace std;
using namespace filesystem;

class FileSystemObject
{
public:
    virtual string GetName() = 0;
    virtual void SetName(const string& name) = 0;
    virtual void Create(const string& path) = 0;
    virtual void Remove(const string& name, const string& curDir) = 0;
    virtual void Rename(const string& name, const string& newName, const string& curDir) = 0;
    virtual uintmax_t GetSize(const string& name, const string& curDir) = 0;
    virtual void Copy(const string& curDir, const string& copyName, const string& destName) = 0;
    virtual ~FileSystemObject() {};
};
