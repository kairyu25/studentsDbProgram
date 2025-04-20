#pragma once

#include "mainframe.h"

class MyApp : public wxApp
{
public:
	bool OnInit() override;
	int OnExit() override;
	MainFrame* frame;
};

