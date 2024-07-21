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
                throw "Достигнут конец файла";
            input.clear();
            input.ignore(ignoreCountOnError, ignoreDelimiterOnError);
        }
        throw "Некорректный ввод";
    }

    void Rules()
    {
        cout << "1-Навигация по каталогам" << endl
            << "2-Отчистить консоль" << endl
            << "3-Создание новой директории" << endl
            << "4-Перейти по названию директории дальше" << endl
            << "5-Удалить директорию/файл" << endl
            << "6-Переименовать директорию/файл" << endl
            << "7-Копировать файл/директорию" << endl
            << "8-Узнать размер файла/директории" << endl
            << "9-Поиск по маске" << endl;
    }

    void RulesNavigator() {
        cout << endl << endl
            << "1-Перейти назад в каталог" << endl
            << "2-Перейти в самое начало каталога" << endl
            << "3-Назад в меню" << endl;
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
                    cout << "Текущий каталог: " << currentDirectory->GetName() << endl;
                }
                else
                    cout << "Вы находитесь в корневом каталоге." << endl;
            }
            else if (choice == 2) {
                delete currentDirectory;
                currentDirectory = new Directory("C:/Users/DELL");
                currentDirectory->Read("C:/Users/DELL");
                cout << "Текущий каталог: " << currentDirectory->GetName() << endl;
            }
            else if (choice == 3) {
                currentDirectory->List();
                break;
            }
            else {
                cout << "Неверный выбор. Повторите ввод." << endl;
                system("cls");
                currentDirectory->List();
            }
        }
    }

    void CreateDirectory()
    {
        cout << "Введите имя новой директории: ";
        string newDirectoryName;
        cin >> newDirectoryName;
        if (!currentDirectory->IsRepeatDirectory(currentDirectory->GetName(), newDirectoryName))
        {
            currentDirectory->objects.push_back(new Directory(newDirectoryName));
            currentDirectory->objects.back()->Create(currentDirectory->GetName());
            cout << "Директория " << newDirectoryName << " успешно создана." << endl;

            system("pause");
        }
        else
            cout << "Директория с таким именем уже существует." << endl;
    }
    void DeleteObject()
    {
        cout << "Введите имя директории/файла, который хотите удалить: ";
        string objectName;
        cin >> objectName;
        bool found = false;
        for (size_t i = 0; i < currentDirectory->objects.size(); i++) {
            if (currentDirectory->objects[i]->GetName() == objectName) {
                try {
                    currentDirectory->objects[i]->Remove(objectName, currentDirectory->GetName());
                    delete currentDirectory->objects[i];
                    currentDirectory->objects.erase(currentDirectory->objects.begin() + i);
                    cout << "Объект " << objectName << " успешно удален." << endl;
                    found = true;
                    break;
                }
                catch (const char* error) {
                    cout << error << endl;
                }
            }
        }
        if (!found) {
            cout << "Объект с таким именем не найден." << endl;
        }
    }

    void RenameObject() {
        cout << "Введите имя объекта, который хотите переименовать: ";
        string objectName;
        cin >> objectName;
        bool found = false;
        for (size_t i = 0; i < currentDirectory->objects.size(); i++) {
            if (currentDirectory->objects[i]->GetName() == objectName) {
                cout << "Введите новое имя: ";
                string newName;
                cin >> newName;
                try {
                    currentDirectory->objects[i]->Rename(objectName, newName, currentDirectory->GetName());
                    currentDirectory->objects[i]->SetName(newName);
                    cout << "Объект переименован." << endl;
                    system("pause");
                    found = true;
                    break;
                }
                catch (const exception& msg) {
                    cout << "Ошибка: " << msg.what() << endl;
                }
            }
        }
        if (!found) {
            system("pause");
            cout << "Объект с таким именем не найден." << endl;
        }
    }

    void CopyObject()
    {
        cout << "Введите имя объекта, который хотите скопировать: ";
        string objectName;
        cin >> objectName;
        bool found = false;
        for (size_t i = 0; i < currentDirectory->objects.size(); i++) {
            if (currentDirectory->objects[i]->GetName() == objectName) {
                cout << "Введите имя для копии: ";
                string copyName;
                cin >> copyName;
                if (currentDirectory->IsRepeatDirectory(currentDirectory->GetName(), copyName)) {
                    cout << "Объект с таким именем уже существует." << endl;
                    return;
                }
                currentDirectory->Copy(currentDirectory->GetName(), objectName, copyName);
                cout << "Объект скопирован." << endl;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Объект с таким именем не найден." << endl;
        }
        system("pause");
    }

    void MoveGetSize()
    {
        cout << "Выберете файл/директоия у которого хотите взять размер" << endl;
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
                        cout << "Размер файла/директории равен - " << temp << endl;
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
            cout << "Такой файла/директории не cуществует!" << endl;
        }
    }

    void SearchByMask()
    {
        cout << "Введите маску поиска: ";
        string mask;
        cin >> mask;
        //Реализовать логику поиска по маске.
        cout << "Найденные объекты: " << endl;
    }
    void ChangeDirectory()
    {
        string targetDirectory;
        cout << "Введите имя директории для перехода: ";
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
                        cout << "Вы перешли в директорию: " << currentDirectory->GetName() << endl;
                        system("pause");
                        break;
                    }
                    catch (const exception& msg) {
                        cout << "Ошибка: " << msg.what() << endl;
                    }
                }
            }
        }
        else
            cout << "Директория не найдена." << endl;
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
                cout << "Неверный выбор. Повторите ввод." << endl;
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