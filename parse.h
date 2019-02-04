/*This header file contains advanced file parsing methods that
are easily adaptable and are compatible with my own custom
logic*/
#ifndef __parse_h
#define __parse_h

#include "table.h"
#include "index.h"
#include "variable.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>



/*Different files may requre different parsing logic, so the
user can create and save parsing methods, which can be attached
to data sources so Tenstat knows how to interpret them.*/
class parsing_method {
private:
    //Internal representation for this method's name
    std::string label;
    /*List of row-numbers to interpret as labels
    for each of these, an index is made available through
    the data source analyzed.*/
    std::vector<int> label_rows;
    //User can specify an index ahead of time if they wish
    std::vector<index*> labrow_indices;
    //List of column-numbers to be interpreted as purely labels
    std::vector<int> label_columns;
    //User can specify an index ahead of time if they wish
    std::vector<index*> labcol_indices;
    //Delimit a single data source into multiple tables this way
    std::vector<char> table_delimiters;
    //Delimit a body of text into multiple lines
    std::vector<char> row_delimiters;
    //Delimit a single line of text into multiple terms
    std::vector<char> term_delimiters;
    /*If > 0, interpret file as multiple tables, subdivided
    (excluding label columns) according to these*/
    std::vector<int> subdivision_columns;
    //Same as above but for rows
    std::vector<int> subdivision_rows;
public:
    //Builds and returns a table by reading a file stream
    table<int> table_from_ifstream(std::ifstream& reader) {
        table<int> rett;

        return rett;
    }

    //Default constructor; uses basic UNLABELED TABLE format, assumes that CSV and TSV won't conflict
    parsing_method() {
        label = "Unlabeled .tsv/.csv table parsing method";
        //Assume lines are equivalent to rows
        row_delimiters.push_back('\n');
        //Make compatable with CSV and TSV
        term_delimiters.push_back('\t'); //TSV
        term_delimiters.push_back(','); //CSV
    }
    //Know-it-all constructor
    parsing_method(std::string name,
        std::vector<int>& labrows, std::vector<int>& labcols,
        std::vector<index*>& labrowdex, std::vector<index*>& labcoldex,
        std::vector<char>& tabledelimiters,
        std::vector<char>& rowdelimiters, std::vector<char>& termdelimiters, 
        std::vector<int>& subdivisioncolumns, std::vector<int>& subdivisionrows) {
        label = name;
        label_rows = labrows; 
        label_columns = labcols;
        labcol_indices = labcoldex;
        labrow_indices = labrowdex;
        table_delimiters = tabledelimiters;
        row_delimiters = rowdelimiters;
        term_delimiters = termdelimiters;
        subdivision_columns = subdivisioncolumns;
        subdivision_rows = subdivision_rows;
    }

    //Built-in constructors
    parsing_method(std::string name, int default_setting) {
        label = name;
        switch (default_setting) {
            case 0: //UNLABELED TABLE
                
            break;
            case 1: //LABELED TABLE

            break;
            case 2: //UNLABELED LIST

            break;
            case 3: //LABELED LIST

            break;
        }
        row_delimiters.push_back('\n');
        term_delimiters.push_back('\t');
        term_delimiters.push_back(',');
    }
    //Built in constructor; uses basic LABELED LIST format

    //Get the indexes recognized by this parsing method
};



//I'm not really sure why I made this class. Bye, parser.
/*
class parser {
private:
//Contents for parsing files
    std::string p_filename;
    fstream p_file;
    //The table contains room for all contents, 
    //row labels, and column labels.
    std::vector<std::vector<std::string>> s_table;
    //Whether or not row labels are detected
    bool c_labels;
    bool r_labels;
public:
    bool clear() {
        close();
        s_table = std::vector<std::vector<std::string>>();
    }
    bool read(std::string file_name) {
        p_file.open(file_name);
        if (p_file.is_open())
            return true;
        return false;
    }
    bool close() {
        if (p_file.is_open()) {
            p_file.close();
            return true;
        }
        return false;
    }
    bool write(std::string file_name) {

    }
};
*/


#endif