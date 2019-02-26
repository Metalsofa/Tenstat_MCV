/*User-defined (or computer-defined) indexes help them and the software make sense of categories */
#ifndef __index_h
#define __index_h


#include <string>
#include <vector>

#include "analysis.h"

class index {
private:
    //Internal representation of the name of this index
    std::string label;
    //Internal representation of the description of this index
    std::string description;
    //The strings that this index defines
    std::vector<std::string> entries;
    //Whether the order of this index has any meaningful order is not said to have any non-arbitrary order
    bool ordered;
public:
    const std::string& get_label() {
        return label;
    }
    const std::string& get_description() {
        return description;
    }
    //Returns non-const; you can use this as an lvalue
    std::string& operator[] (unsigned long ind) {
        return entries[ind];
    }
    void push_back(std::string entry) {
        entries.push_back(entry);
    }
    //Returns const; you cannot use this as an lvalue
    const unsigned long operator() (std::string gloss) {
        for (unsigned long q; q < entries.size(); q++) {
            if (entries[q] == gloss) {
                return q;
            }
        }
    }
    //Returns the size of this index
    const unsigned long size() {
        return entries.size();
    }
	//Elaborate "know-it-all" constructor
	index(std::string _label, std::string _description, std::vector<std::string> labels) {
		label = _label;
		description = _description;
		entries = labels;
	}
	datarange<string> get_datarange() {
		return datarange<string>(entries);
	}
};

#endif