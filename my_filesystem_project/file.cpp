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
        cout << "���� ����������!" << endl;
    }
    catch (const filesystem_error& msg) {
        cerr << "������ ����������� �����: " << msg.what() << endl;
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
        outputFile << "����� ��������� ����" << endl;
        outputFile.close();
        cout << "���� " << fileName << " ������� ������." << endl;
    }
    else
        cout << "������ ��� �������� ����� " << fileName << endl;
}

void File::Remove(const string& fileName, const string& curDir) {
    if (is_regular_file(curDir + "/" + fileName))
        remove(curDir + "/" + fileName);
    else
        throw "�������� ����";
}

void File::Rename(const string& oldName, const string& newName, const string& curDir) {
    if (!exists(curDir + "/" + oldName))
        throw runtime_error("��������� �����/���������� �� ����������: " + oldName);

    rename(curDir + "/" + oldName, curDir + "/" + newName);
}