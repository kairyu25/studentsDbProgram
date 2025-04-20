#include "myapp.h"

bool MyApp::OnInit()
{
	frame = new MainFrame();
	frame->Show(true);
	return true;
}

int MyApp::OnExit()
{
	return 0;
}