#pragma once

#include "Manager.h"
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
	wxMenu* m_menu2;
	wxTreeListCtrl* m_treeListCtrl;
	wxGauge* m_gauge;
	wxStaticText* m_staticText;
	wxButton* m_buttonOk;

	// Generated - End

	Manager* manager;
	std::stringstream displaySizeStream;
	unsigned long long displaySize;

	void UpdateProgramList();
	void UpdateFolderSize(std::string path, bool add);
	std::string GetStyledSize(unsigned long long size = NULL);
	void TreeListCtrl_OnItemChecked(wxTreeListEvent& event);
	void ButtonOk_OnPressed(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};