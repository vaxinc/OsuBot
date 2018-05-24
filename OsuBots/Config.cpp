#include "Config.h"

namespace Config
{
	// These go according to the order of the constants stored in file
	string OSUROOTPATH = "";
	char LEFT_KEY = 'z';
	char RIGHT_KEY = 'x';
	unsigned int CIRCLESLEEPTIME = 10;
	int CLICKOFFSET = 0;

	// Derived constants (not in file)
	string SONGFOLDER = "";
	string FILENAME = "config.txt";
}

// load constants from file into Config namespace
void Config::loadConfigFile(string filename)
{
	ifstream configTxtReader(filename);
	// if file exists, read from the file
	if (configTxtReader.is_open()) {
		string line;
		while (getline(configTxtReader, line))
		{
			auto constVector = Functions::split(line, '=');
			if (constVector.front() == "OSUROOTPATH") {
				Config::OSUROOTPATH = constVector.back();
			}
			else if (constVector.front() == "LEFT_KEY") {
				Config::LEFT_KEY = constVector.back().front();
			}
			else if (constVector.front() == "RIGHT_KEY") {
				Config::RIGHT_KEY = constVector.back().front();
			}
			else if (constVector.front() == "CIRCLESLEEPTIME") {
				Config::CIRCLESLEEPTIME = stoi(constVector.back());
			}
			else if (constVector.front() == "CLICKOFFSET") {
				Config::CLICKOFFSET = stoi(constVector.back());
			}
		}
		Config::SONGFOLDER = Config::OSUROOTPATH + "Songs\\";
		Config::FILENAME = filename;
		configTxtReader.close();
	}
	// else, ask for input and create file
	else {
		cout << "Initializing settings for first time use..." << endl << endl;
		string osuRootPath;
		char leftKey;
		char rightKey;

		cout << "Locate your osu!.exe or its shortcut: (if you accidentally pick the wrong file, please restart the program)" << endl;
		osuRootPath = ProcessTools::promptToChooseFileAndGetPath(L"Find osu!.exe or its shortcut");
		if (osuRootPath == "") {
			throw runtime_error("No file is chosen");
		}
		size_t found = osuRootPath.find_last_of("/\\");
		osuRootPath = osuRootPath.substr(0, found + 1);

		cout << "Enter the 'left click' key according to your settings in osu! (default is 'z')" << endl;
		cin >> leftKey;
		leftKey = static_cast<char>(tolower(leftKey));

		cin.clear();  //clear errors/bad flags on cin
		cin.ignore(cin.rdbuf()->in_avail(), '\n');//precise amount of ignoring

		cout << "Enter the 'right click' key according to your settings in osu! (default is 'x')" << endl;
		cin >> rightKey;
		rightKey = static_cast<char>(tolower(rightKey));

		cout << "Saving..." << endl;
		ofstream configTxtWriter(filename);
		if (configTxtWriter.is_open()) {
			configTxtWriter << "OSUROOTPATH=" << osuRootPath << endl;
			configTxtWriter << "LEFT_KEY=" << leftKey << endl;
			configTxtWriter << "RIGHT_KEY=" << rightKey << endl;
			configTxtWriter << "CIRCLESLEEPTIME=" << Config::CIRCLESLEEPTIME << endl;
			configTxtWriter << "CLICKOFFSET=" << Config::CLICKOFFSET << endl;
			cout << "If you wanna change the settings, delete the 'config.txt' created in the same directory with this program or manipulate the data inside manually." << endl;
			configTxtWriter.close();

			// call itself to read data this time
			loadConfigFile(filename);
			system("pause");
		}
		else {
			throw runtime_error("Failed to create config.txt. Probably file permission issue");
		}
	}
}

void Config::clearAndChangeConfig() {
	CHAR_INFO consoleBuffer[58 * 80];
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	ProcessTools::saveConsoleBuffer(csbi, consoleBuffer);
	system("cls");
	// clear the input buffer so that shift + c (C) is not shown when the setting page appears
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	// call function
	Config::changeConfig();
	ProcessTools::restoreConsoleBuffer(csbi, consoleBuffer);
}

void Config::changeConfig() {
	// var for checking if need to save the change
	bool discard = false;
	while (true) {
		string input;
		cout << "(Press y to save the changes, n to discard the changes)" << endl;
		cout << "Make change to: (current value)" << endl;
		cout << "1) LEFT_KEY (" << Config::LEFT_KEY << ")" << endl;
		cout << "2) RIGHT_KEY (" << Config::RIGHT_KEY << ")" << endl;
		cout << "3) CIRCLESLEEPTIME (" << Config::CIRCLESLEEPTIME << ")" << endl;
		cout << "4) CLICKOFFSET (" << Config::CLICKOFFSET << ")" << endl;
		cin >> input;
		// if y or n found, exit the loop
		if (input.front() == 'y' || input.front() == 'n') {
			discard = input.front() == 'n' ? true : false;
			break;
		}
		// var so that it can break out the outer loop
		bool loopBreak = false;
		while (!(all_of(input.begin(), input.end(), isdigit)) || stoi(input) < 1 || stoi(input) > 4) {
			cout << "Invalid input. Please enter again." << endl;
			cout << "Make change to: (current value)" << endl;
			cout << "1) LEFT_KEY (" << Config::LEFT_KEY << ")" << endl;
			cout << "2) RIGHT_KEY (" << Config::RIGHT_KEY << ")" << endl;
			cout << "3) CIRCLESLEEPTIME (" << Config::CIRCLESLEEPTIME << ")" << endl;
			cout << "4) CLICKOFFSET (" << Config::CLICKOFFSET << ")" << endl;
			cin >> input;
			if (input.front() == 'y' || input.front() == 'n') {
				discard = input.front() == 'n' ? true : false;
				loopBreak = true; // set true to exit outer loop later
				break;
			}
		}
		if (loopBreak) break; // exit outer loop
		int choice = stoi(input);
		system("cls");
		switch (choice) {
		case 1:
		case 2: {
			char change;
			if (choice == 1) {
				cout << "Enter new LEFT_KEY: " << endl;
			}
			else {
				cout << "Enter new RIGHT_KEY: " << endl;
			}
			cin >> change;
			change = static_cast<char>(tolower(change));

			cin.clear();  //clear errors/bad flags on cin
			cin.ignore(cin.rdbuf()->in_avail(), '\n');//precise amount of ignoring

			if (choice == 1) {
				Config::LEFT_KEY = change;
			}
			else {
				Config::RIGHT_KEY = change;
			}
			break;
		}
		case 3:
		case 4: {
			string changeStr;
			bool isNegative = false;
			if (choice == 3) {
				cout << "Enter new CIRCLESLEEPTIME: (recommended +-10 (for Auto only))" << endl;
			}
			else {
				cout << "Enter new CLICKOFFSET: (recommended -50 to 50)" << endl;
			}
			cin >> changeStr;
			// user might have input -ve value, which would fail the isdigit check, so check if it's -ve first
			if (changeStr.front() == '-') {
				isNegative = true;
				// erase the -ve sign for isdigit validation
				changeStr.erase(changeStr.begin());
			}
			if (!(all_of(changeStr.begin(), changeStr.end(), isdigit))) {
				cout << "Invalid value.";
				break;
			}
			else {
				int changeInt = stoi(changeStr);
				if (isNegative) {
					// now assign negative to the value if it is
					changeInt = -changeInt;
				}
				if (choice == 3) {
					Config::CIRCLESLEEPTIME = changeInt;
				}
				else {
					Config::CLICKOFFSET = changeInt;
				}
			}
			break;
		}
		}
		// clear the screen to show the choices only
		system("cls");
	}
	// program will come to here after 'y' or 'n' is input
	// if confirm save, rewrite the file
	if (!discard) {
		cout << "Saving..." << endl;
		ofstream configTxtWriter(Config::FILENAME);
		if (configTxtWriter.is_open()) {
			configTxtWriter << "OSUROOTPATH=" << Config::OSUROOTPATH << endl;
			configTxtWriter << "LEFT_KEY=" << Config::LEFT_KEY << endl;
			configTxtWriter << "RIGHT_KEY=" << Config::RIGHT_KEY << endl;
			configTxtWriter << "CIRCLESLEEPTIME=" << Config::CIRCLESLEEPTIME << endl;
			configTxtWriter << "CLICKOFFSET=" << Config::CLICKOFFSET << endl;
			configTxtWriter.close();
		}
		else {
			throw runtime_error("Failed to rewrite file. Please make sure the file is not opened by another program.");
		}
	}
	else {
		// reload the constants so that if the changes are discarded, the constants that have been modified are changed back
		Config::loadConfigFile(Config::FILENAME);
	}
}