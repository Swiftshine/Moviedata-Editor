#pragma once
#include "window.h"
#include <string>

enum CommandType {
	CT_NONE,
	CT_SWITCH_SCENE,
	CT_MOVE_CAMERA,
	CT_PLAY_SOUND,
	CT_SPAWN_EFFECT,
	CT_END_SCENE,
	CT_SHAKE_SCREEN
};

int swapEndianness(int value) {

	return ((value >> 24) & 0x000000FF) |
		((value >> 8) & 0x0000FF00) |
		((value << 8) & 0x00FF0000) |
		((value << 24) & 0xFF000000);
}

void WindowFrame::GetAllParameterValues(int currentCommand) {
	int paramValue = 0;

	paramValue = commandArray[currentCommand].frame;
	paramValue = swapEndianness(paramValue);
	frame->SetValue(paramValue);

	paramValue = commandArray[currentCommand].type;
	paramValue = swapEndianness(paramValue);
	commandType->Select(paramValue);

	paramValue = commandArray[currentCommand].param1;
	paramValue = swapEndianness(paramValue);
	param1->SetValue(paramValue);

	paramValue = commandArray[currentCommand].param2;
	paramValue = swapEndianness(paramValue);
	param2->SetValue(paramValue);

	paramValue = commandArray[currentCommand].param3;
	paramValue = swapEndianness(paramValue);
	param3->SetValue(paramValue);

	paramValue = commandArray[currentCommand].param4;
	paramValue = swapEndianness(paramValue);
	param4->SetValue(paramValue);

	paramValue = commandArray[currentCommand].param5;
	paramValue = swapEndianness(paramValue);
	param5->SetValue(paramValue);

	paramValue = commandArray[currentCommand].param6;
	paramValue = swapEndianness(paramValue);
	param6->SetValue(paramValue);

	paramValue = commandArray[currentCommand].param7;
	paramValue = swapEndianness(paramValue);
	param7->SetValue(paramValue);
}

void WindowFrame::GetAllLabels(int currentCommand) {
	int type = commandArray[currentCommand].type;
	type = swapEndianness(type);

	switch (type) {
		case CT_NONE: {
			param1Desc->SetLabel("Not used.");
			param2Desc->SetLabel("Not used.");
			param3Desc->SetLabel("Not used.");
			param4Desc->SetLabel("Not used.");
			param5Desc->SetLabel("Not used.");
			param6Desc->SetLabel("Not used.");
			param7Desc->SetLabel("Not used.");
			break;
		}

		case CT_SWITCH_SCENE: {
			param1Desc->SetLabel("Scene ID:");
			param2Desc->SetLabel("Not used.");
			param3Desc->SetLabel("Not used.");
			param4Desc->SetLabel("Not used.");
			param5Desc->SetLabel("Not used.");
			param6Desc->SetLabel("Not used.");
			param7Desc->SetLabel("Not used.");
			break;
		}

		case CT_MOVE_CAMERA: {
			param1Desc->SetLabel("Starting X:");
			param2Desc->SetLabel("Starting Y:");
			param3Desc->SetLabel("Starting Z:");
			param4Desc->SetLabel("Ending x:");
			param5Desc->SetLabel("Ending Y:");
			param6Desc->SetLabel("Ending Z:");
			param7Desc->SetLabel("Duration (frames?):");
			break;
		}

		case CT_PLAY_SOUND: {
			param1Desc->SetLabel("Sound ID:");
			param2Desc->SetLabel("Not used.");
			param3Desc->SetLabel("Not used.");
			param4Desc->SetLabel("Not used.");
			param5Desc->SetLabel("Not used.");
			param6Desc->SetLabel("Not used.");
			param7Desc->SetLabel("Not used.");
			break;
		}

		case CT_SPAWN_EFFECT: {
			param1Desc->SetLabel("Effect ID:");
			param2Desc->SetLabel("Position X:");
			param3Desc->SetLabel("Position Y:");
			param4Desc->SetLabel("Position Z:");
			param5Desc->SetLabel("Not used.");
			param6Desc->SetLabel("Not used.");
			param7Desc->SetLabel("Not used.");
			break;
		}

		case CT_END_SCENE: {
			param1Desc->SetLabel("Not used.");
			param2Desc->SetLabel("Not used.");
			param3Desc->SetLabel("Not used.");
			param4Desc->SetLabel("Not used.");
			param5Desc->SetLabel("Not used.");
			param6Desc->SetLabel("Not used.");
			param7Desc->SetLabel("Not used.");
			break;
		}

		case CT_SHAKE_SCREEN: {
			param1Desc->SetLabel("Shake Type:");
			param2Desc->SetLabel("Not used.");
			param3Desc->SetLabel("Not used.");
			param4Desc->SetLabel("Not used.");
			param5Desc->SetLabel("Not used.");
			param6Desc->SetLabel("Not used.");
			param7Desc->SetLabel("Not used.");
			break;
		}

		default: {
			param1Desc->SetLabel("Invalid command type.");
			param2Desc->SetLabel("Invalid command type.");
			param3Desc->SetLabel("Invalid command type.");
			param4Desc->SetLabel("Invalid command type.");
			param5Desc->SetLabel("Invalid command type.");
			param6Desc->SetLabel("Invalid command type.");
			param7Desc->SetLabel("Invalid command type.");
			break;
		}
	}
}
// window
void WindowFrame::OnClose(wxCloseEvent& event) {
	delete commandArray;
	Destroy();
}

// file io
void WindowFrame::OpenFile(wxCommandEvent& event) {

	wxFileDialog openFileDialog(nullptr, "Open a moviedata/moviesound file", "", "", "moviedata/moviesound file (*.bin)|*.bin|All Files|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		wxLogStatus("Chose not to open file.");
		return; // user cancelled file dialog
	}

	wxString filename = openFileDialog.GetPath();
	std::string filenameStr = filename.ToStdString();

	file->open(filenameStr, std::ios::in | std::ios::binary);

	if (!file->is_open()) {
		wxLogStatus("Failed to open file.");
		return; // file failed to open
	}


	// get file size
	file->seekg(0, std::ios::end);
	std::streampos filesize = file->tellg();
	file->seekg(0, std::ios::beg);

	if (filesize != 18000) {
		file->close();
		wxLogStatus("Filesize mismatch.");
		return; // filesize has to match.
	}

	file->read(reinterpret_cast<char*>(commandArray), 18000);
	fileOpen = true;

	file->close();

	wxLogStatus("Opened file.");

	// clear command list
	for (int i = 0; i < 500; i++) {
		if (commandStrings->GetCount() == 0) {
			break;
		}
		commandStrings->RemoveAt(commandStrings->GetCount() - 1);
	}

	// add existing commands to command list
	int type = 0;
	int startFrame = 0;
	// command types
	for (int i = 0; i < 500; i++) {
		type = commandArray[i].type;
		// swap endianness so the type is read properly.
		type = swapEndianness(type);
		int a = 0;
		a = type;
		if (type != 0 && type != 5) {
			/*wxLogStatus("got here before crashing lmao");*/
			commandStrings->Add(wxString::Format(wxT("Command %d, Action: "), commandStrings->GetCount()) + commandTypes->Item(type));

		}

		if (type == 5) {
			// we should leave, the end of the command chain has been reached.
			commandStrings->Add(wxString::Format(wxT("Command %d, Action: "), commandStrings->GetCount()) + commandTypes->Item(type));
			break;
		}
	}



	// get the first target frame, first command type, and the first parameter set
	commandList->Set(*commandStrings);
	if (commandStrings->GetCount() != 0) {
		int targetFrame = commandArray[0].frame;
		targetFrame = swapEndianness(targetFrame);
		frame->SetValue(targetFrame);

		type = commandArray[0].type;
		type = swapEndianness(type);
		commandType->Select(type);

		commandList->Select(0);

		int paramValue = 0;
		switch (type) {
			case CT_NONE: {
				GetAllParameterValues(0);
				param1Desc->SetLabel("Not used.");
				param2Desc->SetLabel("Not used.");
				param3Desc->SetLabel("Not used.");
				param4Desc->SetLabel("Not used.");
				param5Desc->SetLabel("Not used.");
				param6Desc->SetLabel("Not used.");
				param7Desc->SetLabel("Not used.");
				break;
			}

			case CT_SWITCH_SCENE: {
				GetAllParameterValues(0);

				param1Desc->SetLabel("Scene ID:");
				param2Desc->SetLabel("Not used.");
				param3Desc->SetLabel("Not used.");
				param4Desc->SetLabel("Not used.");
				param5Desc->SetLabel("Not used.");
				param6Desc->SetLabel("Not used.");
				param7Desc->SetLabel("Not used.");
				break;
			}

			case CT_MOVE_CAMERA: {
				GetAllParameterValues(0);

				param1Desc->SetLabel("Starting X:");
				param2Desc->SetLabel("Starting Y:");
				param3Desc->SetLabel("Starting Z:");
				param4Desc->SetLabel("Ending x:");
				param5Desc->SetLabel("Ending Y:");
				param6Desc->SetLabel("Ending Z:");
				param7Desc->SetLabel("Duration (frames?):");
				break;
			}

			case CT_PLAY_SOUND: {
				GetAllParameterValues(0);

				param1Desc->SetLabel("Sound ID:");
				param2Desc->SetLabel("Not used.");
				param3Desc->SetLabel("Not used.");
				param4Desc->SetLabel("Not used.");
				param5Desc->SetLabel("Not used.");
				param6Desc->SetLabel("Not used.");
				param7Desc->SetLabel("Not used.");
				break;
			}

			case CT_SPAWN_EFFECT: {
				GetAllParameterValues(0);

				param1Desc->SetLabel("Effect ID:");
				param2Desc->SetLabel("Position X:");
				param3Desc->SetLabel("Position Y:");
				param4Desc->SetLabel("Position Z:");
				param5Desc->SetLabel("Not used.");
				param6Desc->SetLabel("Not used.");
				param7Desc->SetLabel("Not used.");
				break;
			}

			case CT_END_SCENE: {
				GetAllParameterValues(0);
				param1Desc->SetLabel("Not used.");
				param2Desc->SetLabel("Not used.");
				param3Desc->SetLabel("Not used.");
				param4Desc->SetLabel("Not used.");
				param5Desc->SetLabel("Not used.");
				param6Desc->SetLabel("Not used.");
				param7Desc->SetLabel("Not used.");
				break;
			}

			case CT_SHAKE_SCREEN: {
				GetAllParameterValues(0);

				param1Desc->SetLabel("Shake Type:");
				param2Desc->SetLabel("Not used.");
				param3Desc->SetLabel("Not used.");
				param4Desc->SetLabel("Not used.");
				param5Desc->SetLabel("Not used.");
				param6Desc->SetLabel("Not used.");
				param7Desc->SetLabel("Not used.");
				break;
			}

			default: {
				param1Desc->SetLabel("Invalid command type.");
				param2Desc->SetLabel("Invalid command type.");
				param3Desc->SetLabel("Invalid command type.");
				param4Desc->SetLabel("Invalid command type.");
				param5Desc->SetLabel("Invalid command type.");
				param6Desc->SetLabel("Invalid command type.");
				param7Desc->SetLabel("Invalid command type.");
				break;
			}
		}
		
	}

	GetAllParameterValues(event.GetSelection());
	commandCounter->SetLabel(wxString::Format(wxT("Command count: %d"), commandStrings->GetCount()));
}

void WindowFrame::SaveFile(wxCommandEvent& event) {
	if (!fileOpen) {
		wxLogStatus("There isn't a file open!");
		return;
	}

	wxFileDialog saveFileDialog(nullptr, "Save a moviedata/moviesound file", "", "", "moviedata/moviesound file (*.bin)|*.bin|Any File|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		wxLogStatus("Chose not to save file.");
		return;
	}

	wxString filename = saveFileDialog.GetPath();
	std::string filenameStr = filename.ToStdString();

	file->open(filenameStr, std::ios::out | std::ios::binary);

	if (!file->is_open()) {
		wxLogStatus("Failed to save file");
		return;
	}

	file->write(reinterpret_cast<char*>(commandArray), 18000);
	file->close();
	fileSaved = true;
	wxLogStatus("Saved file.");
}


// etc.
void WindowFrame::AddCommand(wxCommandEvent& event) {
	if (!fileOpen) {
		return;
	}

	commandCount = commandStrings->GetCount();

	if (commandCount >= 500) {
		wxLogStatus("Can't add any more commands! The maximum is 500.");
		return;
	}
	int newCommandIndex = commandStrings->GetCount();
	commandArray[newCommandIndex] = CommandBase();

	commandStrings->Add(wxString::Format(wxT("Command %d, Action: "), newCommandIndex) + commandTypes->Item(0));
	commandList->Set(*commandStrings);
	commandList->Select(newCommandIndex);

	commandCounter->SetLabel(wxString::Format(wxT("Command count: %d"), commandStrings->GetCount()));
	GetAllParameterValues(newCommandIndex);
	GetAllLabels(newCommandIndex);
	wxLogStatus("Added a command.");
}


void WindowFrame::RemoveCommand(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetSelection() == -1) {
		return;
	}

	commandCount = commandStrings->GetCount();

	if (commandCount <= 1) {
		wxLogStatus("Can't remove any more commands!");
		return;
	}

	int currentCommand = commandList->GetSelection();
	int nextSelection = std::max(0, currentCommand - 1);

	for (int i = currentCommand; i < commandStrings->GetCount() - 1; i++) {
		commandArray[i] = commandArray[i + 1];
	}
	memset(&commandArray[commandStrings->GetCount() - 1], 0, sizeof(CommandBase));

	commandStrings->RemoveAt(currentCommand);

	for (int i = 0; i < commandStrings->GetCount(); i++) {
		int type = swapEndianness(commandArray[i].type);
		wxString newLabel = wxString::Format(wxT("Command %d, Action: "), i) + commandTypes->Item(type);
		commandStrings->Item(i) = newLabel;
	}
	commandList->Set(*commandStrings);
	commandList->Select(nextSelection);

	commandCounter->SetLabel(wxString::Format(wxT("Command count: %d"), commandStrings->GetCount()));
	GetAllParameterValues(nextSelection);
	GetAllLabels(nextSelection);
	wxLogStatus("Removed a command.");
	wxYield();
}

void WindowFrame::InsertCommand(wxCommandEvent& event) {
	if (!fileOpen) {
		return;
	}

	int currentCommand = commandList->GetSelection();
	if (currentCommand == wxNOT_FOUND) {
		return;
	}

	int newCommandIndex = currentCommand;

	if (commandCount >= 500) {
		wxLogStatus("Can't insert any more commands! The maximum is 500.");
		return;
	}

	for (int i = commandCount; i > newCommandIndex; i--) {
		commandArray[i] = commandArray[i - 1];
	}

	commandArray[newCommandIndex] = CommandBase();

	commandStrings->Insert(wxString::Format(wxT("Command %d, Action: "), newCommandIndex) + commandTypes->Item(0), newCommandIndex);

	commandCount = commandStrings->GetCount();
	commandCounter->SetLabel(wxString::Format(wxT("Command count: %d"), commandCount));

	for (int i = newCommandIndex + 1; i < commandCount; i++) {
		int type = swapEndianness(commandArray[i].type);
		wxString newLabel = wxString::Format(wxT("Command %d, Action: "), i) + commandTypes->Item(type);
		commandStrings->Item(i) = newLabel;
	}
	commandList->Set(*commandStrings);
	commandList->Select(newCommandIndex);
	GetAllParameterValues(newCommandIndex);
	GetAllLabels(newCommandIndex);
}



void WindowFrame::GetCommandListSelection(wxCommandEvent& event) {
	if (event.GetSelection() == -1) {
		return;
	}
	int currentCommand = event.GetSelection();

	int targetFrame = commandArray[currentCommand].frame;

	// swap endianness so the frame is read properly.
	targetFrame = swapEndianness(targetFrame);

	frame->SetValue(targetFrame);

	int type = commandArray[currentCommand].type;

	type = swapEndianness(type);

	commandType->Select(type);

	if (type == 1) {
		param1->SetMin(0);
		param1->SetMax(5);
	}
	else {
		param1->SetMin(-2147483648);
		param1->SetMax(1977);
	}
	
	GetAllParameterValues(currentCommand);
	GetAllLabels(currentCommand);
}

void WindowFrame::SetFrame(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();

	commandArray[currentCommand].frame = swapEndianness(event.GetInt());
}

void WindowFrame::SetCommandType(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();

	commandArray[currentCommand].type = swapEndianness(event.GetSelection());
	commandStrings->Item(currentCommand) = wxString::Format(wxT("Command %d, Action: "), currentCommand) + commandTypes->Item(event.GetSelection());
	commandList->Set(*commandStrings);
	commandList->Select(currentCommand);
	GetAllLabels(currentCommand);
}

// params

void WindowFrame::SetParam1Value(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();
	commandArray[currentCommand].param1 = swapEndianness(event.GetInt());
}

void WindowFrame::SetParam2Value(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();
	commandArray[currentCommand].param2 = swapEndianness(event.GetInt());
}

void WindowFrame::SetParam3Value(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();
	commandArray[currentCommand].param3 = swapEndianness(event.GetInt());
}

void WindowFrame::SetParam4Value(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();
	commandArray[currentCommand].param4 = swapEndianness(event.GetInt());
}

void WindowFrame::SetParam5Value(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();
	commandArray[currentCommand].param5 = swapEndianness(event.GetInt());
}

void WindowFrame::SetParam6Value(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();
	commandArray[currentCommand].param6 = swapEndianness(event.GetInt());
}

void WindowFrame::SetParam7Value(wxCommandEvent& event) {
	if (!fileOpen || commandList->GetCount() == 0 || commandList->GetSelection() == -1) {
		return;
	}

	int currentCommand = commandList->GetSelection();
	commandArray[currentCommand].param7 = swapEndianness(event.GetInt());
}