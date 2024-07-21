#pragma once

#include "file_system_object.h"

class File : public FileSystemObject
{
	string name;

public:
	File(string name);

	string GetName() override;
	void SetName(const string& name) override;
	void Create(const string& path) override;
	void Remove(const string& fileName, const string& curDir) override;
	void Rename(const string& oldName, const string& newName, const string& curDir) override;
	uintmax_t GetSize(const string& oldName, const string& curDir) override;
	void Copy(const string& curDir, const string& copyName, const string& destName) override;
};