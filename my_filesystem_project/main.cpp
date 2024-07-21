#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <Windows.h>
#include <fstream>

#include "file_system_object.h"
#include "file.h"
#include "directory.h"


using namespace std;
using namespace filesystem;

class FileManager {
public:
    Directory* currentDirectory;

public:
    FileManager() {
        currentDirectory = new Directory("C:/Users/DELL");
        currentDirectory->Read("C:/Users/DELL");
    }

    bool TryReadValue
    (int* result,
        istream& input,
        const streamsize ignoreCountOnError = 1,
        const int ignoreDelimiterOnError = EOF)
    {
        while (true)
        {
            input >> *result;
            if (!input.fail())
                return true;
            if (input.eof())
                throw "��������� ����� �����";
            input.clear();
            input.ignore(ignoreCountOnError, ignoreDelimiterOnError);
        }
        throw "������������ ����";
    }

    void Rules()
    {
        cout << "1-��������� �� ���������" << endl
            << "2-��������� �������" << endl
            << "3-�������� ����� ����������" << endl
            << "4-������� �� �������� ���������� ������" << endl
            << "5-������� ����������/����" << endl
            << "6-������������� ����������/����" << endl
            << "7-���������� ����/����������" << endl
            << "8-������ ������ �����/����������" << endl
            << "9-����� �� �����" << endl;
    }

    void RulesNavigator() {
        cout << endl << endl
            << "1-������� ����� � �������" << endl
            << "2-������� � ����� ������ ��������" << endl
            << "3-����� � ����" << endl;
    }

    void Navigator() {
        int choice = 0;
        while (true) {
            RulesNavigator();

            if (!TryReadValue(&choice, cin))
                continue;

            if (choice == 1) {
                if (currentDirectory->GetName() != "C://") {
                    string parentPath = currentDirectory->GetName();
                    size_t lastSlashPos = parentPath.find_last_of("/\\");
                    if (lastSlashPos != string::npos)
                        parentPath = parentPath.substr(0, lastSlashPos);
                    delete currentDirectory;
                    currentDirectory = new Directory(parentPath);
                    system("cls");
                    currentDirectory->Read(parentPath);
                    cout << "������� �������: " << currentDirectory->GetName() << endl;
                }
                else
                    cout << "�� ���������� � �������� ��������." << endl;
            }
            else if (choice == 2) {
                delete currentDirectory;
                currentDirectory = new Directory("C:/Users/DELL");
                currentDirectory->Read("C:/Users/DELL");
                cout << "������� �������: " << currentDirectory->GetName() << endl;
            }
            else if (choice == 3) {
                currentDirectory->List();
                break;
            }
            else {
                cout << "�������� �����. ��������� ����." << endl;
                system("cls");
                currentDirectory->List();
            }
        }
    }

    void CreateDirectory()
    {
        cout << "������� ��� ����� ����������: ";
        string newDirectoryName;
        cin >> newDirectoryName;
        if (!currentDirectory->IsRepeatDirectory(currentDirectory->GetName(), newDirectoryName))
        {
            currentDirectory->objects.push_back(new Directory(newDirectoryName));
            currentDirectory->objects.back()->Create(currentDirectory->GetName());
            cout << "���������� " << newDirectoryName << " ������� �������." << endl;

            system("pause");
        }
        else
            cout << "���������� � ����� ������ ��� ����������." << endl;
    }
    void DeleteObject()
    {
        cout << "������� ��� ����������/�����, ������� ������ �������: ";
        string objectName;
        cin >> objectName;
        bool found = false;
        for (size_t i = 0; i < currentDirectory->objects.size(); i++) {
            if (currentDirectory->objects[i]->GetName() == objectName) {
                try {
                    currentDirectory->objects[i]->Remove(objectName, currentDirectory->GetName());
                    delete currentDirectory->objects[i];
                    currentDirectory->objects.erase(currentDirectory->objects.begin() + i);
                    cout << "������ " << objectName << " ������� ������." << endl;
                    found = true;
                    break;
                }
                catch (const char* error) {
                    cout << error << endl;
                }
            }
        }
        if (!found) {
            cout << "������ � ����� ������ �� ������." << endl;
        }
    }

    void RenameObject() {
        cout << "������� ��� �������, ������� ������ �������������: ";
        string objectName;
        cin >> objectName;
        bool found = false;
        for (size_t i = 0; i < currentDirectory->objects.size(); i++) {
            if (currentDirectory->objects[i]->GetName() == objectName) {
                cout << "������� ����� ���: ";
                string newName;
                cin >> newName;
                try {
                    currentDirectory->objects[i]->Rename(objectName, newName, currentDirectory->GetName());
                    currentDirectory->objects[i]->SetName(newName);
                    cout << "������ ������������." << endl;
                    system("pause");
                    found = true;
                    break;
                }
                catch (const exception& msg) {
                    cout << "������: " << msg.what() << endl;
                }
            }
        }
        if (!found) {
            system("pause");
            cout << "������ � ����� ������ �� ������." << endl;
        }
    }

    void CopyObject()
    {
        cout << "������� ��� �������, ������� ������ �����������: ";
        string objectName;
        cin >> objectName;
        bool found = false;
        for (size_t i = 0; i < currentDirectory->objects.size(); i++) {
            if (currentDirectory->objects[i]->GetName() == objectName) {
                cout << "������� ��� ��� �����: ";
                string copyName;
                cin >> copyName;
                if (currentDirectory->IsRepeatDirectory(currentDirectory->GetName(), copyName)) {
                    cout << "������ � ����� ������ ��� ����������." << endl;
                    return;
                }
                currentDirectory->Copy(currentDirectory->GetName(), objectName, copyName);
                cout << "������ ����������." << endl;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "������ � ����� ������ �� ������." << endl;
        }
        system("pause");
    }

    void MoveGetSize()
    {
        cout << "�������� ����/��������� � �������� ������ ����� ������" << endl;
        string name;
        cin >> name;

        if (currentDirectory->IsRepeatDirectory(currentDirectory->GetName(), name))
        {
            for (auto i = 0; i < currentDirectory->objects.size(); i++)
                if (currentDirectory->objects[i]->GetName() == name)
                {
                    try
                    {
                        uintmax_t temp = currentDirectory->objects[i]->GetSize(name, currentDirectory->GetName());
                        cout << "������ �����/���������� ����� - " << temp << endl;
                        system("pause");
                        break;
                    }
                    catch (...)
                    {
                        cout << "Error: " << endl;
                    }
                    break;
                }
        }
        else
        {
            cout << "����� �����/���������� �� c���������!" << endl;
        }
    }

    void SearchByMask()
    {
        cout << "������� ����� ������: ";
        string mask;
        cin >> mask;
        //����������� ������ ������ �� �����.
        cout << "��������� �������: " << endl;
    }
    void ChangeDirectory()
    {
        string targetDirectory;
        cout << "������� ��� ���������� ��� ��������: ";
        cin >> targetDirectory;

        string newPath = currentDirectory->GetName() + "/" + targetDirectory;

        if (is_directory(newPath))
        {
            for (int i = 0; i < currentDirectory->objects.size(); i++) {
                if (currentDirectory->objects[i]->GetName() == targetDirectory)
                {
                    try
                    {
                        delete currentDirectory;
                        currentDirectory = new Directory(newPath);
                        currentDirectory->Read(newPath);
                        cout << "�� ������� � ����������: " << currentDirectory->GetName() << endl;
                        system("pause");
                        break;
                    }
                    catch (const exception& msg) {
                        cout << "������: " << msg.what() << endl;
                    }
                }
            }
        }
        else
            cout << "���������� �� �������." << endl;
    }

    void Run() {
        int choice = 0;
        while (true) {
            Rules();

            if (!TryReadValue(&choice, cin))
                continue;

            switch (choice) {
            case 1:
                Navigator();
                break;
            case 2:
            {
                system("cls");
                currentDirectory->List();
                break;
            }
            case 3:
                CreateDirectory();
                system("cls");
                currentDirectory->List();

                break;
            case 4:
                ChangeDirectory();
                system("cls");
                currentDirectory->List();
                break;
            case 5:
                currentDirectory->List();
                DeleteObject();
                break;
            case 6:
                RenameObject();
                system("cls");
                currentDirectory->List();
                break;
            case 7:
                CopyObject();
                system("cls");
                currentDirectory->List();
                break;
            case 8:
                MoveGetSize();
                system("cls");
                currentDirectory->List();
                break;
            case 9:
                currentDirectory->List();
                SearchByMask();
                break;
            default:
                cout << "�������� �����. ��������� ����." << endl;
                system("cls");
                currentDirectory->List();
            }
        }
    }

    ~FileManager() { delete currentDirectory; }
};

void main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FileManager fileManager;
    fileManager.Run();
}