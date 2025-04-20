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
	: wxFrame(nullptr, wxID_ANY, "���������� ������� ���������")
{
	// Icon
	SetIcon(wxIcon("images/icon.ico", wxBITMAP_TYPE_ICO));

	// Menu
	wxMenu* menuDb = new wxMenu();
	menuDb->Append(ID_MENU_CONNECT_TO_DB, "������������", "������������ � ���� ������");
	menuDb->Append(ID_MENU_DISCONNECT, "�����������", "����������� �� ���� ������");
	menuDb->GetMenuItems()[1]->Enable(false);

	wxMenuBar* menu = new wxMenuBar;
	menu->Append(menuDb, "���� ������");
	SetMenuBar(menu);

	// Toolbar
	wxToolBar* toolBar = CreateToolBar();
	toolBar->SetWindowStyle(wxTB_TEXT);
	wxImage::AddHandler(new wxPNGHandler);

	toolBar->AddSeparator();

	toolBar->AddTool(ID_TOOLS_ADD, "��������", wxBitmap("images/add.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("�������� ������ �������� � ������");

	toolBar->AddTool(ID_TOOLS_REMOVE, "�������", wxBitmap("images/remove.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("������� �������� �� ������");

	toolBar->AddTool(ID_TOOLS_EDIT, "��������", wxBitmap("images/edit.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("�������� ���������� � ��������");

	toolBar->AddTool(ID_TOOLS_MOVE, "���������", wxBitmap("images/move.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("��������� �������� � ������ ������");

	toolBar->AddSeparator();

	toolBar->AddTool(ID_TOOLS_ADD_GROUP, "���. ������", wxBitmap("images/add_group.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("�������� ����� ������");

	toolBar->AddTool(ID_TOOLS_REMOVE_GROUP, "��. ������", wxBitmap("images/remove_group.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("������� ������");

	toolBar->AddTool(ID_TOOLS_RENAME_GROUP, "���. ������", wxBitmap("images/edit.png", wxBITMAP_TYPE_PNG));
	toolBar->GetToolByPos(toolBar->GetToolsCount() - 1)->SetLongHelp("������������� ������");

	toolBar->Realize();

	// Statusbar
	CreateStatusBar();

	// Students grid
	gridStudents = new wxGrid(this, ID_GRID_STUDENTS);
	gridStudents->EnableEditing(false);
	gridStudents->EnableDragRowSize(false);

	wxGridStringTable* tableStudents = new wxGridStringTable;
	gridStudents->AssignTable(tableStudents);
	gridStudents->SetColLabelValue(0, "�������");
	gridStudents->SetColLabelValue(1, "���");
	gridStudents->SetColLabelValue(2, "��������");
	gridStudents->SetColLabelValue(3, "���");
	gridStudents->SetColLabelValue(4, "������");

	gridStudents->AppendCols(5);

	// Student groups grid
	gridStudentGroups = new wxGrid(this, ID_GRID_GROUPS);
	gridStudentGroups->EnableEditing(false);
	gridStudentGroups->EnableDragColSize(false);
	gridStudentGroups->EnableDragRowSize(false);

	wxGridStringTable* tableStudentGroups = new wxGridStringTable;
	gridStudentGroups->AssignTable(tableStudentGroups);
	gridStudentGroups->SetColLabelValue(0, "������ �����");

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

	ConnectToDbDialog dialog(this, wxID_ANY, "������������ � ���� ������", autoConnect);
	if (dialog.ShowModal() != wxID_OK)
		return;

	std::array <wxString, 5> connectionData = dialog.GetConnectionData();

	if (connectionData[0] == wxEmptyString or connectionData[1] == wxEmptyString or connectionData[2] == wxEmptyString or connectionData[3] == wxEmptyString)
	{
		wxMessageBox("��� ��, ����, ���� � ��� ������������ �� ����� ���� �������!", "������");
		return;
	}

	long port;
	if (!connectionData[2].ToLong(&port))
	{
		wxMessageBox("���� ������ ���� ������!", "������");
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
		wxMessageBox(std::string("��� ����������� � ���� ������ ��������� ������:\n") + err.what(), "������");
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
	AddStudentDialog dialog(this, wxID_ANY, "�������� ��������", GetListOfStudentGroups());
	if (dialog.ShowModal() != wxID_OK)
		return;

	Student student = dialog.GetStudentData();

	if (student.surname.IsEmpty() or student.name.IsEmpty())
	{
		wxMessageBox("������� � ��� �� ����� ���� �������!", "������");
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
		wxMessageBox(wxString("��� ���������� �������� ��������� ������:\n") + err.what(), "������");
	}
}

void MainFrame::OnRemoveStudent(const wxCommandEvent& event)
{
	RemoveStudentDialog dialog(this, wxID_ANY, "������� ��������", GetListOfStudents());
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
		wxMessageBox(wxString("��� �������� �������� ��������� ������:\n") + err.what(), "������");
	}
}

void MainFrame::OnEditStudent(const wxCommandEvent& event)
{
	EditStudentDialog dialog(this, wxID_ANY, "�������� ���������� � ��������", GetListOfStudents());
	if (dialog.ShowModal() != wxID_OK)
		return;

	const unsigned int& id = dialog.GetEditData().first;
	const Student& student = dialog.GetEditData().second;

	if (student.surname.IsEmpty() or student.name.IsEmpty())
	{
		wxMessageBox("����� ������� � ��� �� ����� ���� �������!", "������");
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
		wxMessageBox(wxString("��� ��������� ���������� � �������� ��������� ������:\n") + err.what(), "������");
	}
}

void MainFrame::OnMoveStudent(const wxCommandEvent& event)
{
	MoveStudentDialog dialog(this, wxID_ANY, "��������� ��������", GetListOfStudents(), GetListOfStudentGroups());
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
		wxMessageBox(wxString("��� �������� �������� ��������� ������:\n") + err.what(), "������");
	}
}

void MainFrame::OnAddGroup(const wxCommandEvent& event)
{
	std::vector<wxString> groups = GetListOfStudentGroups();

	AddGroupDialog dialog(this, wxID_ANY, "�������� ������");
	if (dialog.ShowModal() != wxID_OK)
		return;

	if (std::find(groups.begin(), groups.end(), dialog.GetGroupName()) != groups.end())
	{
		wxMessageBox("����� ������ ��� ����������!", "������");
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
		wxMessageBox(wxString("��� ���������� ����� ������ ��������� ������:\n") + err.what(), "������");
	}
}

void MainFrame::OnRemoveGroup(const wxCommandEvent& event)
{
	RemoveGroupDialog dialog(this, wxID_ANY, "������� ������", GetListOfStudentGroups());
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
		wxMessageBox(wxString("��� �������� ������ ��������� ������:\n") + err.what(), "������");
	}
}

void MainFrame::OnRenameGroup(const wxCommandEvent& event)
{
	std::vector<wxString> groups = GetListOfStudentGroups();

	RenameGroupDialog dialog(this, wxID_ANY, "������������� ������", groups);
	if (dialog.ShowModal() != wxID_OK)
		return;

	if (std::find(groups.begin(), groups.end(), dialog.GetEditData().second) != groups.end())
	{
		wxMessageBox("����� ������ ��� ����������!", "������");
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
		wxMessageBox(wxString("��� �������������� ������ ��������� ������:\n") + err.what(), "������");
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
		gridStudents->SetCellValue(i, 3, students.at(i).isFemale ? "�" : "�");
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
				(std::basic_string<wchar_t>)rows.at(i).get(4) == "F" or (std::basic_string<wchar_t>)rows.at(i).get(4) == "�",
				rows.at(i).get(5).isNull() ? wxString("") : wxString((std::basic_string<wchar_t>)rows.at(i).get(5))
			);
	}
	catch (const mysqlx::Error& err)
	{
		wxMessageBox(wxString("��� ���������� ������ ��������� ��������� ������:\n") + err.what(), "������");
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
		wxMessageBox(wxString("��� ���������� ������ ����� ��������� ������:\n") + err.what(), "������");
	}

	return result;
}