/*
* Copyright © 2020 Berny23
*
* This file is part of "SEOS Cache Cleaner" which is released under the "MIT" license.
* See file "LICENSE" or go to "https://choosealicense.com/licenses/mit" for full license details.
*/

#pragma once

#include <fstream>
#include <filesystem>
#include <vector>
#include <json.hpp>
#include <windows.h>
#include <string>
#include <codecvt>

class Manager
{
public:
	Manager();
	~Manager();

	std::string GetExePath();
	void PrepareGroupList();
	unsigned long long ProcessFilesRecursively(const std::string& folderPath, bool stopAtFirstFile = false, bool deleteFiles = false);

	struct DIRECTORY {
		DIRECTORY(unsigned int id, bool isSafe, std::string path) {
			this->id = id;
			this->isSafe = isSafe;
			this->path = path;
		}

		unsigned int id;
		bool isSafe;
		std::string path;
	};

	struct GROUP {
		GROUP(unsigned int id, std::string name, std::vector<DIRECTORY> directories) {
			this->id = id;
			this->name = name;
			this->directories = directories;
		}

		unsigned int id;
		std::string name;
		std::vector<DIRECTORY> directories;
	};

	std::vector<GROUP> groupList;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

private:
	struct stat info;

	std::vector<std::string> GetWildcardMatchingPaths(std::string fullPath);
	bool wildcardMatch(char str[], const char pattern[]);
};