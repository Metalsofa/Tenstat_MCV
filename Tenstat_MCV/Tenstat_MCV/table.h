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

#include "index.h"

using namespace std;

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
	//Indices attached to this table
	index* row_labels;
	index* row_descriptions;
	index* column_labels;
	index* column_descriptions;
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
            contents.push_back(vector<tab_dat>(nominal_width));
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
	//Access the indices for this table
	index& get_row_labels() {
		return *row_labels;
	}
	index& get_column_labels() {
		return *column_labels;
	}
    //Default constructor
    table<tab_dat>() {
        label = "Unnamed table";
        nominal_height = 0;
        nominal_width = 0;
    }
	//Vector-table constructor
	table<tab_dat>(string& name, vector<vector<tab_dat>>& vectab) {
		nominal_height = vectab.size();
		nominal_width = vectab.back().size();
		contents = vectab;
		label = name;
	}
	//Vector-table constructor, with row/column labels
	table<tab_dat>(string& name, vector<vector<tab_dat>>& vectab, index& row_names, index& column_names) {
		nominal_height = vectab.size();
		nominal_width = vectab.back().size();
		contents = vectab;
		label = name;
		row_labels = &row_names;
		column_labels = &column_names;
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