#pragma once
#include <string>

class Entry {
public:
	Entry() : name(""), doDate(""), priority(false), done(false) {} // Default constructor - First element
	Entry(const std::string& name, const std::string& doDate, bool priority, bool done = false);
	~Entry() = default;

	bool operator==(const Entry& other) const;

	std::string name;
	std::string doDate;
	bool priority = false;
	bool done = false;
};

namespace std {
	template <>
	struct hash<Entry> {
		std::size_t operator()(const Entry& e) const {
			return std::hash<std::string>()(e.name) ^
				(std::hash<std::string>()(e.doDate) << 1) ^
				(std::hash<bool>()(e.priority) << 2) ^
				(std::hash<bool>()(e.done) << 3); 
		}
	};
}