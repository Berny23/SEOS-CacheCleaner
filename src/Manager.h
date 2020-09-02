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
#include <processenv.h>
#include <algorithm>
#include <string>

class Manager
{
public:
	Manager();
	~Manager();

	void PrepareProgramList();
	unsigned long long GetFolderSize(std::string path);
	
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

	struct PROGRAM {
		PROGRAM(unsigned int id, std::string name, std::vector<DIRECTORY> directories) {
			this->id = id;
			this->name = name;
			this->directories = directories;
		}

		unsigned int id;
		std::string name;
		std::vector<DIRECTORY> directories;
	};

	std::vector<PROGRAM> programList;

private:
	struct stat info;

	std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
};