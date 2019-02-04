/*Tables are a very basic but easy-to-operate-on data structure.
They should be the go-to structure to put data parsed from files into.
The sofware will be doing a lot of
    -reading files into tables
    -reading tables into other data structures*/
#ifndef __table_h
#define __table_h

#include <vector>
#include <string>
#include <iostream>


//Basic table, all the same class. Cannot have headers 
template<class tab_dat>
class table {
private:
    //Internal representation of this table's name
    std::string label;
    //Rows, Columns
    std::vector<std::vector<tab_dat>> contents;
    //Nominal sizes
    unsigned long nominal_width;
    unsigned long nominal_height;
    //Fix the size properly, even if it means sacrifice
    void fix_dimensions() {
        int Xtrunc = 0;
        int Xpop = 0;
        int Ytrunc = 0;
        int Ypop = 0;
        while (contents.size() > nominal_height) {
            contents.pop_back();
            Ypop++;
        }
        while (contents.size() < nominal_height) {
            contents.push_back(0);
            Ytrunc++;
        }
        for (unsigned int i = 0; i < contents.size(); i++) {
            //Conditional for if this row is too long
            while (contents[i].size() > nominal_width) {
                contents[i].pop_back();
                Xpop++;
            }
            while (contents[i].size() < nominal_width) {
                contents[i].push_back(0);
                Xtrunc++;
            }
        }
        //We keep track of those integers just in case we need them
        //Might turn this message off at some point
        //print_update_message("Fixed dimensions of table " + label); //Doesn't work withou console.h, and including that causes amazing amounts of trouble.
    }
public:
    //Accesor for the name of this table 
    std::string& get_name() {
        return label;
    }
    //Access the contents of this table at particular cell
    tab_dat& operator() (unsigned long i, unsigned long j) {
        if (i > contents.size()) {
            std::cerr << "ERROR: Attempted to index table " 
            << label << " out of bounds vertically: " << i 
            << " > " << contents.size() << std::endl;
            exit (1);
        }
        return contents[i][j];
    }
    //Returns the height, in cells, of this table
    const unsigned long height() {
        fix_dimensions();
        return nominal_height;
    }
    //Returns the width, in cells, of this table
    const unsigned long width() {
        fix_dimensions();
        return nominal_width;
    }
    //Default constructor
    table<tab_dat>() {
        label = "Unnamed table";
        nominal_height = 0;
        nominal_width = 0;
    }
    //Constructor for known width, height
    table<tab_dat>(std::string name, unsigned long _width, unsigned long _height) {
        label = name;
        nominal_width = _width;
        nominal_height = _height;
        fix_dimensions();
    }
};

#endif