#pragma once

#include <wx/wx.h>
#include <mysqlx/xdevapi.h>
#include <wx/grid.h>
#include "dialogs.h"
#include "student.h"
#include <fstream>

class MainFrame : public wxFrame
{
public:
	MainFrame();

private:
	wxGrid* gridStudents;
	wxGrid* gridStudentGroups;
	mysqlx::Session* sqlSession;

	void OnConnectToDb(const wxCommandEvent& event);
	void OnDisconnect(const wxCommandEvent& event);
	void OnAddStudent(const wxCommandEvent& event);
	void OnRemoveStudent(const wxCommandEvent& event);
	void OnEditStudent(const wxCommandEvent& event);
	void OnMoveStudent(const wxCommandEvent& event);
	void OnAddGroup(const wxCommandEvent& event);
	void OnRemoveGroup(const wxCommandEvent& event);
	void OnRenameGroup(const wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void EnableInterface(bool enable = true);
	void UpdateTables();
	std::vector<Student> GetListOfStudents();
	std::vector<wxString> GetListOfStudentGroups();
};
