/*
* Copyright © 2020 Berny23
*
* This file is part of "SEOS Cache Cleaner" which is released under the "MIT" license.
* See file "LICENSE" or go to "https://choosealicense.com/licenses/mit" for full license details.
*/

#include "GUI.h"

wxBEGIN_EVENT_TABLE(GUI, wxFrame)
	EVT_TREELIST_ITEM_CHECKED(10002, TreeListCtrl_OnItemChecked)
	EVT_BUTTON(10001, ButtonOk_OnPressed)
	EVT_MENU(10003, MenuItem1_OnPressed)
	EVT_MENU(10004, MenuItem2_OnPressed)
	EVT_MENU(10005, MenuItem3_OnPressed)
wxEND_EVENT_TABLE()

GUI::GUI() : wxFrame(nullptr, wxID_ANY, "SEOS Cache Cleaner")
{
	// Generated - Start

	this->SetSizeHints(wxSize(650, 400), wxDefaultSize);

	m_statusBar = this->CreateStatusBar(6, 0, wxID_ANY);
	m_menubar = new wxMenuBar(0);
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem(m_menu1, 10003, wxString(wxT("Refresh program list")) + wxT('\t') + wxT("CTRL+R"), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem1);

	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem(m_menu1, 10004, wxString(wxT("Exit")) + wxT('\t') + wxT("CTRL+Q"), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem2);

	m_menubar->Append(m_menu1, wxT("File"));

	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem(m_menu2, 10005, wxString(wxT("About...")) + wxT('\t') + wxT("CTRL+I"), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(m_menuItem3);

	m_menubar->Append(m_menu2, wxT("Help"));

	this->SetMenuBar(m_menubar);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	m_treeListCtrl = new wxTreeListCtrl(this, 10002, wxDefaultPosition, wxDefaultSize, wxTL_3STATE | wxTL_CHECKBOX | wxTL_DEFAULT_STYLE | wxTL_MULTIPLE);
	m_treeListCtrl->AppendColumn(wxT("Installed Programs"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_SORTABLE);
	m_treeListCtrl->AppendColumn(wxT("ID"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_HIDDEN);
	m_treeListCtrl->AppendColumn(wxT("Size"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, 0);

	bSizer1->Add(m_treeListCtrl, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	m_gauge = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	m_gauge->SetValue(0);
	m_gauge->Enable(false);

	bSizer4->Add(m_gauge, 1, wxEXPAND | wxRIGHT, 5);

	m_buttonOk = new wxButton(this, 10001, wxT("Clean selected directories"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(m_buttonOk, 0, wxEXPAND, 5);


	bSizer1->Add(bSizer4, 0, wxALL | wxEXPAND, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	// Generated - End

	int widths[] = { 0, 100, -1, 200, -1, 100 };
	m_statusBar->SetStatusWidths(6, widths);
	m_statusBar->SetStatusText("© 2020 - Berny23", 1);

	about = new DialogAbout(this);
	manager = new Manager();
	UpdateProgramList();
}

GUI::~GUI()
{
}

void GUI::UpdateProgramList()
{	
	m_treeListCtrl->DeleteAllItems();
	displaySize = 0;

	wxTreeListItem tempItem, tempChildItem;
	unsigned long long progSize, folderSize;

	for (auto& prog : manager->programList) {
		tempItem = m_treeListCtrl->AppendItem(m_treeListCtrl->GetRootItem(), prog.name);
		m_treeListCtrl->SetItemText(tempItem, 1, std::to_string(prog.id));

		progSize = 0;

		for (auto& dir : prog.directories) {
			tempChildItem = m_treeListCtrl->AppendItem(tempItem, dir.path);
			m_treeListCtrl->SetItemText(tempChildItem, 1, std::to_string(dir.id));
			folderSize = manager->GetFolderSize(dir.path);
			progSize += folderSize;
			m_treeListCtrl->SetItemText(tempChildItem, 2, GetStyledSize(folderSize));
			
			if (dir.isSafe) {
				m_treeListCtrl->CheckItem(tempChildItem);
				UpdateFolderSize(dir.path, true);
			}
		}

		m_treeListCtrl->SetItemText(tempItem, 2, GetStyledSize(progSize));

		m_treeListCtrl->UpdateItemParentStateRecursively(tempChildItem);
	}
}

void GUI::UpdateFolderSize(std::string path, bool add)
{
	if (add) displaySize += manager->GetFolderSize(path); // Add size
	else displaySize -= manager->GetFolderSize(path); // Subtract size

	m_statusBar->SetStatusText(GetStyledSize(), 5);
}

std::string GUI::GetStyledSize(unsigned long long size) {
	if (size == NULL) size = displaySize;

	displaySizeStream.str(std::string()); // Clear content

	if (size < 1000) displaySizeStream << size << " Bytes";
	else if (size < 1000000) displaySizeStream << std::fixed << std::setprecision(2) << size / 1000.0 << " KB";
	else if (size < 1000000000) displaySizeStream << std::fixed << std::setprecision(2) << size / 1000000.0 << " MB";
	else if (size < 1000000000000) displaySizeStream << std::fixed << std::setprecision(2) << size / 1000000000.0 << " GB";
	else displaySizeStream << std::fixed << std::setprecision(2) << size / 1000000000000.0 << " TB";

	return displaySizeStream.str();
}

void GUI::TreeListCtrl_OnItemChecked(wxTreeListEvent& event)
{
	m_statusBar->SetStatusText("", 3); // Clear status text

	m_treeListCtrl->UpdateItemParentStateRecursively(event.GetItem());
	wxTreeListItem tempItem;

	switch (event.GetOldCheckedState()) {
	case wxCHK_UNCHECKED: // If previously unchecked, check children
		if (m_treeListCtrl->GetItemParent(event.GetItem()) == m_treeListCtrl->GetRootItem()) { // If parent
			
			tempItem = m_treeListCtrl->GetFirstChild(event.GetItem());
			UpdateFolderSize(m_treeListCtrl->GetItemText(tempItem).ToStdString(), true);

			while (tempItem = m_treeListCtrl->GetNextSibling(tempItem)) { // Iterate over children
				UpdateFolderSize(m_treeListCtrl->GetItemText(tempItem).ToStdString(), true);
			}
		}
		else UpdateFolderSize(m_treeListCtrl->GetItemText(event.GetItem()).ToStdString(), true); // If child

		m_treeListCtrl->CheckItemRecursively(event.GetItem());
		break;
	case wxCHK_CHECKED: // If previously checked, uncheck children
		if (m_treeListCtrl->GetItemParent(event.GetItem()) == m_treeListCtrl->GetRootItem()) { // If parent
			
			tempItem = m_treeListCtrl->GetFirstChild(event.GetItem());
			UpdateFolderSize(m_treeListCtrl->GetItemText(tempItem).ToStdString(), false);

			while (tempItem = m_treeListCtrl->GetNextSibling(tempItem)) { // Iterate over children
				UpdateFolderSize(m_treeListCtrl->GetItemText(tempItem).ToStdString(), false);
			}
		}
		else UpdateFolderSize(m_treeListCtrl->GetItemText(event.GetItem()).ToStdString(), false); // If child

		m_treeListCtrl->CheckItemRecursively(event.GetItem(), wxCHK_UNCHECKED);
		break;
	case wxCHK_UNDETERMINED: // If previously undetermined, uncheck children
		if (m_treeListCtrl->GetItemParent(event.GetItem()) == m_treeListCtrl->GetRootItem()) { // If parent

			tempItem = m_treeListCtrl->GetFirstChild(event.GetItem());
			if (m_treeListCtrl->GetCheckedState(tempItem) == wxCHK_CHECKED) UpdateFolderSize(m_treeListCtrl->GetItemText(tempItem).ToStdString(), false);

			while (tempItem = m_treeListCtrl->GetNextSibling(tempItem)) { // Iterate over children
				if (m_treeListCtrl->GetCheckedState(tempItem) == wxCHK_CHECKED) UpdateFolderSize(m_treeListCtrl->GetItemText(tempItem).ToStdString(), false);
			}
		}
		else UpdateFolderSize(m_treeListCtrl->GetItemText(event.GetItem()).ToStdString(), false); // If child

		m_treeListCtrl->CheckItemRecursively(event.GetItem(), wxCHK_UNCHECKED);
		break;
	}
}

void GUI::ButtonOk_OnPressed(wxCommandEvent& event)
{
	wxTreeListItems items;
	wxTreeListItem tempItem = m_treeListCtrl->GetFirstItem();

	while (tempItem = m_treeListCtrl->GetNextItem(tempItem)) { // Iterate over all items
		if (m_treeListCtrl->GetItemParent(tempItem) != m_treeListCtrl->GetRootItem() && m_treeListCtrl->GetCheckedState(tempItem) == wxCHK_CHECKED) items.push_back(tempItem); // Add to list if child and checked
	}

	if (items.size() > 0) {
		m_buttonOk->Disable();
		m_treeListCtrl->Disable();
		m_gauge->Enable();
		m_statusBar->SetStatusText("Removing selected files...", 3);

		displaySize = 0;
		std::string path;
		int counter = 1;
		m_gauge->SetRange(items.size());

		for (auto& item : items) {
			m_gauge->SetValue(counter);
			counter++;

			path = m_treeListCtrl->GetItemText(item).ToStdString();
			displaySize += manager->GetFolderSize(path); // Needed to display total filesize later
			
			for (auto& p : std::filesystem::recursive_directory_iterator(path)) { // Iterate over all files and folders
				try {
					std::filesystem::remove(p); // Delete file
				}
				catch (std::filesystem::filesystem_error& e) {
					std::cout << e.what() << std::endl; // Simple log
				}
			}

			try {
				std::filesystem::remove(path); // Delete folder if possible
			}
			catch (std::filesystem::filesystem_error& e) {
				std::cout << e.what() << std::endl; // Simple log
			}
		}

		m_statusBar->SetStatusText("Cleaned " + GetStyledSize() + " in total!", 3);
		m_gauge->Disable();
		m_treeListCtrl->Enable();
		m_buttonOk->Enable();

		displaySize = 0;
		manager->PrepareProgramList();
		UpdateProgramList();
	}
}

void GUI::MenuItem1_OnPressed(wxCommandEvent& event)
{
	manager->PrepareProgramList();
	UpdateProgramList();

	m_gauge->SetValue(0);
	m_statusBar->SetStatusText("Successfully refreshed list.", 3);
}

void GUI::MenuItem2_OnPressed(wxCommandEvent& event)
{
	exit(EXIT_SUCCESS);
}

void GUI::MenuItem3_OnPressed(wxCommandEvent& event)
{
	about->ShowModal();
}
