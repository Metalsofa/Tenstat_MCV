/*This header file is for the data_source object, which keeps
track of the file locations of data sources for this project and
how they should be parsed and interpreted automatically */
#ifndef __data_source_h
#define __data_source_h

#include "parse.h"

#include <string>
#include <fstream>



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
        
    }

    /*Function that faithfully executes this parsing method
    on this file and returns the information so that it can be
    loaded into a table*/
};

#endif