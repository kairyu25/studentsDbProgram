#include "dialogs.h"

// ConnectToDbDialog
ConnectToDbDialog::ConnectToDbDialog(wxWindow* parent, wxWindowID id, const wxString& title, const std::array<wxString, 5>& autoConnect)
	: wxDialog(parent, id, title)
{
	// UI elements
	wxStaticText* labelDatabase = new wxStaticText(this, wxID_ANY, "Имя БД");
	textCtrlDatabase = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	textCtrlDatabase->SetValue(autoConnect[0]);
	wxStaticText* labelHost = new wxStaticText(this, wxID_ANY, "Хост");
	textCtrlHost = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	textCtrlHost->SetValue(autoConnect[1]);
	wxStaticText* labelPort = new wxStaticText(this, wxID_ANY, "Порт");
	textCtrlPort = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	textCtrlPort->SetValue(autoConnect[2]);
	wxStaticText* labelUser = new wxStaticText(this, wxID_ANY, "Юзер");
	textCtrlUser = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	textCtrlUser->SetValue(autoConnect[3]);
	wxStaticText* labelPass = new wxStaticText(this, wxID_ANY, "Пароль");
	textCtrlPass = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	textCtrlPass->SetValue(autoConnect[4]);

	wxButton* ok = new wxButton(this, wxID_OK, "Подключиться");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Отмена");

	// Sizers
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer* topSizer = new wxFlexGridSizer(2, 4, 0);

	mainSizer->Add(topSizer);
	mainSizer->Add(bottomSizer);

	topSizer->Add(labelDatabase, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(textCtrlDatabase, 0, wxLEFT | wxTOP | wxRIGHT, 5);
	topSizer->Add(labelHost, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(textCtrlHost, 0, wxLEFT | wxTOP | wxRIGHT, 5);
	topSizer->Add(labelPort, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(textCtrlPort, 0, wxLEFT | wxTOP | wxRIGHT, 5);
	topSizer->Add(labelUser, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(textCtrlUser, 0, wxLEFT | wxTOP | wxRIGHT, 5);
	topSizer->Add(labelPass, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(textCtrlPass, 0, wxLEFT | wxTOP | wxRIGHT, 5);

	bottomSizer->Add(ok, 0, wxALL, 5);
	bottomSizer->Add(cancel, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);
}

std::array<wxString, 5> ConnectToDbDialog::GetConnectionData()
{
	return {
		textCtrlDatabase->GetValue(),
		textCtrlHost->GetValue(),
		textCtrlPort->GetValue(),
		textCtrlUser->GetValue(),
		textCtrlPass->GetValue(),
	};
}

// AddStudentDialog
AddStudentDialog::AddStudentDialog(wxWindow* parent, wxWindowID id, const wxString& title, 
	const std::vector<wxString>& groups)
	: wxDialog(parent, id, title)
{
	// UI elements
	wxStaticText* labelGroup = new wxStaticText(this, wxID_ANY, "Группа");
	comboBoxStudentGroup = new wxComboBox(this, wxID_ANY, "-", wxDefaultPosition, wxSize(200, 22));
	comboBoxStudentGroup->SetEditable(false);
	comboBoxStudentGroup->Append("-");
	for (int i = 0; i < groups.size(); i++)
		comboBoxStudentGroup->Append(groups.at(i));

	comboBoxStudentGroup->SetSelection(0);

	wxStaticText* labelSurname = new wxStaticText(this, wxID_ANY, "Фамилия");
	textCtrlSurname = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	wxStaticText* labelName = new wxStaticText(this, wxID_ANY, "Имя");
	textCtrlName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	wxStaticText* labelPatronymic = new wxStaticText(this, wxID_ANY, "Отчество");
	textCtrlPatronymic = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	wxStaticText* labelSex = new wxStaticText(this, wxID_ANY, "Пол");
	comboBoxSex = new wxComboBox(this, wxID_ANY, "Мужской", wxDefaultPosition, wxSize(200, 22));
	comboBoxSex->SetEditable(false);
	comboBoxSex->Append("Мужской");
	comboBoxSex->Append("Женский");

	wxButton* ok = new wxButton(this, wxID_OK, "Добавить");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Отмена");

	// Sizers
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer* topSizer = new wxFlexGridSizer(2, 4, 0);

	mainSizer->Add(topSizer);
	mainSizer->Add(bottomSizer);

	topSizer->Add(labelGroup, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(comboBoxStudentGroup, 0, wxTOP | wxRIGHT, 5);
	topSizer->Add(labelSurname, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(textCtrlSurname, 0, wxTOP | wxRIGHT, 5);
	topSizer->Add(labelName, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(textCtrlName, 0, wxTOP | wxRIGHT, 5);
	topSizer->Add(labelPatronymic, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(textCtrlPatronymic, 0, wxTOP | wxRIGHT, 5);
	topSizer->Add(labelSex, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(comboBoxSex, 0, wxTOP | wxRIGHT, 5);

	bottomSizer->Add(ok, 0, wxALL, 5);
	bottomSizer->Add(cancel, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);
}

Student AddStudentDialog::GetStudentData()
{
	return Student(
		0, 
		textCtrlSurname->GetValue(), 
		textCtrlName->GetValue(), 
		textCtrlPatronymic->GetValue(),
		comboBoxSex->GetSelection() == 1,
		comboBoxStudentGroup->GetSelection() == 0 ? wxString("") : comboBoxStudentGroup->GetValue()
	);
}

// RemoveStudentDialog
RemoveStudentDialog::RemoveStudentDialog(wxWindow* parent, wxWindowID id, const wxString& title,
	const std::vector<Student>& studentsList)
	: wxDialog(parent, id, title), studentsList(studentsList)
{
	// UI elements
	comboBox = new wxComboBox(this, wxID_ANY, wxEmptyString);
	comboBox->SetEditable(false);

	for (int i = 0; i < studentsList.size(); i++)
		comboBox->Append(studentsList.at(i).ToString());

	comboBox->SetSelection(0);

	wxButton* ok = new wxButton(this, wxID_OK, "Удалить");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Отмена");

	// Sizers
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

	mainSizer->Add(comboBox, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mainSizer->Add(bottomSizer);

	bottomSizer->Add(ok, 0, wxALL, 5);
	bottomSizer->Add(cancel, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);
}

unsigned int RemoveStudentDialog::GetStudentId()
{
	return studentsList.at(comboBox->GetSelection()).id;
}

int RemoveStudentDialog::ShowModal()
{
	if (studentsList.empty())
	{
		wxMessageBox("Список студентов пуст", "Ошибка");
		return 0;
	}
	return wxDialog::ShowModal();
}

// EditStudentDialog
EditStudentDialog::EditStudentDialog(wxWindow* parent, wxWindowID id, const wxString& title,
	const std::vector<Student>& studentsList)
	: wxDialog(parent, id, title), studentsList(studentsList)
{
	// UI elements
	wxStaticText* labelStudent = new wxStaticText(this, wxID_ANY, "Студент");

	comboBoxStudent = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	comboBoxStudent->SetEditable(false);

	for (int i = 0; i < studentsList.size(); i++)
		comboBoxStudent->Append(studentsList.at(i).ToString());

	comboBoxStudent->SetSelection(0);

	wxStaticText* labelSurname = new wxStaticText(this, wxID_ANY, "Фамилия");
	textCtrlSurname = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	wxStaticText* labelName = new wxStaticText(this, wxID_ANY, "Имя");
	textCtrlName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	wxStaticText* labelPatronymic = new wxStaticText(this, wxID_ANY, "Отчество");
	textCtrlPatronymic = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 22));
	wxStaticText* labelSex = new wxStaticText(this, wxID_ANY, "Пол");
	comboBoxSex = new wxComboBox(this, wxID_ANY, "Мужской", wxDefaultPosition, wxSize(200, 22));
	comboBoxSex->SetEditable(false);
	comboBoxSex->Append("Мужской");
	comboBoxSex->Append("Женский");

	wxButton* ok = new wxButton(this, wxID_OK, "Изменить");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Отмена");

	// Sizers
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer* topSizer = new wxFlexGridSizer(2, 4, 0);

	mainSizer->Add(topSizer);
	mainSizer->Add(bottomSizer);

	topSizer->Add(labelStudent, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(comboBoxStudent, 0, wxTOP | wxRIGHT, 5);
	topSizer->Add(labelSurname, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(textCtrlSurname, 0, wxTOP | wxRIGHT, 5);
	topSizer->Add(labelName, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(textCtrlName, 0, wxTOP | wxRIGHT, 5);
	topSizer->Add(labelPatronymic, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(textCtrlPatronymic, 0, wxTOP | wxRIGHT, 5);
	topSizer->Add(labelSex, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(comboBoxSex, 0, wxTOP | wxRIGHT, 5);

	bottomSizer->Add(ok, 0, wxALL, 5);
	bottomSizer->Add(cancel, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);
}

std::pair<unsigned int, Student> EditStudentDialog::GetEditData()
{
	return std::pair<unsigned int, Student>(
		studentsList.at(comboBoxStudent->GetSelection()).id,
		Student(0, textCtrlSurname->GetValue(), textCtrlName->GetValue(),
		textCtrlPatronymic->GetValue(), comboBoxSex->GetSelection() == 2, wxEmptyString
	));
}

int EditStudentDialog::ShowModal()
{
	if (studentsList.empty())
	{
		wxMessageBox("Список студентов пуст", "Ошибка");
		return 0;
	}
	return wxDialog::ShowModal();
}

MoveStudentDialog::MoveStudentDialog(wxWindow* parent, wxWindowID id, const wxString& title,
	const std::vector<Student>& studentsList,
	const std::vector<wxString>& studentGroupsList)
	: wxDialog(parent, id, title), studentsList(studentsList), studentGroupsList(studentGroupsList)
{
	// UI elements
	wxStaticText* labelStudent = new wxStaticText(this, wxID_ANY, "Студент");
	comboBoxStudent = new wxComboBox(this, wxID_ANY);
	comboBoxStudent->SetEditable(false);

	for (int i = 0; i < studentsList.size(); i++)
		comboBoxStudent->Append(studentsList.at(i).ToString());

	comboBoxStudent->SetSelection(0);

	wxStaticText* labelGroup = new wxStaticText(this, wxID_ANY, "Новая группа");
	comboBoxStudentGroup = new wxComboBox(this, wxID_ANY);
	comboBoxStudentGroup->SetEditable(false);
	comboBoxStudentGroup->Append("-");

	for (int i = 0; i < studentGroupsList.size(); i++)
		comboBoxStudentGroup->Append(studentGroupsList.at(i));

	comboBoxStudentGroup->SetSelection(0);

	wxButton* ok = new wxButton(this, wxID_OK, "Перевести");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Отмена");

	// Sizers
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer* topSizer = new wxFlexGridSizer(2, 2, 0);

	topSizer->Add(labelStudent, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(comboBoxStudent, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(labelGroup, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(comboBoxStudentGroup, 0, wxTOP | wxLEFT | wxRIGHT, 5);

	mainSizer->Add(topSizer);
	mainSizer->Add(bottomSizer);

	bottomSizer->Add(ok, 0, wxALL, 5);
	bottomSizer->Add(cancel, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);
}

std::pair<unsigned int, wxString> MoveStudentDialog::GetEditData()
{
	return std::pair<unsigned int, wxString>(
		studentsList.at(comboBoxStudent->GetSelection()).id,
		comboBoxStudentGroup->GetSelection() == 0 ? wxString("") : comboBoxStudentGroup->GetValue()
	);
}

int MoveStudentDialog::ShowModal()
{
	if (studentGroupsList.empty() and studentsList.empty())
	{
		wxMessageBox("Списки студентов и групп пусты", "Ошибка");
		return 0;
	}
	else if (studentGroupsList.empty())
	{
		wxMessageBox("Список групп пуст", "Ошибка");
		return 0;
	}
	else if (studentsList.empty())
	{
		wxMessageBox("Список студентов пуст", "Ошибка");
		return 0;
	}
	return wxDialog::ShowModal();
}

// AddGroupDialog
AddGroupDialog::AddGroupDialog(wxWindow* parent, wxWindowID id, const wxString& title)
	: wxDialog(parent, id, title)
{
	// UI elements
	textCtrlGroupName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition);

	wxButton* ok = new wxButton(this, wxID_OK, "Добавить");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Отмена");

	// Sizers
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

	mainSizer->Add(textCtrlGroupName, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	mainSizer->Add(bottomSizer);

	bottomSizer->Add(ok, 0, wxALL, 5);
	bottomSizer->Add(cancel, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);
}

wxString AddGroupDialog::GetGroupName()
{
	return textCtrlGroupName->GetValue();
}

// RemoveGroupDialog
RemoveGroupDialog::RemoveGroupDialog(wxWindow* parent, wxWindowID id, const wxString& title, 
	const std::vector<wxString>& studentGroupsList)
	: wxDialog(parent, id, title), studentGroupsList(studentGroupsList)
{
	// UI elements
	comboBoxGroupName = new wxComboBox(this, wxID_ANY);
	comboBoxGroupName->SetEditable(false);

	for (int i = 0; i < studentGroupsList.size(); i++)
		comboBoxGroupName->Append(studentGroupsList.at(i));

	comboBoxGroupName->SetSelection(0);

	wxButton* ok = new wxButton(this, wxID_OK, "Удалить");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Отмена");

	// Sizers
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

	mainSizer->Add(comboBoxGroupName, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mainSizer->Add(bottomSizer);

	bottomSizer->Add(ok, 0, wxALL, 5);
	bottomSizer->Add(cancel, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);
}

wxString RemoveGroupDialog::GetGroupName()
{
	return comboBoxGroupName->GetValue();
}

int RemoveGroupDialog::ShowModal()
{
	if (studentGroupsList.empty())
	{
		wxMessageBox("Список групп пуст", "Ошибка");
		return 0;
	}
	return wxDialog::ShowModal();
}

// RenameGroupDialog
RenameGroupDialog::RenameGroupDialog(wxWindow* parent, wxWindowID id, const wxString& title, 
	const std::vector<wxString>& studentGroupsList)
	: wxDialog(parent, id, title), studentGroupsList(studentGroupsList)
{
	// UI elements
	wxStaticText* labelStudent = new wxStaticText(this, wxID_ANY, "Группа");
	comboBoxStudentGroup = new wxComboBox(this, wxID_ANY);
	comboBoxStudentGroup->SetEditable(false);

	for (int i = 0; i < studentGroupsList.size(); i++)
		comboBoxStudentGroup->Append(studentGroupsList.at(i));

	comboBoxStudentGroup->SetSelection(0);

	wxStaticText* labelGroup = new wxStaticText(this, wxID_ANY, "Новое имя");
	textCtrlGroupName = new wxTextCtrl(this, wxID_ANY);

	wxButton* ok = new wxButton(this, wxID_OK, "Переименовать");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "Отмена");

	// Sizers
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer* topSizer = new wxFlexGridSizer(2, 2, 0);

	topSizer->Add(labelStudent, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(comboBoxStudentGroup, 0, wxTOP | wxLEFT | wxRIGHT, 5);
	topSizer->Add(labelGroup, 0, wxTOP | wxLEFT, 5);
	topSizer->Add(textCtrlGroupName, 0, wxTOP | wxLEFT | wxRIGHT, 5);

	mainSizer->Add(topSizer);
	mainSizer->Add(bottomSizer);

	bottomSizer->Add(ok, 0, wxALL, 5);
	bottomSizer->Add(cancel, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);
}

std::pair<wxString, wxString> RenameGroupDialog::GetEditData()
{
	return std::pair<wxString, wxString>(
		comboBoxStudentGroup->GetValue(),
		textCtrlGroupName->GetValue()
	);
}

int RenameGroupDialog::ShowModal()
{
	if (studentGroupsList.empty())
	{
		wxMessageBox("Список групп пуст", "Ошибка");
		return 0;
	}
	return wxDialog::ShowModal();
}