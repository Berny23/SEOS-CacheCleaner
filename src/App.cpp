/*
* Copyright © 2020 Berny23
*
* This file is part of "SEOS Cache Cleaner" which is released under the "MIT" license.
* See file "LICENSE" or go to "https://choosealicense.com/licenses/mit" for full license details.
*/

#include "App.h"

wxIMPLEMENT_APP(App);

App::App()
{
	const char* originalFileName = "ProgramDirList.json";
	const char* tempFileName = "___ProgramDirList.json";

	LPCWSTR url = L"https://raw.githubusercontent.com/Berny23/SEOS-CacheCleaner/master/src/ProgramDirList.json";
	LPCWSTR originalFileNameL = L"ProgramDirList.json";
	LPCWSTR tempFileNameL = L"___ProgramDirList.json";

	URLDownloadToFileW(NULL, url, tempFileNameL, 0, NULL); // Download latest path list from GitHub

	// Read files and compare MD5 checksums:
	fstream = std::ifstream(tempFileName);
	buffer << fstream.rdbuf();
	std::string md5checksumA = md5(buffer.str());

	buffer.str(std::string()); // Clear buffer

	fstream = std::ifstream(originalFileName);
	buffer << fstream.rdbuf();
	std::string md5checksumB = md5(buffer.str());

	fstream.close();

	if (md5checksumA == md5checksumB) {
		std::filesystem::remove(tempFileName); // Remove temp file if nothing has changed
	}
	else { // Rename temp file to original file if something has changed
		std::filesystem::remove(originalFileName);
		auto result = rename(tempFileName, originalFileName);
	}
}

App::~App()
{
}

bool App::OnInit()
{
	frame = new GUI();
	frame->Show();

	frame->Init();

	return true;
}
