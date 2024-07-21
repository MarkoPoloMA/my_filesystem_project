#pragma once
#include "file.h"
#include<iostream>
#include <fstream>
using namespace std;

File::File(string name) : name(name) {}

string File::GetName() {
    return name;
}

void File::SetName(const string& name) {
    this->name = name;
}

void File::Copy(const string& curDir, const string& copyName, const string& destName) {
    try {
        copy_file(curDir + "/" + copyName, curDir + "/" + destName);
        cout << "Файл скопирован!" << endl;
    }
    catch (const filesystem_error& msg) {
        cerr << "Ошибка копирования файла: " << msg.what() << endl;
    }
    system("pause");
}

uintmax_t File::GetSize(const string& oldName, const string& curDir) {
    uintmax_t totalSize = 0;
    if (is_regular_file(curDir + "/" + oldName))
        totalSize += file_size(curDir + "/" + oldName);

    return totalSize;
}

void File::Create(const string& path) {
    string fileName = path + "/" + name;

    ofstream outputFile(fileName);

    if (outputFile.is_open()) {
        outputFile << "Новый созданный файл" << endl;
        outputFile.close();
        cout << "Файл " << fileName << " успешно создан." << endl;
    }
    else
        cout << "Ошибка при создании файла " << fileName << endl;
}

void File::Remove(const string& fileName, const string& curDir) {
    if (is_regular_file(curDir + "/" + fileName))
        remove(curDir + "/" + fileName);
    else
        throw "Неверный путь";
}

void File::Rename(const string& oldName, const string& newName, const string& curDir) {
    if (!exists(curDir + "/" + oldName))
        throw runtime_error("Исходного файла/директории не существует: " + oldName);

    rename(curDir + "/" + oldName, curDir + "/" + newName);
}