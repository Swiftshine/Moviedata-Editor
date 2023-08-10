#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/listbox.h>
#include <fstream>

struct CommandBase {
	int frame;
	int type;
	int param1;
	int param2;
	int param3;
	int param4;
	int param5;
	int param6;
	int param7;
};

static_assert(sizeof(CommandBase) == 0x24, "Command size mismatch");


// let's hope this isn't a mess.
class WindowFrame : public wxFrame {
public:
	WindowFrame(const wxString& title);

	int commandCount;
	bool fileOpen;
	bool fileSaved;
	std::fstream* file;
	CommandBase* commandArray;

private:
	wxStaticText* commandCounter;
	wxListBox* commandList;
	wxArrayString* commandStrings;
	wxPanel* panel;

	wxButton* openFileButton;
	wxButton* saveFileButton;
	wxButton* addCommandButton;
	wxButton* removeCommandButton;
	wxButton* insertCommandButton;

	wxSpinCtrl* frame;
	wxChoice* commandType;
	wxArrayString* commandTypes;

	wxSpinCtrl* param1;
	wxSpinCtrl* param2;
	wxSpinCtrl* param3;
	wxSpinCtrl* param4;
	wxSpinCtrl* param5;
	wxSpinCtrl* param6;
	wxSpinCtrl* param7;

	wxStaticText* param1Desc;
	wxStaticText* param2Desc;
	wxStaticText* param3Desc;
	wxStaticText* param4Desc;
	wxStaticText* param5Desc;
	wxStaticText* param6Desc;
	wxStaticText* param7Desc;
private:
	void OnClose(wxCloseEvent& event);

	void OpenFile(wxCommandEvent& event);
	void SaveFile(wxCommandEvent& event);

	void GetCommandListSelection(wxCommandEvent& event);
	void AddCommand(wxCommandEvent& event);
	void RemoveCommand(wxCommandEvent& event);
	void InsertCommand(wxCommandEvent& event);

	void SetFrame(wxCommandEvent& event);
	void SetCommandType(wxCommandEvent& event);

	void GetAllParameterValues(int currentCommand);
	void GetAllLabels(int currentCommand);

	void SetParam1Value(wxCommandEvent& event);
	void SetParam2Value(wxCommandEvent& event);
	void SetParam3Value(wxCommandEvent& event);
	void SetParam4Value(wxCommandEvent& event);
	void SetParam5Value(wxCommandEvent& event);
	void SetParam6Value(wxCommandEvent& event);
	void SetParam7Value(wxCommandEvent& event);
};

enum TextIDS {
	Text_CommandCount = 2,
	Text_Info,
	Text_FrameInfo,
	Text_CommandType,
	Text_Parameters,
	Text_Param1Desc,
	Text_Param2Desc,
	Text_Param3Desc,
	Text_Param4Desc,
	Text_Param5Desc,
	Text_Param6Desc,
	Text_Param7Desc,
	
};

WindowFrame::WindowFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(768, 432), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	// variables
	
	commandCount = 0;
	fileOpen = false;
	fileSaved = false;

	file = new std::fstream;

	commandArray = new CommandBase[500];
	memset(commandArray, 0, sizeof(CommandBase) * 500);

	// window stuff
	panel = new wxPanel(this);

	// gui
	commandStrings = new wxArrayString;
	
	commandList = new wxListBox(panel, wxID_ANY, wxPoint(500, 10), wxSize(250, 400), *commandStrings, wxLB_SINGLE | wxLB_NEEDED_SB);
	commandList->Bind(wxEVT_LISTBOX, &WindowFrame::GetCommandListSelection, this);

	commandCounter = new wxStaticText(panel, Text_CommandCount, "Command Count will go here.", commandList->GetPosition() + wxPoint(0, 402));
	openFileButton = new wxButton(panel, wxID_ANY, "Open File", wxPoint(10, 10), wxSize(100, 50));
	openFileButton->Bind(wxEVT_BUTTON, &WindowFrame::OpenFile, this);

	saveFileButton = new wxButton(panel, wxID_ANY, "Save File", wxPoint(120, 10), wxSize(100, 50));
	saveFileButton->Bind(wxEVT_BUTTON, &WindowFrame::SaveFile, this);

	addCommandButton = new wxButton(panel, wxID_ANY, "Add at end", wxPoint(10, 70), wxSize(100, 35));
	addCommandButton->Bind(wxEVT_BUTTON, &WindowFrame::AddCommand, this);

	removeCommandButton = new wxButton(panel, wxID_ANY, "Remove selected", addCommandButton->GetPosition() + wxPoint(110, 0), wxSize(100, 35));
	removeCommandButton->Bind(wxEVT_BUTTON, &WindowFrame::RemoveCommand, this);

	insertCommandButton = new wxButton(panel, wxID_ANY, "Add at selected", removeCommandButton->GetPosition() + wxPoint(110, 0), wxSize(100, 35));
	insertCommandButton->Bind(wxEVT_BUTTON, &WindowFrame::InsertCommand, this);

	wxStaticText* commandInfoText = new wxStaticText(panel, Text_Info, "Command Info", wxPoint(10, 120));
	wxFont font = commandInfoText->GetFont();
	font.SetWeight(wxFONTWEIGHT_BOLD);
	commandInfoText->SetFont(font);

	wxStaticText* frameInfoText = new wxStaticText(panel, Text_FrameInfo, "This command will execute on frame:", commandInfoText->GetPosition() + wxPoint(0, 30));
	wxStaticText* commandTypeText = new wxStaticText(panel, Text_CommandType, "This command will: ", frameInfoText->GetPosition() + wxPoint(0, 25));
	
	// maximum values this high seem to disable the arrows. unfortunate.
	frame = new wxSpinCtrl(panel, wxID_ANY, "", (frameInfoText->GetPosition() + wxPoint(210, -2)), wxSize(100, 20), wxSP_ARROW_KEYS, 0, 2147483647, 0);
	frame->Bind(wxEVT_SPINCTRL, &WindowFrame::SetFrame, this);
	
	commandTypes = new wxArrayString;
	commandTypes->Add("None");
	commandTypes->Add("Switch Scene");
	commandTypes->Add("Move Camera");
	commandTypes->Add("Play Sound");
	commandTypes->Add("Spawn Effect");
	commandTypes->Add("End Cutscene");
	commandTypes->Add("Shake Screen");

	
	commandType = new wxChoice(panel, wxID_ANY, (frame->GetPosition() + wxPoint(0, 25)), wxSize(100, 25), *commandTypes);
	commandType->Bind(wxEVT_CHOICE, &WindowFrame::SetCommandType, this);

	wxStaticText* paramText = new wxStaticText(panel, Text_Parameters, "Parameters", wxPoint(10, 200));
	paramText->SetFont(font);

	param1Desc = new wxStaticText(panel, Text_Param1Desc, "Param 1", paramText->GetPosition() + wxPoint(0, 20));
	param1 = new wxSpinCtrl(panel, wxID_ANY, "", (param1Desc->GetPosition() + wxPoint(210, 0)), wxSize(100, 20), wxSP_ARROW_KEYS, 0, 1977, 0);
	param1->Bind(wxEVT_SPINCTRL, &WindowFrame::SetParam1Value, this);

	param2Desc = new wxStaticText(panel, Text_Param2Desc, "Param 2", param1Desc->GetPosition() + wxPoint(0, 25));
	param2 = new wxSpinCtrl(panel, wxID_ANY, "", (param2Desc->GetPosition() + wxPoint(210, 0)), wxSize(100, 20), wxSP_ARROW_KEYS, -2147483648, 2147483647, 0);
	param2->Bind(wxEVT_SPINCTRL, &WindowFrame::SetParam2Value, this);

	param3Desc = new wxStaticText(panel, Text_Param3Desc, "Param 3", param2Desc->GetPosition() + wxPoint(0, 25));				  
	param3 = new wxSpinCtrl(panel, wxID_ANY, "", (param3Desc->GetPosition() + wxPoint(210, 0)), wxSize(100, 20), wxSP_ARROW_KEYS, -2147483648, 2147483647, 0);
	param3->Bind(wxEVT_SPINCTRL, &WindowFrame::SetParam3Value, this);

	param4Desc = new wxStaticText(panel, Text_Param4Desc, "Param 4", param3Desc->GetPosition() + wxPoint(0, 25));				  
	param4 = new wxSpinCtrl(panel, wxID_ANY, "", (param4Desc->GetPosition() + wxPoint(210, 0)), wxSize(100, 20), wxSP_ARROW_KEYS, -2147483648, 2147483647, 0);
	param4->Bind(wxEVT_SPINCTRL, &WindowFrame::SetParam4Value, this);

	param5Desc = new wxStaticText(panel, Text_Param5Desc, "Param 5", param4Desc->GetPosition() + wxPoint(0, 25));				  
	param5 = new wxSpinCtrl(panel, wxID_ANY, "", (param5Desc->GetPosition() + wxPoint(210, 0)), wxSize(100, 20), wxSP_ARROW_KEYS, -2147483648, 2147483647, 0);
	param5->Bind(wxEVT_SPINCTRL, &WindowFrame::SetParam5Value, this);

	param6Desc = new wxStaticText(panel, Text_Param6Desc, "Param 6", param5Desc->GetPosition() + wxPoint(0, 25));				  
	param6 = new wxSpinCtrl(panel, wxID_ANY, "", (param6Desc->GetPosition() + wxPoint(210, 0)), wxSize(100, 20), wxSP_ARROW_KEYS, -2147483648, 2147483647, 0);
	param6->Bind(wxEVT_SPINCTRL, &WindowFrame::SetParam6Value, this);

	param7Desc = new wxStaticText(panel, Text_Param7Desc, "Param 7", param6Desc->GetPosition() + wxPoint(0, 25));				  
	param7 = new wxSpinCtrl(panel, wxID_ANY, "", (param7Desc->GetPosition() + wxPoint(210, 0)), wxSize(100, 20), wxSP_ARROW_KEYS, -2147483648, 2147483647, 0);
	param7->Bind(wxEVT_SPINCTRL, &WindowFrame::SetParam7Value, this);

	
	CreateStatusBar();
}
