#include "mainframe.h"

#define MIN_FRAME_WIDTH 800
#define MIN_FRAME_HEIGHT 400

enum
{
	ID_GRID_STUDENTS = 2,
	ID_GRID_GROUPS,
	ID_MENU_CONNECT_TO_DB,
	ID_MENU_DISCONNECT,
	ID_TOOLS_ADD,
	ID_TOOLS_REMOVE,
	ID_TOOLS_MOVE,
	ID_TOOLS_EDIT,
	ID_TOOLS_ADD_GROUP,
	ID_TOOLS_REMOVE_GROUP,
	ID_TOOLS_RENAME_GROUP,
};

MainFrame::MainFrame()
	: wxFrame(nullptr, wxID_ANY, "Управление списком студентов")
{
	// Icon
	SetIcon(wxIcon("images/icon.ico", wxBITMAP_TYPE_ICO));

	// Menu
	wxMenu* menuDb = new wxMenu();
	menuDb->Append(ID_MENU_CONNECT_TO_DB, "Подключиться", "Подключиться к базе данных");
	menuDb->Append(ID_MENU_DISCONNECT, "Отключиться", "Отключиться от базы данных");
	menuDb->GetMenuItems()[1]->Enable(false);

	wxMenuBar* menu = new wxMenuBar;
	menu->Append(menuDb, "База данных");
	SetMenuBar(menu);

	// Toolbar
	wxToolBar* toolBar = CreateToolBar();
	toolBar->SetWindowStyle(wxTB_TEXT);
	wxImage::AddHandler(new wxPNGHandler);

	toolBar->AddSeparator();

	toolBar->AddTool(ID_TOOLS_ADD, "Добавить", wxBitmap("images/add.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("Добавить нового студента в список");

	toolBar->AddTool(ID_TOOLS_REMOVE, "Удалить", wxBitmap("images/remove.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("Удалить студента из списка");

	toolBar->AddTool(ID_TOOLS_EDIT, "Изменить", wxBitmap("images/edit.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("Изменить информацию о студенте");

	toolBar->AddTool(ID_TOOLS_MOVE, "Перевести", wxBitmap("images/move.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("Перевести студента в другую группу");

	toolBar->AddSeparator();

	toolBar->AddTool(ID_TOOLS_ADD_GROUP, "Доб. группу", wxBitmap("images/add_group.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("Добавить новую группу");

	toolBar->AddTool(ID_TOOLS_REMOVE_GROUP, "Уд. группу", wxBitmap("images/remove_group.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("Удалить группу");

	toolBar->AddTool(ID_TOOLS_RENAME_GROUP, "Пер. группу", wxBitmap("images/edit.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("Переименовать группу");

	toolBar->Realize();

	// Statusbar
	CreateStatusBar();

	// Students grid
	gridStudents = new wxGrid(this, ID_GRID_STUDENTS);
	gridStudents->EnableEditing(false);
	gridStudents->EnableDragRowSize(false);

	wxGridStringTable* tableStudents = new wxGridStringTable;
	gridStudents->AssignTable(tableStudents);
	gridStudents->SetColLabelValue(0, "Фамилия");
	gridStudents->SetColLabelValue(1, "Имя");
	gridStudents->SetColLabelValue(2, "Отчество");
	gridStudents->SetColLabelValue(3, "Пол");
	gridStudents->SetColLabelValue(4, "Группа");

	gridStudents->AppendCols(5);

	// Student groups grid
	gridStudentGroups = new wxGrid(this, ID_GRID_GROUPS);
	gridStudentGroups->EnableEditing(false);
	gridStudentGroups->EnableDragColSize(false);
	gridStudentGroups->EnableDragRowSize(false);

	wxGridStringTable* tableStudentGroups = new wxGridStringTable;
	gridStudentGroups->AssignTable(tableStudentGroups);
	gridStudentGroups->SetColLabelValue(0, "Список групп");

	gridStudentGroups->AppendCols(1);
	gridStudentGroups->SetColSize(0, 150);

	// Sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(gridStudentGroups, 0, wxEXPAND);
	sizer->Add(gridStudents, 1, wxEXPAND);

	SetSizer(sizer);

	// Size of the frame
	SetInitialSize(wxSize(MIN_FRAME_WIDTH * 1.5, MIN_FRAME_HEIGHT * 1.5));
	SetMinSize(wxSize(MIN_FRAME_WIDTH, MIN_FRAME_HEIGHT));

	// Disable interface
	EnableInterface(false);

	// Events
	Bind(wxEVT_MENU, &MainFrame::OnConnectToDb, this, ID_MENU_CONNECT_TO_DB);
	Bind(wxEVT_MENU, &MainFrame::OnDisconnect, this, ID_MENU_DISCONNECT);
	Bind(wxEVT_TOOL, &MainFrame::OnAddStudent, this, ID_TOOLS_ADD);
	Bind(wxEVT_TOOL, &MainFrame::OnRemoveStudent, this, ID_TOOLS_REMOVE);
	Bind(wxEVT_TOOL, &MainFrame::OnEditStudent, this, ID_TOOLS_EDIT);
	Bind(wxEVT_TOOL, &MainFrame::OnMoveStudent, this, ID_TOOLS_MOVE);
	Bind(wxEVT_TOOL, &MainFrame::OnAddGroup, this, ID_TOOLS_ADD_GROUP);
	Bind(wxEVT_TOOL, &MainFrame::OnRemoveGroup, this, ID_TOOLS_REMOVE_GROUP);
	Bind(wxEVT_TOOL, &MainFrame::OnRenameGroup, this, ID_TOOLS_RENAME_GROUP);
	Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::OnConnectToDb(const wxCommandEvent& event)
{
	std::array <wxString, 5> autoConnect;

	std::ifstream file;
	file.open("autoconnect.txt");
	if (file.is_open())
		for (int i = 0; i < 5; i++)
		{
			std::string line;
			std::getline(file, line, '\n');
			autoConnect[i] = line;
		}
	file.close();

	ConnectToDbDialog dialog(this, wxID_ANY, "Подключиться к базе данных", autoConnect);
	if (dialog.ShowModal() != wxID_OK)
		return;

	std::array <wxString, 5> connectionData = dialog.GetConnectionData();

	if (connectionData[0] == wxEmptyString or connectionData[1] == wxEmptyString or connectionData[2] == wxEmptyString or connectionData[3] == wxEmptyString)
	{
		wxMessageBox("Имя БД, хост, порт и имя пользователя не могут быть пустыми!", "Ошибка");
		return;
	}

	long port;
	if (!connectionData[2].ToLong(&port))
	{
		wxMessageBox("Порт должен быть числом!", "Ошибка");
		return;
	}

	try
	{
		sqlSession = new mysqlx::Session(mysqlx::SessionSettings(
			mysqlx::string(connectionData[1].ToStdWstring()),
			port,
			mysqlx::string(connectionData[3].ToStdWstring()),
			mysqlx::string(connectionData[4].ToStdWstring()),
			mysqlx::string(connectionData[0].ToStdWstring())
		));

		EnableInterface();

		UpdateTables();

		GetMenuBar()->GetMenu(0)->GetMenuItems()[0]->Enable(false);
		GetMenuBar()->GetMenu(0)->GetMenuItems()[1]->Enable();
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(std::string("При подключении к базе данных произошла ошибка:\n") + err.what(), "Ошибка");
		return;
	}
}

void MainFrame::OnDisconnect(const wxCommandEvent& event)
{
	EnableInterface(false);

	GetMenuBar()->GetMenu(0)->GetMenuItems()[0]->Enable();
	GetMenuBar()->GetMenu(0)->GetMenuItems()[1]->Enable(false);
}

void MainFrame::OnAddStudent(const wxCommandEvent& event)
{
	AddStudentDialog dialog(this, wxID_ANY, "Добавить студента", GetListOfStudentGroups());
	if (dialog.ShowModal() != wxID_OK)
		return;

	Student student = dialog.GetStudentData();

	if (student.surname.IsEmpty() or student.name.IsEmpty())
	{
		wxMessageBox("Фамилия и имя не могут быть пустыми!", "Ошибка");
		return;
	}

	try
	{
		sqlSession->sql(std::wstring(
			L"" + std::format("INSERT INTO students (st_surname,st_name,st_patronymic,st_sex,st_group) VALUES ('{}','{}',{},'{}',{});",
				student.surname.ToStdString(),
				student.name.ToStdString(),
				student.patronymic.IsEmpty() ? "NULL" : "'" + student.patronymic.ToStdString() + "'",
				student.isFemale ? "F" : "M",
				(student.group == "") ? "NULL" : "'" + student.group.ToStdString() + "'"
			))).execute();

		UpdateTables();
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("При добавлении студента произошла ошибка:\n") + err.what(), "Ошибка");
	}
}

void MainFrame::OnRemoveStudent(const wxCommandEvent& event)
{
	RemoveStudentDialog dialog(this, wxID_ANY, "Удалить студента", GetListOfStudents());
	if (dialog.ShowModal() != wxID_OK)
		return;

	try
	{
		sqlSession->sql(std::format("DELETE FROM students WHERE st_id={};",
			dialog.GetStudentId()
		)).execute();

		UpdateTables();
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("При удалении студента произошла ошибка:\n") + err.what(), "Ошибка");
	}
}

void MainFrame::OnEditStudent(const wxCommandEvent& event)
{
	EditStudentDialog dialog(this, wxID_ANY, "Изменить информацию о студенте", GetListOfStudents());
	if (dialog.ShowModal() != wxID_OK)
		return;

	const unsigned int& id = dialog.GetEditData().first;
	const Student& student = dialog.GetEditData().second;

	if (student.surname.IsEmpty() or student.name.IsEmpty())
	{
		wxMessageBox("Новые фамилия и имя не могут быть пустыми!", "Ошибка");
		return;
	}

	try
	{
		sqlSession->sql(std::wstring(
			L"" + std::format("UPDATE students SET st_surname = '{}', st_name = '{}', st_patronymic = {}, st_sex = '{}' WHERE st_id = {};",
				student.surname.ToStdString(),
				student.name.ToStdString(),
				student.patronymic.IsEmpty() ? "NULL" : "'" + student.patronymic.ToStdString() + "'",
				student.isFemale ? "F" : "M",
				id
			))).execute();

		UpdateTables();
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("При изменении информации о студенте произошла ошибка:\n") + err.what(), "Ошибка");
	}
}

void MainFrame::OnMoveStudent(const wxCommandEvent& event)
{
	MoveStudentDialog dialog(this, wxID_ANY, "Перевести студента", GetListOfStudents(), GetListOfStudentGroups());
	if (dialog.ShowModal() != wxID_OK)
		return;

	const unsigned int& id = dialog.GetEditData().first;
	const wxString& group = dialog.GetEditData().second;

	try
	{
		sqlSession->sql(std::wstring(L"" + std::format("UPDATE students SET st_group={} WHERE st_id={};",
			group.IsEmpty() ? "NULL" : "'" + group.ToStdString() + "'",
			id
		))).execute();

		UpdateTables();
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("При переводе студента произошла ошибка:\n") + err.what(), "Ошибка");
	}
}

void MainFrame::OnAddGroup(const wxCommandEvent& event)
{
	std::vector<wxString> groups = GetListOfStudentGroups();

	AddGroupDialog dialog(this, wxID_ANY, "Добавить группу");
	if (dialog.ShowModal() != wxID_OK)
		return;

	if (std::find(groups.begin(), groups.end(), dialog.GetGroupName()) != groups.end())
	{
		wxMessageBox("Такая группа уже существует!", "Ошибка");
		return;
	}

	try
	{
		sqlSession->sql(std::wstring(L"" + std::format("INSERT INTO `groups` VALUES ('{}');",
			dialog.GetGroupName().ToStdString()
		))).execute();

		UpdateTables();
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("При добавлении новой группы произошла ошибка:\n") + err.what(), "Ошибка");
	}
}

void MainFrame::OnRemoveGroup(const wxCommandEvent& event)
{
	RemoveGroupDialog dialog(this, wxID_ANY, "Удалить группу", GetListOfStudentGroups());
	if (dialog.ShowModal() != wxID_OK)
		return;

	try
	{
		sqlSession->sql(std::wstring(L"" + std::format("UPDATE students SET st_group=NULL WHERE st_group='{}';",
			dialog.GetGroupName().ToStdString()
		))).execute();

		sqlSession->sql(std::wstring(L"" + std::format("DELETE FROM `groups` WHERE gr_group = '{}';",
			dialog.GetGroupName().ToStdString()
		))).execute();

		UpdateTables();
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("При удалении группы произошла ошибка:\n") + err.what(), "Ошибка");
	}
}

void MainFrame::OnRenameGroup(const wxCommandEvent& event)
{
	std::vector<wxString> groups = GetListOfStudentGroups();

	RenameGroupDialog dialog(this, wxID_ANY, "Переименовать группу", groups);
	if (dialog.ShowModal() != wxID_OK)
		return;

	if (std::find(groups.begin(), groups.end(), dialog.GetEditData().second) != groups.end())
	{
		wxMessageBox("Такая группа уже существует!", "Ошибка");
		return;
	}

	const wxString& oldName = dialog.GetEditData().first;
	const wxString& newName = dialog.GetEditData().second;

	try
	{
		sqlSession->sql("SET FOREIGN_KEY_CHECKS=0;").execute();

		sqlSession->sql(std::wstring(L"" + std::format("UPDATE `groups` SET gr_group='{}' WHERE gr_group='{}';",
			newName.ToStdString(),
			oldName.ToStdString()
		))).execute();

		sqlSession->sql("SET FOREIGN_KEY_CHECKS=1;").execute();

		sqlSession->sql(std::wstring(L"" + std::format("UPDATE students SET st_group='{}' WHERE st_group='{}';",
			newName.ToStdString(),
			oldName.ToStdString()
		))).execute();

		UpdateTables();
	}
	catch (const mysqlx::Error& err)
	{
		sqlSession->sql("SET FOREIGN_KEY_CHECKS=1;").execute();
		wxMessageBox(wxString("При переименовании группы произошла ошибка:\n") + err.what(), "Ошибка");
	}
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	if (sqlSession != nullptr)
		delete sqlSession;
	Destroy();
}

void MainFrame::EnableInterface(bool enable)
{
	gridStudents->Show(enable);
	gridStudentGroups->Show(enable);

	GetToolBar()->EnableTool(ID_TOOLS_ADD, enable);
	GetToolBar()->EnableTool(ID_TOOLS_REMOVE, enable);
	GetToolBar()->EnableTool(ID_TOOLS_MOVE, enable);
	GetToolBar()->EnableTool(ID_TOOLS_EDIT, enable);
	GetToolBar()->EnableTool(ID_TOOLS_ADD_GROUP, enable);
	GetToolBar()->EnableTool(ID_TOOLS_REMOVE_GROUP, enable);
	GetToolBar()->EnableTool(ID_TOOLS_RENAME_GROUP, enable);
}

void MainFrame::UpdateTables()
{
	// Students table
	if (gridStudents->GetNumberRows() > 0)
		gridStudents->DeleteRows(0, gridStudents->GetNumberRows());

	std::vector<Student> students = GetListOfStudents();

	for (int i = 0; i < students.size(); i++)
	{
		gridStudents->AppendRows();
		gridStudents->SetRowLabelValue(i, wxString::Format(wxT("%i"), students.at(i).id));
		gridStudents->SetCellValue(i, 0, students.at(i).surname);
		gridStudents->SetCellValue(i, 1, students.at(i).name);
		gridStudents->SetCellValue(i, 2, students.at(i).patronymic);
		gridStudents->SetCellValue(i, 3, students.at(i).isFemale ? "Ж" : "М");
		gridStudents->SetCellValue(i, 4, students.at(i).group);
	}

	// Student groups table
	if (gridStudentGroups->GetNumberRows() > 0)
		gridStudentGroups->DeleteRows(0, gridStudentGroups->GetNumberRows());

	std::vector<wxString> groups = GetListOfStudentGroups();

	for (int i = 0; i < groups.size(); i++)
	{
		gridStudentGroups->AppendRows();
		gridStudentGroups->SetCellValue(i, 0, groups.at(i));
	}
}

std::vector<Student> MainFrame::GetListOfStudents()
{
	std::vector<Student> result;
	try
	{
		mysqlx::SqlResult sqlResult = sqlSession->sql("SELECT * FROM students").execute();
		std::vector<mysqlx::Row> rows = sqlResult.fetchAll();
		result.reserve(rows.size());
		for (int i = 0; i < rows.size(); i++)
			result.emplace_back(
				(unsigned int)rows.at(i).get(0),
				wxString((std::basic_string<wchar_t>)rows.at(i).get(1)),
				wxString((std::basic_string<wchar_t>)rows.at(i).get(2)),
				rows.at(i).get(3).isNull() ? wxString("") : wxString((std::basic_string<wchar_t>)rows.at(i).get(3)),
				(std::basic_string<wchar_t>)rows.at(i).get(4) == "F" or (std::basic_string<wchar_t>)rows.at(i).get(4) == "Ж",
				rows.at(i).get(5).isNull() ? wxString("") : wxString((std::basic_string<wchar_t>)rows.at(i).get(5))
			);
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("При обновлении списка студентов произошла ошибка:\n") + err.what(), "Ошибка");
	}

	return result;
}

std::vector<wxString> MainFrame::GetListOfStudentGroups()
{
	std::vector<wxString> result;
	try
	{
		mysqlx::SqlResult sqlResult = sqlSession->sql("SELECT * FROM `groups`").execute();
		std::vector<mysqlx::Row> rows = sqlResult.fetchAll();
		result.reserve(rows.size());
		for (int i = 0; i < rows.size(); i++)
			result.emplace_back((std::basic_string<wchar_t>)rows.at(i).get(0));
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("При обновлении списка групп произошла ошибка:\n") + err.what(), "Ошибка");
	}

	return result;
}