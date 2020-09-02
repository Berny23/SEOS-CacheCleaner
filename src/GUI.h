/*
* Copyright © 2020 Berny23
*
* This file is part of "SEOS Cache Cleaner" which is released under the "MIT" license.
* See file "LICENSE" or go to "https://choosealicense.com/licenses/mit" for full license details.
*/

#pragma once

#include "Manager.h"
#include "DialogAbout.h"
#include <sstream>
#include <wx/wx.h>
#include <wx/treelist.h>

class GUI : public wxFrame
{
public:
	GUI();
	~GUI();

private:

	// Generated - Start

	wxStatusBar* m_statusBar;
	wxMenuBar* m_menubar;
	wxMenu* m_menu1;
	wxMenu* m_menu1_1;
	wxMenu* m_menu2;
	wxTreeListCtrl* m_treeListCtrl;
	wxGauge* m_gauge;
	wxButton* m_buttonOk;

	// Generated - End

	DialogAbout* about;
	Manager* manager;
	std::stringstream displaySizeStream;
	unsigned long long displaySize;

	void UpdateProgramList();
	void UpdateFolderSize(std::string path, bool add);
	std::string GetStyledSize(unsigned long long size = NULL);
	void TreeListCtrl_OnItemChecked(wxTreeListEvent& event);
	void ButtonOk_OnPressed(wxCommandEvent& event);
	void MenuItem1_OnPressed(wxCommandEvent& event);
	void MenuItem2_OnPressed(wxCommandEvent& event);
	void MenuItem3_OnPressed(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};