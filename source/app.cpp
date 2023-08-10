#include <wx/wx.h>
#include "window.h"
#include "events.h"

class App : public wxApp {
public:
	bool OnInit();
};

wxIMPLEMENT_APP(App);

bool App::OnInit() {

	WindowFrame* windowFrame = new WindowFrame("Moviedata Editor");
	windowFrame->SetClientSize(768, 432);

	windowFrame->Show();
	return true;
}