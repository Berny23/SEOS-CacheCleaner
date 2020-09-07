/*
* Copyright © 2020 Berny23
*
* This file is part of "SEOS Cache Cleaner" which is released under the "MIT" license.
* See file "LICENSE" or go to "https://choosealicense.com/licenses/mit" for full license details.
*/

#include "Manager.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

Manager::Manager()
{

}

Manager::~Manager()
{
}

std::string Manager::GetExePath() {
	wchar_t result[MAX_PATH];
	GetModuleFileNameW(NULL, result, MAX_PATH);
	std::wstring::size_type pos = std::wstring(result).find_last_of(L"\\/");
	return converter.to_bytes(result).substr(0, pos);
}

void Manager::PrepareGroupList()
{
	std::ifstream in(GetExePath() + "\\ProgramDirList.json"); // Load json file
	json j;
	in >> j;

	groupList.clear();

	unsigned int progCounter = 0; // Used for unique group IDs
	for (auto& prog : j["groups"]) { // Iterate over all groups
		std::vector<DIRECTORY> tempDirList; // Store directories of current group

		unsigned int dirCounter = 0; // Used for unique directory IDs
		for (auto& dir : prog["directories"]) { // Iterate over all directories of current group
			std::string path = dir["path"];
			wchar_t fullPath[MAX_PATH];

			ExpandEnvironmentStringsW(converter.from_bytes(path).c_str(), fullPath, MAX_PATH); // Abomination of mankind

			std::vector<std::string> fullPathStrings;
			fullPathStrings.push_back(converter.to_bytes(fullPath));


			if (fullPathStrings[0].find('*') != std::string::npos || fullPathStrings[0].find('?') != std::string::npos) {
				fullPathStrings = GetWildcardMatchingPaths(fullPathStrings[0]);
			}


			for (auto& path : fullPathStrings) {
				if (ProcessFilesRecursively(path, true) > 0) {
					tempDirList.push_back(DIRECTORY(dirCounter, dir["isSafe"], path)); // Add current directory to temporary list
					dirCounter++;
				}
			}
		}

		if (tempDirList.size() > 0) {
			groupList.push_back(GROUP(progCounter, prog["name"], tempDirList)); // Add current group to list
			progCounter++;
		}
	}
}

bool Manager::wildcardMatch(char str[], const char pattern[]) {
	const int n = strlen(str), m = strlen(pattern);

	if (m == 0) return (n == 0); // If pattern empty

	std::vector<std::vector<bool>> lookupTable(n + 1, std::vector<bool>(m + 1)); // Allocate memory for 2D bool array

	lookupTable[0][0] = true;

	for (int j = 1; j <= m; j++) {
		if (pattern[j - 1] == '*') lookupTable[0][j] = lookupTable[0][j - 1];
	}

	// Fill table (bootom-up):
	for (int i = 1; i <= n; i++) { // Iterate over chars in str
		for (int j = 1; j <= m; j++) { // Iterate over chars in pattern

			if (pattern[j - 1] == '*') { // Check for '*'
				lookupTable[i][j] = lookupTable[i][j - 1] || lookupTable[i - 1][j];
			}
			else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1]) { // Check for '?' or matching character
				lookupTable[i][j] = lookupTable[i - 1][j - 1];
			}
			else lookupTable[i][j] = false; // If characters don't match
		}
	}

	return lookupTable[n][m];
}

std::vector<std::string> Manager::GetWildcardMatchingPaths(std::string path) {
	std::string delimiter = "\\";
	std::string tempPart;
	std::vector<std::string> parts;
	std::vector<std::string> tempPaths;
	std::vector<std::string> allPaths;
	size_t position = 0;
	bool pushNewItem = true;
	bool lastLoop = false;

	while ((position = path.find(delimiter)) != std::string::npos || lastLoop) { // Build list of parts containing a wildcard
		tempPart = path.substr(0, position);

		if (parts.size() > 0) {
			if (pushNewItem) parts.push_back("\\" + tempPart);
			else parts[parts.size() - 1] += "\\" + tempPart;

			if (tempPart.find('*') != std::string::npos || tempPart.find('?') != std::string::npos) { // Part contains wildcard
				pushNewItem = true;
			}
			else {
				pushNewItem = false;
			}
		}
		else {
			parts.push_back(tempPart);
			pushNewItem = false;
		}

		path.erase(0, position + delimiter.length());

		if (lastLoop) lastLoop = false;
		else if (path.find(delimiter) == std::string::npos) lastLoop = true;
	}

	tempPart = "";
	int i = 0;
	while (i < parts.size()) { // Iterate over all path parts
		if (allPaths.size() == 0) allPaths.push_back(parts[i]);

		tempPaths.clear();
		i++;

		for (const auto& path : allPaths) {

			std::string pathWithoutWildcard = path.substr(0, path.find_last_of(delimiter) + delimiter.length());
			if (stat(pathWithoutWildcard.c_str(), &info) == 0) { // If path exists
				for (const auto& entry : fs::directory_iterator(pathWithoutWildcard)) {
					if (wildcardMatch(&*entry.path().string().begin(), &*path.begin())) {
						if (i < parts.size()) tempPaths.push_back(entry.path().string() + parts[i]);
						else tempPaths.push_back(entry.path().string());
					}
				}
			}
		}
		allPaths = tempPaths;
	}

	return allPaths;
}

unsigned long long Manager::ProcessFilesRecursively(const std::string& folderPath, bool stopAtFirstFile, bool deleteFiles) { // Returns size of found files
	unsigned long long size = 0;
	unsigned long long tempSize;

	std::wstring tempPath;
	LPCWSTR path;
	std::string fileName;
	LPCWSTR filePath;
	WIN32_FIND_DATA fileData;
	HANDLE hFind;
	std::vector<std::string> subDirs;

	tempPath = converter.from_bytes(folderPath) + L"\\*";
	path = tempPath.c_str();
	hFind = FindFirstFileW(path, &fileData);

	if (INVALID_HANDLE_VALUE != hFind) { // If no error
		do {
			fileName = converter.to_bytes(fileData.cFileName);
			tempPath = converter.from_bytes(folderPath) + L"\\" + converter.from_bytes(fileName);
			filePath = tempPath.c_str();

			if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) { // If subdirectory
				if ((fileName != ".") && (fileName != "..")) { // If valid directory
					subDirs.push_back(folderPath + "\\" + fileName);
				}
			}
			else { // If file
				tempSize = fs::file_size(filePath); // Temporarily save size before file gets deleted
				size += tempSize;

				if (deleteFiles) {
					if (DeleteFileW(filePath) == 0) { // Try to delete file
						size -= tempSize; // If failed, subtract filesize
					}
				}
				else if (stopAtFirstFile) {
					break;
				}
			}
		} while (FindNextFileW(hFind, &fileData) != 0); // While there is another file in directory
	}
	else {
		printf("FindFirstFile failed (%d)\n", GetLastError());
	}

	FindClose(hFind);

	for (auto& dir : subDirs) { // Iterate over subdirectories via recursion
		size += ProcessFilesRecursively(dir, stopAtFirstFile, deleteFiles);
	}

	return size;
}