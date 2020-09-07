/*
* Copyright © 2020 Berny23
*
* This file is part of "SEOS Cache Cleaner" which is released under the "MIT" license.
* See file "LICENSE" or go to "https://choosealicense.com/licenses/mit" for full license details.
*/

#include "DialogAbout.h"

DialogAbout::DialogAbout(wxWindow* parent) : wxDialog(parent, wxID_ANY, "SEOS Cache Cleaner")
{
	// Generated - Start

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_staticText = new wxStaticText(this, wxID_ANY, wxT("<big>SEOS Cache Cleaner</big>\n\n<b>DESCRIPTION</b>\nAllows you to clean temporary or cached files from selected programs.\nFiles which can't be deleted get skipped automatically.\n\n<b>LICENSE</b>\nMIT (https://choosealicense.com/licenses/mit/)\n\n<b>COPYRIGHT </b>\n© 2020 - Berny23"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxBORDER_NONE);
	m_staticText->SetLabelMarkup(wxT("<big>SEOS Cache Cleaner</big>\n\n<b>DESCRIPTION</b>\nAllows you to clean temporary or cached files from selected programs.\nFiles which can't be deleted get skipped automatically.\n\n<b>LICENSE</b>\nMIT (https://choosealicense.com/licenses/mit/)\n\n<b>COPYRIGHT </b>\n© 2020 - Berny23"));
	m_staticText->Wrap(-1);
	bSizer3->Add(m_staticText, 1, wxALIGN_CENTER | wxALL, 5);


	this->SetSizer(bSizer3);
	this->Layout();
	bSizer3->Fit(this);

	this->Centre(wxBOTH);

	// Generated - End
}

DialogAbout::~DialogAbout()
{
}
