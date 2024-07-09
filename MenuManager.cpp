#include "MenuManager.h"
#include "Database.h"
#include "UtilityFunctions.h"
#include "Entry.h"
#include <iostream>
#include <cctype>
#include <vector>

#pragma region Non-Virtual Implementations
void Dashboard::start(const bool clearScreen) {
	if (clearScreen) {
		utility::console::clearScreen();
	}
	utility::console::setTitle("Dashboard"); // TODO: Implement
	displayOptions();
}
#pragma endregion

#pragma region Dashboard
void Dashboard::displayOptions() {
	std::cout << "Welcome to the To-Do list main menu!\n";
	displayTasks();
	std::cout << "\nOptions\n";
	
	// Print the menu options and their prefix
	for (const auto& pair : menuOption.option) {
		std::cout << pair.second << ". " << pair.first << '\n';
	}

	std::string choice;
	std::cout << "Enter choice: ";
	std::getline(std::cin, choice);
	handleChoice(choice);
}

void Dashboard::handleChoice(std::string& choice) {
	utility::string::convertToLowercase(choice);

	for (const auto& pair : menuOption.option) {
		// Convert the key to lowercase for comparison
		std::string lowerKey = utility::string::toLowerCase(pair.first);

		// Compare the choice with the lowercase key and the prefix (pair.second)
		if (choice == lowerKey || choice == std::string(1, pair.second)) {
			if (pair.second == 'a') {
				actionMenu->addEntry();
				return;
			}
			else if (pair.second == 'b') {
				actionMenu->editEntry();
				return;
			}
			else if (pair.second == 'c') {
				removeEntry();
				return;
			}
			else if (pair.second == 'd') {
				// Open settings
				return;
			}
			else if (pair.second == 'e') {
				std::cout << "Exiting...";
				exit(1);
			}
		}
	}

	std::cout << "Invalid choice, please try again" << std::endl;
	// TODO: Clear screen
	utility::console::clearInputBuffer();
	std::cin.get();
	start();
}

void Dashboard::displayTasks() {
	entryNumLink.clear(); // Clear existing mappings
	std::vector<Entry> entries = Database::loadEntries(); // Load entries from the database

	for (size_t i = 1; i < entries.size(); ++i) { // Start from 1 to skip the empty element
		std::cout << i << ". " << entries[i].name << " (" << entries[i].doDate << ")\n";
		entryNumLink[entries[i]] = static_cast<int>(i); // Map the entry to its index
	}
}
#pragma endregion

#pragma region ActionMenu
void ActionMenu::addEntry() {
	Dashboard dashboard;
	utility::console::setTitle("Add Entry"); 

	std::string name;
	std::string doDate;
	std::string priorityInput; // Converts to bool

	std::cout << "Enter task name: ";
	std::getline(std::cin, name);

	std::cout << "Enter do-date (optional, MMDD): ";
	std::getline(std::cin, doDate);

	std::cout << "Do you want to make this task a priority? (y/n): ";
	std::getline(std::cin, priorityInput);
	EntryValidation validate;

	bool priority = false;
	bool failure = false;

	if (validate.name(name) == success) {
		if (validate.date(doDate) == success) {
			if (validate.priority(priorityInput) == success) {
				priority = false;
			}
			else if (validate.priority(priorityInput) == successAndPriorityIsTrue) {
				priority = true;
			} else {
				std::cout << "Do-date was not valid due to reason: " << validate.priority(priorityInput);
				failure = true;
			}
		} else {
			std::cout << "Do-date was not valid due to reason: " << validate.date(doDate);
			failure = true;
		}
	} else {
		std::cout << "Name was not valid due to reason: " << validate.name(name);
		failure = true;
	}
	std::cout << std::endl;

	if (!failure) {
		Entry newEntry(name, doDate, priority);
		EntryManagement entryManagement(newEntry);
		entryManagement.addEntry();
	}
	dashboard.start();
}

void Dashboard::removeEntry() {
	std::vector<Entry> entries = Database::loadEntries();
	utility::console::setTitle("Remove Entry");
	bool entryFound = false;
	Entry selectedEntry;

	std::string choice;
	std::cout << "Enter the name or prefix of the task you want to mark as done/remove: ";
	std::getline(std::cin, choice);
	utility::string::convertToLowercase(choice);

	if (choice.empty()) {
		start();
		return;
	}

	// Check if the choice is a number (prefix)
	if (utility::string::isNumber(choice)) {
		int numChoice = utility::string::convertStringToNum(choice);

		// Use entryNumLink to find the corresponding entry
		for (const auto& pair : entryNumLink) {
			if (pair.second == numChoice) {
				selectedEntry = pair.first;
				entryFound = true;
				break;
			}
		}
	}
	else {
		// If not a number, check by name
		for (const auto& entry : entries) {
			if (choice == utility::string::toLowerCase(entry.name)) {
				selectedEntry = entry;
				entryFound = true;
				break;
			}
		}
	}

	if (entryFound) {
		EntryManagement entryManagement(selectedEntry);
		if (!entryManagement.removeEntry()) {
			std::cout << "Failed to remove entry. Please try again\n";
		}
	}
	else {
		std::cout << "No entry with the name or prefix \"" << choice << "\" was found. Please try again\n";
	}

	start();
}

void ActionMenu::editEntry() {

}

void ActionMenu::displayHelp() {

}

#pragma endregion

#pragma region Entry input validation

ActionMenu::EntryValidationResult ActionMenu::EntryValidation::name(const std::string& name) {
	if (name.length() <= 2) {
		return tooShort;
	}
	else if (name.length() >= 50) {
		return tooLong;
	}
	else {
		return success;
	}
}

ActionMenu::EntryValidationResult ActionMenu::EntryValidation::date(const std::string& doDate) {
	if (doDate.length() != 4) {
		return wrongFormat;
	}

	for (char c : doDate) {
		if (!std::isdigit(c)) {
			return wrongFormat;
		}
	}
	int month = std::stoi(doDate.substr(0, 2));
	int day = std::stoi(doDate.substr(2, 2));

	if (month < 1 || month > 12 || day < 1 || day > 31) {
		return wrongFormat;
	}

	return success;
}

ActionMenu::EntryValidationResult ActionMenu::EntryValidation::priority(std::string& priorityInput) {
	utility::string::convertToLowercase(priorityInput);
	if (priorityInput == "y" || priorityInput == "yes" || priorityInput == "true" || priorityInput == "agree" || priorityInput == "priority") {
		return successAndPriorityIsTrue;
	}
	else if (priorityInput == "n" || priorityInput == "no" || priorityInput == "false" || priorityInput == "disagree" || priorityInput == "no priority") {
		return success;
	}
	else {
		return invalidInput;
	}
}

#pragma endregion