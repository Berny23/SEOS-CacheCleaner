#include "App.h"

wxIMPLEMENT_APP(App);

App::App()
{
}

App::~App()
{
}

bool App::OnInit()
{
	frame = new GUI();
	frame->Show();

	return true;
}
