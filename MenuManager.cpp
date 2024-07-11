#include "MenuManager.h"
#include "Database.h"
#include "UtilityFunctions.h"
#include "Entry.h"
#include <iostream>
#include <cctype>
#include <vector>

Dashboard::Dashboard() {
    std::cout << "Dashboard constructor called" << std::endl;
}

void Dashboard::setActionMenu(std::shared_ptr<ActionMenu> actionMenu) {
    this->actionMenu = actionMenu;
}

ActionMenu::ActionMenu(std::shared_ptr<Dashboard> dashboard) : dashboard(dashboard) {
    std::cout << "ActionMenu constructor called" << std::endl;
}

#pragma region Non-Virtual Implementations
void Dashboard::start(const bool clearScreen) {
    if (clearScreen) {
        utility::console::clearScreen();
    }
    utility::console::setTitle("Dashboard");
    displayOptions();
}
#pragma endregion

#pragma region Dashboard
void Dashboard::displayOptions() {
    std::cout << "Welcome to the To-Do list Dashboard\n";
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

    if (choice.empty()) {
        start(); // Restart dashboard menu with no warning
        return;
    }

    bool success = true;
    if (utility::string::containsNumber(choice)) {
        success = handleAdvancedCommand(choice);
    }
    else {
        success = handleBasicCommand(choice);
    }
    
    if (!success) {
        utility::console::clearInputBuffer();
        std::cout << "Invalid choice. Please type \"help\" for help (Press any key to continue)\n";
    }
    else {
        start(); // If success and the function opened by the handleCommand function returned
    }
    std::cin.get(); // Handlecommand returned false
    start();
    return;
}

bool Dashboard::handleBasicCommand(const std::string& choice) {
    for (const auto& pair : menuOption.option) {
        std::string lowerKey = utility::string::toLowerCase(pair.first); // Convert keys to lowercase for better comparison

        // Compare the choice with the lowercase key and the prefix (pair.second)
        if (choice == lowerKey || choice == std::string(1, pair.second)) {
            if (pair.second == 'a') {
                addEntryMenu();
                return true;
            }
            else if (pair.second == 'b') {
                editEntryMenu();
                return true;
            }
            else if (pair.second == 'c') {
                removeEntryMenu();
                return true;
            }
            else if (pair.second == 'd') {
                openSettingsMenu();
                return true;
            }
            else if (pair.second == 'e') {
                std::cout << "Exiting...";
                exit(1);
            }
            return false;
        }
    }
}

bool Dashboard::handleAdvancedCommand(std::string& choice) {
    auto result = validateAdvancedCommand(choice); // Separate num (task) and char (process)
    if (!result) {
        return false; // Invalid command
    }

    if (result->second == 'a') {
        return false; // Cannot add entry via advanced function: Because a specific task input is not needed
    }
    else if (result->second == 'b') {
        int prefixNumber = result->first;
        auto it = std::find_if(entryNumLink.begin(), entryNumLink.end(),
            [prefixNumber](const auto& pair) {
                return pair.second == prefixNumber;
            });
        if (it != entryNumLink.end()) {
            Entry* p_SelectedEntry = const_cast<Entry*>(&it->first);
            editEntryFromCommand(p_SelectedEntry);
            return true;
        }
        else {
            std::cout << "Entry with prefix " << prefixNumber << " not found." << std::endl;
            return false;
        }
    }
    else if (result->second == 'c') {
        int prefixNumber = result->first;
        auto it = std::find_if(entryNumLink.begin(), entryNumLink.end(),
            [prefixNumber](const auto& pair) {
                return pair.second == prefixNumber;
            });
        if (it != entryNumLink.end()) {
            Entry* p_SelectedEntry = const_cast<Entry*>(&it->first);
            std::string entryPrefix = std::to_string((result->first));
            actionMenu->removeEntry(entryPrefix);
            return true;
        }
        else {
            return false;
        }
    }
    
    return false;
}

std::optional<std::pair<int, char>> Dashboard::validateAdvancedCommand(std::string& command) {
    if (!std::isalpha(command.back())) { // Check if command ends with an alphabetic character
        return std::nullopt;
    }

    char character = command.back();
    std::string numberPart = command.substr(0, command.length() - 1);

    if (numberPart.empty())  {
        return std::nullopt;
    }

    utility::string::isNumber(numberPart);
    int number = std::stoi(numberPart);

    return std::make_pair(number, character);
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

void Dashboard::openSettingsMenu() {
    // Temporary method implementation
    std::string line;
    std::cout << "1. Help\n";
    std::cout << "Enter choice: ";
    std::getline(std::cin, line);
    utility::string::convertToLowercase(line);

    if (line == "help") {
        displayHelp();
    }
    else {
        start();
    }
}

#pragma region Entry Management

void Dashboard::addEntryMenu() {
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
    actionMenu->addEntry(name, doDate, priorityInput);
}

void Dashboard::removeEntryMenu() {
    utility::console::setTitle("Remove Task");

    std::string choice;
    std::cout << "Enter the name or prefix of the task you want to mark as done/remove: ";
    std::getline(std::cin, choice);
    actionMenu->removeEntry(choice);
}

void Dashboard::editEntryMenu() {
    utility::console::setTitle("Edit Task");

    std::string entryChoice;
    std::cout << "Enter the name or prefix of the task you want to edit: ";
    std::getline(std::cin, entryChoice);
    Entry* selectedEntry = actionMenu->getSelectedEntryForEdit(entryChoice);
    if (selectedEntry == nullptr) {
        std::cout << "The selected entry was not found. Please try again\n";
        std::cin.get();
        start();
        return;
    }

    std::cout << "Name: " << selectedEntry->name << "\n";
    std::cout << "Do-date: " << selectedEntry->doDate << "\n";
    std::cout << "Is priority: " << selectedEntry->priority << "\n";
    std::cout << "Is done: " << selectedEntry->done << "\n";

    std::string detailChoice;
    std::cout << "Enter the name of the detail you want to edit: ";
    std::getline(std::cin, detailChoice);

    if (!actionMenu->handleEditEntryChoice(detailChoice, *selectedEntry)) {
        std::cout << "Please try again" << std::endl;
        start();
    }

    std::cout << "The entry detail \"" << detailChoice << "\" was successfully changed\n";
    std::cin.get();
    start();
}

void Dashboard::editEntryFromCommand(Entry* selectedEntry) {

    // Selected entry details based from command
    std::cout << "Name: " << selectedEntry->name << "\n";
    std::cout << "Do-date: " << selectedEntry->doDate << "\n";
    std::cout << "Is priority: " << selectedEntry->priority << "\n";
    std::cout << "Is done: " << selectedEntry->done << "\n";

    std::string detailChoice;
    std::cout << "Enter the name of the detail you want to edit: ";
    std::getline(std::cin, detailChoice);

    if (!actionMenu->handleEditEntryChoice(detailChoice, *selectedEntry)) {
        std::cout << "Please try again" << std::endl;
        start();
    }

    std::cout << "The entry detail \"" << detailChoice << "\" was successfully changed\n";
    std::cin.get();
    start();
}
#pragma endregion

#pragma region Entry Management Implemenations
void ActionMenu::addEntry(std::string& nameInput, std::string& doDateInput, std::string& priorityInput) {
    EntryValidation validate;

    bool priority = false;
    bool failure = false;

    if (validate.name(nameInput) == success) {
        if (validate.date(doDateInput) == success) {
            if (validate.priority(priorityInput) == success) {
                priority = false;
            }
            else if (validate.priority(priorityInput) == successAndPriorityIsTrue) {
                priority = true;
            }
            else {
                std::cout << "Do-date was not valid due to reason: " << validate.priority(priorityInput);
                failure = true;
            }
        }
        else {
            std::cout << "Do-date was not valid due to reason: " << validate.date(doDateInput);
            failure = true;
        }
    }
    else {
        std::cout << "Name was not valid due to reason: " << validate.name(nameInput);
        failure = true;
    }
    std::cout << std::endl;

    if (!failure) {
        Entry newEntry(nameInput, doDateInput, priority);
        EntryManagement entryManagement(newEntry);
        entryManagement.addEntry();
    }
    dashboard->start();
}

void ActionMenu::removeEntry(std::string& choice) {
    utility::string::convertToLowercase(choice);
    std::vector<Entry> entries = Database::loadEntries();
    Entry selectedEntry;
    bool entryFound = false;

    if (choice.empty()) {
        dashboard->start();
    }

    // Check if the choice is a number (prefix)
    if (utility::string::isNumber(choice)) {
        int numChoice = utility::string::convertStringToNum(choice);

        // Use entryNumLink to find the corresponding entry
        for (const auto& pair : dashboard->entryNumLink) {
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
            std::cout << "Failed to edit entry. Please try again\n";
        }
    }
    else {
        std::cout << "No entry with the name or prefix \"" << choice << "\" was found. Please try again\n";
    }

    dashboard->start();
}

Entry* ActionMenu::getSelectedEntryForEdit(std::string& entryChoice) {
    utility::string::convertToLowercase(entryChoice);
    std::vector<Entry> entries = Database::loadEntries();
    bool entryFound = false;
    Entry* selectedEntry = nullptr;

    if (entryChoice.empty()) {
        return nullptr;
    }

    // Check if the choice is a number (prefix)
    if (utility::string::isNumber(entryChoice)) {
        int numChoice = utility::string::convertStringToNum(entryChoice);

        // Use entryNumLink to find the corresponding entry
        for (const auto& pair : dashboard->entryNumLink) {
            if (pair.second == numChoice) {
                for (auto& entry : entries) {
                    if (entry == pair.first) {
                        // Ensure selectedEntry points to a persistent object
                        dashboard->currentEntries.push_back(entry);
                        selectedEntry = &dashboard->currentEntries.back();
                        entryFound = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    else {
        // If not a number, check by name
        for (auto& entry : entries) {
            if (entryChoice == utility::string::toLowerCase(entry.name)) {
                // Ensure selectedEntry points to a persistent object
                dashboard->currentEntries.push_back(entry);
                selectedEntry = &dashboard->currentEntries.back();
                entryFound = true;
                break;
            }
        }
    }

    return entryFound ? selectedEntry : nullptr;
}

bool ActionMenu::handleEditEntryChoice(std::string& taskChoice, Entry& selectedEntry) {
    utility::string::convertToLowercase(taskChoice);
    EntryManagement entryManagement(selectedEntry);

    if (taskChoice == "name" || taskChoice == selectedEntry.name) {
        if (!entryManagement.editEntryDetails("name"))
            return false;
    }
    else if (taskChoice == "do-date" || taskChoice == "dodate" || taskChoice == selectedEntry.doDate) {
        if (!entryManagement.editEntryDetails("dodate"))
            return false;
    }
    else if (taskChoice == "is priority" || taskChoice == "priority") {
        if (!entryManagement.editEntryDetails("priority"))
            return false;
    }
    else if (taskChoice == "is done" || taskChoice == "done") {
        if (!entryManagement.editEntryDetails("done"))
            return false;
    }
    else {
        std::cout << "Invalid choice. ";
        return false;
    }
    return true;
}

void Dashboard::displayHelp() {
    std::cout << "Command options\n";
    std::cout << " 1. Write the prefix of one of the options on the dashboard. These are prefixed with a letter, starting from the letter \'a\'\n";
    std::cout << " 2. Type in the prefixed integer of a task followed by a relevant action's prefix, to edit that specific entry. (Example 2c to remove task 2)\n";
    std::cout << " 3. To customize your experience, or edit specific options about the application, open the settings menu by typing it's prefixed letter\n";
    std::cout << " 4. To safely exit the application, either type \"exit\" or simple \'e\'\n";
    std::cout << " 5. For more information about the application, read the README file found on Github or in the base directory\n";
    std::cout << "\nWhen you are done reading, please press Enter to continue";
    std::cin.get();
    start();
}

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
