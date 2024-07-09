#include "Entry.h"


Entry::Entry(const std::string& name, const std::string& doDate, bool priority, bool done) : name(name), doDate(doDate), priority(priority), done(done) {

}

bool Entry::operator==(const Entry& other) const {
    return name == other.name && doDate == other.doDate && priority == other.priority && done == other.done;
}