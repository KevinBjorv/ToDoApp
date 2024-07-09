#pragma once
#include <string>
#include <functional>
#include <map>
#include <sstream>
#include "Entry.h"

class ActionMenu;
class Entry;

class Dashboard {
public:
	explicit Dashboard() = default;
	virtual ~Dashboard() = default;

	void start(const bool clearScreen = true);

	std::unordered_map<Entry, int> entryNumLink; // Links the number that's displayed before the entry name on the dashboard
private: 
	struct MenuOptions { 
		std::vector<std::pair<std::string, char>> option; // Name & Prefix

		MenuOptions(){
			option.push_back({ "Add entry", 'a' });
			option.push_back({ "Edit entry", 'b' });
			option.push_back({ "Remove entry", 'c' });
			option.push_back({ "Settings", 'd' });
			option.push_back({ "Exit", 'e' });
		}
	};

	virtual void displayTasks();
	virtual void displayOptions();
	virtual void handleChoice(std::string& choice);

	virtual void removeEntry();

	std::unique_ptr<ActionMenu> actionMenu = std::make_unique<ActionMenu>();
	MenuOptions menuOption;
};

class ActionMenu {
public: 
	ActionMenu() = default;
	~ActionMenu() = default;

	virtual void addEntry();
	virtual void editEntry();

	virtual void displayHelp();

private: 
	enum EntryValidationResult {
		success = 0,
		successAndPriorityIsTrue = 1,
		// Name
		tooShort = 2,
		tooLong = 3,

		// Date
		wrongFormat = 4,

		// Priority
		invalidInput = 5
	};

	struct EntryValidation {
		EntryValidationResult name(const std::string& name);
		EntryValidationResult date(const std::string& doDate);
		EntryValidationResult priority(std::string& priorityInput);
	};
};



