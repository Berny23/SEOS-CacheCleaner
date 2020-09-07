/*
* Copyright © 2020 Berny23
*
* This file is part of "SEOS Cache Cleaner" which is released under the "MIT" license.
* See file "LICENSE" or go to "https://choosealicense.com/licenses/mit" for full license details.
*/

#pragma once

#include "GUI.h"
#include "wx/wx.h"
#include "md5.h"

class App : public wxApp
{
public:
	App();
	~App();
	virtual bool OnInit();

private:
	std::stringstream buffer;
	std::ifstream fstream;

	GUI* frame = nullptr;
};