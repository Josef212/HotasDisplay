#include "stdafx.h"
#include "HelperFuncs.h"
#include "DirectOutputWrapper.h"
#include "DataProviders/PainiteDataProvider.h"

// https://forums.frontier.co.uk/threads/how-to-use-x52-pro-sdk-making-us-of-the-mfd-and-leds.428813/

#define SETTINGS_FILE "settings.txt"

bool closeOnWindowX = false;

BOOL ControlHandler(DWORD fdwCtrlType);
bool CheckSettingsFile(std::wstring& profilePath);
void CreateSettingsFile();

int main()
{
	std::cout << "HOTAS Display" << std::endl;

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ControlHandler, TRUE);

	PainiteDataProvider dataProvider;
	DirectOutputWrapper output(&dataProvider);

	if (output.Init(L"EliteDangerousX52MFD") != S_OK)
	{
		std::cout << "Error while DirectOutput init." << std::endl;
		system("pause");
		return 1;
	}

	output.RegisterDevice();

	if (!output.GetDeviceType())
	{
		std::cout << "Press ENTER to close app" << std::endl;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return 0;
	}

	std::wstring profilePath;
	if (!CheckSettingsFile(profilePath))
	{
		std::cout << "Could not find or create Settings.txt with profile path." << std::endl;
		// TODO: Could try to open filepath popup
		return 0;
	}

	output.SetDeviceProfile(profilePath.c_str());
	output.RegisterSoftBtnCbk();
	output.RegisterPageCbk();

	output.SetPage(0, FLAG_SET_AS_ACTIVE);

	while (!closeOnWindowX)
	{
		output.Refresh();
		Sleep(10000);

		//static int counter = 0;
		//wchar_t buffer[16];
		//swprintf_s(buffer, 16, L"Counter: %d", counter++);
		//output.SetString(0, 0, buffer);
		//Sleep(100);
	}

	output.UnregisterSoftBtnCbk();
	output.UnregisterPageCbk();
	output.Deinit();

	return 0;
}

BOOL ControlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		closeOnWindowX = true;
		return TRUE;

	default:
		return FALSE;
	}
}

bool CheckSettingsFile(std::wstring& profilePath)
{
	std::cout << "Checking for settings file... " << std::endl;
	const char* fileName = SETTINGS_FILE;
	bool profileFound = false;

	auto lineRead = [&](std::string& line, uint32_t lineNumber) 
	{
		if (lineNumber == 0)
		{
			profilePath = std::wstring(line.begin(), line.end());
			profileFound = true;
		}
	};

	if (!ReadTxtFile(SETTINGS_FILE, lineRead))
	{
		CreateSettingsFile();
		profileFound = CheckSettingsFile(profilePath);
	}

	return profileFound;
}

void CreateSettingsFile()
{
	std::cout << "Creating txt file... ";

	TCHAR currentDir[MAX_PATH];
	TCHAR txtFile[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, currentDir);
	swprintf_s(txtFile, _T("%s\\%s"), currentDir, SETTINGS_FILE);
	CreateFile(txtFile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	std::cout << "DONE. \nWriting to txt file." << std::endl;

	std::wofstream file;
	file.open(SETTINGS_FILE);
	file << "C:\\Users\\Public\\Documents\\Logitech\\X52 Professional\\ED0.pr0\n";
	file.close();

	std::cout << "Wrote to txt file." << std::endl;
}
