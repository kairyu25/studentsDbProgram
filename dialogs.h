#pragma once

#include <wx/wx.h>
#include <array>
#include "student.h"

class ConnectToDbDialog : public wxDialog
{
public:
	ConnectToDbDialog(wxWindow* parent, wxWindowID id, const wxString& title, const std::array<wxString, 5>& autoConnect = std::array<wxString, 5>());
	std::array<wxString, 5> GetConnectionData();

private:
	wxTextCtrl* textCtrlDatabase;
	wxTextCtrl* textCtrlHost;
	wxTextCtrl* textCtrlPort;
	wxTextCtrl* textCtrlUser;
	wxTextCtrl* textCtrlPass;
};

class AddStudentDialog : public wxDialog
{
public:
	AddStudentDialog(wxWindow* parent, wxWindowID id, const wxString& title, 
		const std::vector<wxString>& groups);
	Student GetStudentData();

private:
	wxComboBox* comboBoxStudentGroup;
	wxTextCtrl* textCtrlSurname;
	wxTextCtrl* textCtrlName;
	wxTextCtrl* textCtrlPatronymic;
	wxComboBox* comboBoxSex;
};

class RemoveStudentDialog : public wxDialog
{
public:
	RemoveStudentDialog(wxWindow* parent, wxWindowID id, const wxString& title,
		const std::vector<Student>& studentsList);
	unsigned int GetStudentId();
	int ShowModal() override;

private:
	const std::vector<Student> studentsList;
	wxComboBox* comboBox;
};

class EditStudentDialog : public wxDialog
{
public:
	EditStudentDialog(wxWindow* parent, wxWindowID id, const wxString& title,
		const std::vector<Student>& studentsList);
	std::pair<unsigned int, Student> GetEditData();
	int ShowModal() override;

private:
	const std::vector<Student> studentsList;
	wxComboBox* comboBoxStudent;
	wxTextCtrl* textCtrlSurname;
	wxTextCtrl* textCtrlName;
	wxTextCtrl* textCtrlPatronymic;
	wxComboBox* comboBoxSex;
};

class MoveStudentDialog : public wxDialog
{
public:
	MoveStudentDialog(wxWindow* parent, wxWindowID id, const wxString& title,
		const std::vector<Student>& studentsList,
		const std::vector<wxString>& studentGroupsList);
	std::pair<unsigned int, wxString> GetEditData();
	int ShowModal() override;

private:
	const std::vector<Student> studentsList;
	const std::vector<wxString> studentGroupsList;
	wxComboBox* comboBoxStudent;
	wxComboBox* comboBoxStudentGroup;
};

class AddGroupDialog : public wxDialog
{
public:
	AddGroupDialog(wxWindow* parent, wxWindowID id, const wxString& title);
	wxString GetGroupName();

private:
	wxTextCtrl* textCtrlGroupName;
};

class RemoveGroupDialog : public wxDialog
{
public:
	RemoveGroupDialog(wxWindow* parent, wxWindowID id, const wxString& title,
		const std::vector<wxString>& studentGroupsList);
	wxString GetGroupName();
	int ShowModal() override;

private:
	const std::vector<wxString> studentGroupsList;
	wxComboBox* comboBoxGroupName;
};

class RenameGroupDialog : public wxDialog
{
public:
	RenameGroupDialog(wxWindow* parent, wxWindowID id, const wxString& title,
		const std::vector<wxString>& studentGroupsList);
	std::pair<wxString, wxString> GetEditData();
	int ShowModal() override;

private:
	const std::vector<wxString> studentGroupsList;
	wxComboBox* comboBoxStudentGroup;
	wxTextCtrl* textCtrlGroupName;
};