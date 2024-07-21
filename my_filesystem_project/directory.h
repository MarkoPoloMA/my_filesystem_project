#pragma once
#include "file_system_object.h"
#include <vector>

class Directory : public FileSystemObject
{
public:
    string name;
    vector<FileSystemObject*> objects;

public:
    Directory(const string& name);

    string GetName() override;
    void SetName(const string& name) override;
    void Create(const string& path) override;
    void Remove(const string& fileName, const string& curDir) override;
    void Rename(const string& oldName, const string& newName, const string& curDir) override;
    uintmax_t GetSize(const string& fileName, const string& curDir) override;
    void Copy(const string& curDir, const string& copyName, const string& destName) override;
    bool IsRepeatDirectory(const string& directory, const string& targetDirectory);
    void Read(const string& directory);
    void List();
}; #pragma once
