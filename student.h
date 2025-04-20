#pragma once

#include <wx/wx.h>
#include <format>

class Student
{
public:
	Student(unsigned int id, wxString surname, wxString name, wxString patronymic = wxEmptyString, bool isFemale = false, wxString group = "");
	unsigned int id;
	wxString surname, name, patronymic, group;
	bool isFemale;
	wxString ToString() const;
};

