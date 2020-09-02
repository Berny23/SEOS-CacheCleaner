/*
* Copyright © 2020 Berny23
*
* This file is part of "SEOS Cache Cleaner" which is released under the "MIT" license.
* See file "LICENSE" or go to "https://choosealicense.com/licenses/mit" for full license details.
*/

#pragma once

#include <wx/wx.h>

class DialogAbout : public wxDialog
{
public:
	DialogAbout(wxWindow* parent);
	~DialogAbout();

private:
	wxStaticText* m_staticText;
};

