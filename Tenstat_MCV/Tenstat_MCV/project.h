/*The members of this object should contain every property regarding an analysis project
that a user could possibly want to save, such as the data, how it is structured, where it came from,
when it was implemented, et. cetera.
This header file will also contain the methods for reading/writing software to/from files.*/
#ifndef __project_h
#define __project_h

#include "variable.h"
#include "tensor.h"
#include "table.h"
#include "index.h"
#include "data_source.h"
#include "plots.h"
#include "parse.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//Returns false if it failed to save
bool write_project(std::string filename) {
    std::ofstream write;
    write.open(filename);
    if (write.is_open()) {
        
    }
    else {
        return false;
    }
    return true;
}

//Data structure containting all the save-able attributes of a project
class tenstat_project {
private:
    //The name of this project
    std::string label;
    //Variable web
    web proj_web;
    //Various Structs
    std::vector<table<std::string>> proj_string_tables;
    std::vector<table<double>> proj_double_tables;
    std::vector<table<int>> proj_int_tables;
    std::vector<table<float>> proj_float_tables;
        //These float*'s point to the appropriate values of the variables they are assigned to
    std::vector<tensor<float*>> proj_tensors;
    //Data Sources (Addresses & stuff)
    std::vector<data_source> proj_data_sources;
    //Parsing methods
	vector<parsing_method> proj_parsing_methods;
    //Sorting Algorithms

    //Plots
	vector<plot> proj_plots;
    //Equations

    //Indexes
    std::vector<index> proj_indexes;
    //Results repositories
	vector<seginter_results> proj_seginter_repository;
    //True if the current version is saved/backed up
    bool saved;
public:
    //Adding a variable to the web

    //Name constructor
    tenstat_project(std::string name) {
        label = name;
        proj_web = web();
        saved = false;
		//Standard inclusions
		proj_indexes.push_back(index("Alphabet", "The Roman alphabet, all uppercase",
			{ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
			"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" }));
    }
	//Default constructor
	tenstat_project() {
		label = "Untitled Tenstat Project";
		proj_web = web();
		saved = false;
	}
    //Returns a refrence to this project's file name
    std::string& get_name() {
        return label;
    }
    //Returns a refrence to this project's variable web
    web& get_web() {
        return proj_web;
    }
    //Save this project to a .ten file with it's name
    void save() {
        write_project(label + ".ten");
    }
	//Returns a refrence to one of this project's many tables
	table<float>& get_float_table(unsigned long table_ID) {
		return proj_float_tables[table_ID];
	}
    //Save this project to a different .ten file
    void save_as(std::string new_name) {
        label = new_name;
        save();
    }
    //Returns a refrence to this project's index vector
    index& get_index(unsigned long whomst) { //Don't bother me about spelling...
        return proj_indexes[whomst];
    }
	//Returns an option list for the tables in this project
	vector<string> table_names() {
		vector<string> retv;
		for (unsigned int i = 0; i < proj_float_tables.size(); i++) {
			retv.push_back(proj_float_tables[i].get_name() + " " + to_string(proj_float_tables[i].height()) + "x" + to_string(proj_float_tables[i].width()));
		}
		return retv;
	}
	//Returns an option list for the indices in this project
	vector<string> index_names() {
		vector<string> retv;
		for (unsigned int i = 0; i < proj_indexes.size(); i++) {
			retv.push_back(proj_indexes[i].get_label() + " (" + to_string(proj_indexes[i].size()) + " entries) - " + proj_indexes[i].get_description());
		}
		return retv;
	}
	//Returns an option list for the parsing methods in this project
	vector<string> parsing_names() {
		vector<string> retv;
		for (unsigned int i = 0; i < proj_parsing_methods.size(); i++) {
			retv.push_back(proj_parsing_methods[i].get_name() + " - " + proj_parsing_methods[i].get_description());
		}
	}
	//Returns a refrence to this project's seginter results repository
	vector<seginter_results>& get_seginter_repository() {
		return proj_seginter_repository;
	}
    /*Add an additional index to this project's saved ones.
    Returns false if there was a namespace collision*/
    bool add_index(index& new_dex) {
        //Doublecheck that there isn't a name collision
        for (unsigned int i = 0; i < proj_indexes.size(); i++) {
            if (new_dex.get_label() == proj_indexes[i].get_label()) {
                std::cerr << "Could not add index '" << 
                new_dex.get_label() << 
                "'; namespace collision with index #" << i 
                << std::endl;
                return false;
            }
        }
        proj_indexes.push_back(new_dex);
        return true;
    }
    //Add another data path to the project
    void add_data_source(data_source source) {
        proj_data_sources.push_back(source);
    }
};

#endif