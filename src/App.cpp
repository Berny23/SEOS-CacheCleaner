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
	manager = new Manager();

	std::string exePath = manager->GetExePath();
	LPCWSTR fileName = L"\\ProgramDirList.json";
	std::wstring filePath = manager->converter.from_bytes(exePath) + fileName;
	LPCWSTR url = L"https://raw.githubusercontent.com/Berny23/SEOS-CacheCleaner/master/src/ProgramDirList.json";

	URLDownloadToFileW(NULL, url, filePath.c_str(), 0, NULL); // Download latest path list from GitHub
}

App::~App()
{
}

bool App::OnInit()
{
	try {
		frame = new GUI();
		frame->Show();

		frame->Init();
	}
	catch (std::exception& e) {
		MessageBoxW(NULL, manager->converter.from_bytes(e.what()).c_str(), _T("Exception"), MB_OK | MB_SYSTEMMODAL);
	}

	return true;
}
