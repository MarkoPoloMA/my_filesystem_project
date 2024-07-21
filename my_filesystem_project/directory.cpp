#pragma once
#include <fstream>
#include <iostream>

#include "file.h"
#include "directory.h"

Directory::Directory(const string& name) : name(name) {}

bool Directory::IsRepeatDirectory(const string& directory, const string& targetDirectory) {
    if (exists(directory) && is_directory(directory))
        for (const auto& entry : directory_iterator(directory))
            if (entry.path().filename().string() == targetDirectory)
                return true;

    return false;
}

void Directory::Create(const string& path) {
    if (!create_directories(path + "/" + name))
        cerr << "Ошибка: Не удалось создать папку " << path << endl;
}

string Directory::GetName() {
    return name;
}

void Directory::SetName(const string& name) {
    this->name = name;
}

void Directory::Copy(const string& curDir, const string& copyName, const string& destName) {
    create_directories(curDir + "/" + destName);

    for (const auto& entry : directory_iterator(curDir + "/" + copyName)) {
        auto relative_path = relative(entry.path(), curDir + "/" + copyName);
        auto target_path = (curDir + "/" + destName) / relative_path;

        if (entry.is_regular_file()) {
            copy_file(entry.path(), target_path);
        }
        else if (entry.is_directory()) {
            create_directory(target_path);
        }
    }
}

void Directory::Read(const string& directory) {
    try {
        for (const auto& entry : directory_iterator(directory)) {
            if (is_regular_file(entry)) {
                objects.push_back(new File(entry.path().filename().string()));
            }
            else if (is_directory(entry)) {
                objects.push_back(new Directory(entry.path().filename().string()));
            }
            else {
                throw "Ошибка при чтении файла";
            }
        }
        List();
    }
    catch (const filesystem_error& msg) {
        cout << "Ошибка чтения директории: " << msg.what() << endl;
    }
}

void Directory::List()
{
    for (int i = 0; i < objects.size(); i++) {
        if (is_regular_file(objects[i]->GetName()))
            cout << i + 1 << " - " << "FILE: " << objects[i]->GetName() << endl;
        else
            cout << i + 1 << " - " << "DIR: " << objects[i]->GetName() << endl;
    }
    cout << endl;
}

void Directory::Remove(const string& folderName, const string& curDir)
{
    if (is_directory(curDir + "/" + folderName)) {
        path newCur = curDir + "/" + folderName;
        for (const auto& entry : directory_iterator(newCur)) {
            if (is_regular_file(entry)) {
                remove(entry);
            }
            else if (is_directory(entry)) {
                Remove(entry.path().filename().string(), newCur.string());
            }
            else {
                cout << "Такого файла нет" << endl;
            }
        }
        remove(newCur);
    }
    else
        throw "Неверный путь";
}

uintmax_t Directory::GetSize(const string& fileName, const string& curDir) {
    uintmax_t totalSize = 0;
    for (const auto& entry : recursive_directory_iterator(curDir + "/" + fileName)) {
        if (is_regular_file(entry)) {
            totalSize += file_size(entry);
        }
        else if (is_directory(entry)) {
            totalSize += GetSize(entry.path().filename().string(), entry.path().parent_path().string());
        }
    }
    return totalSize;
}

void Directory::Rename(const string& oldName, const string& newName, const string& curDir) {
    if (!exists(curDir + "/" + oldName))
        throw runtime_error("Исходного файла/директории не существует: " + oldName);

    rename(curDir + "/" + oldName, curDir + "/" + newName);
}