#pragma once
#include "Entry.h"
#include "UtilityFunctions.h"
#include "nlohmann/json.hpp"
#include <vector>
using json = nlohmann::json;

class EntryManagement {
public:
	EntryManagement(Entry entry);
	~EntryManagement();

	std::string entriesFilePath = utility::console::getEntryFilePath();

	// Add new entry
	bool addEntry(); // Main method
	// Derived methods
	bool loadExistingEntries();
	void createNewEntry();
	bool saveEntriesToFile();

	bool editEntryDetails();
	bool removeEntry();

private:
	Entry entry;
	std::vector<json> entries;
};

class Database {
public:
	static std::vector<Entry> loadEntries();
	static bool resetEntries();
};

