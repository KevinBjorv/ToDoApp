#pragma once
#include <string>
#include <functional>
#include <map>
#include <sstream>
#include <memory>
#include <optional>
#include "Entry.h"

class ActionMenu;

class Dashboard {
public:
    explicit Dashboard();
    virtual ~Dashboard() = default;

    void start(const bool clearScreen = true);
    std::unordered_map<Entry, int> entryNumLink; // Links the number that's displayed before the entry name on the dashboard
    std::vector<Entry> currentEntries;
    void setActionMenu(std::shared_ptr<ActionMenu> actionMenu);

private:
    struct MenuOptions {
        std::vector<std::pair<std::string, char>> option; // Name & Prefix

        MenuOptions() {
            option.push_back({ "Add entry", 'a' });
            option.push_back({ "Edit entry", 'b' });
            option.push_back({ "Remove entry", 'c' });
            option.push_back({ "Settings", 'd' });
            option.push_back({ "Exit", 'e' });
        }
    };

    virtual void displayTasks();
    virtual void displayOptions();


    // Dashboard choice handling

    virtual void handleChoice(std::string& choice);
    bool handleBasicCommand(const std::string& choice);
    bool handleAdvancedCommand(std::string& choice);
    std::optional<std::pair<int, char>> validateAdvancedCommand(std::string& command);

    void openSettingsMenu();
    void displayHelp();


    void addEntryMenu();
    virtual void removeEntryMenu();
    virtual void editEntryMenu();
    void editEntryFromCommand(Entry* selectedEntry);

    std::shared_ptr<ActionMenu> actionMenu;
    MenuOptions menuOption;
};

class ActionMenu {
public:
    ActionMenu(std::shared_ptr<Dashboard> dashboard);
    ~ActionMenu() = default;

    // Implementations of entry management methods
    void addEntry(std::string& nameInput, std::string& doDateInput, std::string& priorityInput);
    void removeEntry(std::string& choice);

    // Edit entry
    Entry* getSelectedEntryForEdit(std::string& entryChoice);
    virtual bool handleEditEntryChoice(std::string& taskChoice, Entry& selectedEntry);


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

    std::shared_ptr<Dashboard> dashboard;
};
