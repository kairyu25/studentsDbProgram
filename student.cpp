#include "student.h"

Student::Student(unsigned int id, wxString surname, wxString name, wxString patronymic, bool isFemale, wxString group)
{
	this->id = id;
	this->surname = surname;
	this->name = name;
	this->patronymic = patronymic;
	this->isFemale = isFemale;
	this->group = group;
}

wxString Student::ToString() const
{
	return std::format("{} - {} {}{}{}",
		std::to_string(id),
		surname.ToStdString(),
		name.ToStdString(),
		patronymic.IsEmpty() ? "" : " " + patronymic.ToStdString(),
		group.IsEmpty() ? "" : " (" + group.ToStdString() + ")"
		);
}
