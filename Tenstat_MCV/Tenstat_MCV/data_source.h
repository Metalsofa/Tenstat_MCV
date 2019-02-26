/*This header file is for the data_source object, which keeps
track of the file locations of data sources for this project and
how they should be parsed and interpreted automatically */
#ifndef __data_source_h
#define __data_source_h

#include "parse.h"

#include <string>
#include <fstream>

using namespace std;

class data_source {
private:
    //Location on this computer where the data is located
    std::string location;
    //Information about how the data file should be parsed
    parsing_method interpretation;
public:
    //Default constructor
    data_source() {
        location = "";
        interpretation = parsing_method();
    }
	//Location-only constructor
    data_source(std::string filepath) {
        location = filepath;
        interpretation = parsing_method();
    }
    /*Function that faithfully executes this parsing method
    on this file and returns the information so that it can be
    loaded into a table*/
    //no here yet
};

string get_file_extention(string& filename) {
	string rets = "";
	bool ext;
	for (unsigned int i = 0; i < filename.size(); i++) {
		if (filename[i] == '.') {
			ext = true;
		}
		if (ext) {
			rets.push_back(filename[i]);
		}
	}
	return rets;
}

#endif