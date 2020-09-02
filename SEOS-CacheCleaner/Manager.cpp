#include "Manager.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

Manager::Manager()
{
	PrepareProgramList();
}

Manager::~Manager()
{
}

void Manager::PrepareProgramList()
{
	std::ifstream in("ProgramDirList.json"); // Load json file
	json j;
	in >> j;

	programList.clear();

	unsigned int progCounter = 0; // Used for unique program IDs
	for (auto& prog : j["programs"]) { // Iterate over all programs
		std::vector<DIRECTORY> tempDirList; // Store directories of current program

		unsigned int dirCounter = 0; // Used for unique directory IDs
		for (auto& dir : prog["directories"]) { // Iterate over all directories of current program
			std::string path = dir["path"];

			char fullPath[MAX_PATH];

			auto x = ExpandEnvironmentStringsA(path.c_str(), fullPath, MAX_PATH);

			std::string fullPathString(fullPath);
			fullPathString = ReplaceAll(fullPathString, "\\", "/");
			//std::replace(fullPathString.begin(), fullPathString.end(), "\\", "/");

			if (!stat(fullPathString.c_str(), &info)) { // If directory exists
				tempDirList.push_back(DIRECTORY(dirCounter, dir["isSafe"], fullPathString)); // Add current directory to temporary list
				dirCounter++;
			}
		}

		if (tempDirList.size() > 0) {
			programList.push_back(PROGRAM(progCounter, prog["name"], tempDirList)); // Add current program to list
			progCounter++;
		}
	}
}

std::string Manager::ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

unsigned long long Manager::GetFolderSize(std::string path)
{
	unsigned long long size = 0;
	for (auto& file : fs::recursive_directory_iterator(path)) {
		size += fs::file_size(file);
	}
	return size;
}
