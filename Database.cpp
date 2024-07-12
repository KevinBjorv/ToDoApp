#include "Database.h"
#include "MenuManager.h"
#include <istream>
#include <fstream>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <vector>

EntryManagement::EntryManagement(Entry entry) : entry(entry) {
	
}

bool EntryManagement::addEntry() {
	Dashboard dashboard;
	bool failure = false;

	if (!loadExistingEntries()) {
		failure = true;
	}
	
	createNewEntry(); // Void - No error checking
	
	if (!saveEntriesToFile()) {
		failure = true;
	} 
	
	if (failure) {
		std::cerr << "Error: The task was not successfully added, please try again later\n";
		return false;
	} 

	//DEBUG: std::cout << "Successfully added the task to file: " << entriesFilePath << "\n";
	return true;
}

bool EntryManagement::loadExistingEntries() {
	std::ifstream entriesFileIn(entriesFilePath);
	if (entriesFileIn.is_open()) {
		if (entriesFileIn.peek() == std::ifstream::traits_type::eof()) {
			// File is empty, initialize entries to an empty array
			entries = std::vector<json>();
			entriesFileIn.close();
			return true;
		}

		try {
			json j;
			entriesFileIn >> j;
			if (j.is_array()) {
				entries = j.get<std::vector<json>>();
			} else {
				// If JSON is not an array, initialize entries to an empty array
				entries = std::vector<json>();
			}
		} catch (const std::exception& e) {
			std::cerr << "Error reading entry file: " << e.what() << std::endl;
			// Initialize entries to an empty array on error
			entries = std::vector<json>();
			entriesFileIn.close();
			return false;
		}
		entriesFileIn.close();
	} else {
		// If the file doesn't exist, initialize entries to an empty array
		entries = std::vector<json>();
	}
	return true;
}

void EntryManagement::createNewEntry() {
	json newEntry;
	newEntry["name"] = entry.name;
	newEntry["doDate"] = entry.doDate;
	newEntry["isPriority"] = entry.priority ? "true" : "false";
	newEntry["isDone"] = entry.done; // Should be false
	entries.push_back(newEntry); // Append new entry
}

bool EntryManagement::saveEntriesToFile() const {
	std::ofstream entryFileOut(entriesFilePath, std::ios::trunc); // Open in truncation mode
	if (!entryFileOut.is_open()) {
		std::cerr << "Failed to open file for writing: " << entriesFilePath << std::endl;
		return false;
	}
	try {
		json j = entries; // Convert entries vector to JSON array
		entryFileOut << j.dump(4); // Write the entire JSON array to the file
		entryFileOut.close();
	}
	catch (const std::exception& e) {
		std::cerr << "Error writing to entry file: " << e.what() << std::endl;
		entryFileOut.close();
		return false;
	}
	return true;
}

bool EntryManagement::removeEntry() {
	if (!loadExistingEntries()) {
		std::cout << "Failed to load existing entries for removal\n";
		std::cin.get();
		return false;
	}

	// Remove the entry from the vector
	auto it = std::remove_if(entries.begin(), entries.end(), [this](const json& entryJson) {
		Entry tempEntry(
			entryJson.at("name").get<std::string>(),
			entryJson.at("doDate").get<std::string>(),
			entryJson.at("isPriority").get<std::string>() == "true",
			entryJson.at("isDone").get<bool>()
		);
		return tempEntry == this->entry;
		});

	if (it != entries.end()) {
		entries.erase(it, entries.end());
	} else {
		std::cerr << "Entry not found in the database\n";
		return false;
	}

	return saveEntriesToFile();
}

bool EntryManagement::editEntryDetails(const std::string& detailName) {
	if (!loadExistingEntries()) {
		std::cerr << "Failed to load existing entries for editing.\n";
		return false;
	}

	// Locate the entry to edit
	auto it = std::find_if(entries.begin(), entries.end(), [this](const json& entryJson) {
		return entryJson.at("name").get<std::string>() == entry.name;
		});

	if (it == entries.end()) {
		std::cerr << "Entry not found for editing.\n";
		return false;
	}

	// Edit the specific detail
	if (detailName == "name") {
		std::string newName;
		std::cout << "Enter new name: ";
		std::getline(std::cin, newName);
		it->at("name") = newName;
		entry.name = newName; // Update the current entry object as well
	}
	else if (detailName == "doDate") {
		std::string newDoDate;
		std::cout << "Enter new do-date: ";
		std::cin >> newDoDate;
		it->at("doDate") = newDoDate;
		entry.doDate = newDoDate; // Update the current entry object as well
	}
	else if (detailName == "priority") {
		std::string newPriority;
		std::cout << "Do you want to set this task as priority? (true/false): ";
		std::cin >> newPriority;
		it->at("isPriority") = newPriority;
		entry.priority = (newPriority == "true"); // Update the current entry object as well
	}
	else if (detailName == "done") {
		bool newDone;
		std::cout << "Enter new done status (0/1): ";
		std::cin >> newDone;
		it->at("isDone") = newDone;
		entry.done = newDone; // Update the current entry object as well
	}
	else {
		std::cerr << "Invalid detail name.\n";
		return false;
	}

	// Save the updated entries to the file
	return saveEntriesToFile();
}

#pragma region Database
std::vector<Entry> Database::loadEntries() {
		std::vector<Entry> entries;
		std::string entriesFilePath = utility::console::getEntryFilePath();
		std::ifstream entriesFileIn(entriesFilePath);

		if (entriesFileIn.is_open()) {
			if (entriesFileIn.peek() == std::ifstream::traits_type::eof()) {
				std::cout << "You have no tasks left. Great!\n";
				entriesFileIn.close();
				return entries;
			}

			try {
				json j;
				entriesFileIn >> j;

				if (j.is_array()) {
					for (const auto& entryJson : j) {
						Entry entry(
							entryJson.at("name").get<std::string>(),
							entryJson.at("doDate").get<std::string>(),
							entryJson.at("isPriority").get<std::string>() == "true",
							entryJson.at("isDone").get<bool>()
						);
						entries.push_back(entry);
					}
				}
			}
			catch (const std::exception& e) {
				std::cerr << "Error reading entry file: " << e.what() << std::endl;
				entriesFileIn.close();
				return {};
			}

			entriesFileIn.close();
		}
		entries.insert(entries.begin(), Entry()); // Make the first element empty
		return entries;
}

bool Database::resetEntries() {
	return true;
}
#pragma endregion