/*Your one-stop shop for iostream-based user interactivity.
Also potentially useful beyond that, so don't go scrapping it.*/
#ifndef __console_h
#define __console_h

#include "variable.h"
#include "tensor.h"
#include "project.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

//This list contains ALL commands, even sub-commands.
const vector<string> legal_commands = {
	"INVALID_COMMAND",  //0
	"help",             //1
	"show",             //2
	"web",              //3
	"struct",           //4
	"exit",             //5
	"new",              //6
	"tensor",           //7
	"project",          //8
	"variable",         //9
	"quantitative",     //10
	"categorical",      //11
	"discrete",         //12
	"continuous",       //13
	"intervaled",       //14
	"source",           //15

};

/*Iterates through a vector and returns the index corresponding to the query, if it exists,
returning -1 otherwise*/
template<class guts>
const int search(guts needle, vector<guts> haystack/*Some of these commonly used haystacks are const, so haystack can't be passed by ref. Don't try.*/) {
	for (unsigned int owo = 0; owo < haystack.size(); owo++) {
		if (haystack[owo] == needle) {
			return owo;
		}
	}
	return -1;
}

//Validates a command, returning 0 if there is no such command as the searched one
unsigned int vcom(string command, vector<int> specify) {
	int com = search(command, legal_commands);
	bool legal = search(search(command, legal_commands), specify);
	if (com == -1 && legal && com)
		return 0;
	if (com == 1) { //'help'
		cout << "Here is a list of all valid commands: " << endl;
		for (int i = 1; i < legal_commands.size(); i++) {
			//If the command is in the specified list, show it.
			if (search(i, specify) != -1) {
				cout << legal_commands[i] << endl;
			}
		}
	}
	return com;
}

//For when the user now gets to use a wizard to edit or create a critical project element
void print_wizard_header(string title) {
	for (unsigned long i = 0; i < title.size() + 6; i++) {
		cout << '|';
	}
	cout << endl;
	cout << "|| " << title << " ||" << endl;
	for (unsigned long i = 0; i < title.size() + 6; i++) {
		cout << '|';
	}
	cout << endl;
}

//For any information display
void print_information_header(string title) {
	for (unsigned long i = 0; i < title.size() + 6; i++) {
		cout << '~';
	}
	cout << endl;
	cout << "   " << title << "   " << endl;
	for (unsigned long i = 0; i < title.size() + 6; i++) {
		cout << '~';
	}
	cout << endl;
}

//Subheader printing format
void print_subheader(string title) {
	cout << title << endl;
	for (unsigned int i = 0; i < title.size(); i++) {
		cout << '-';
	}
	cout << endl;
}

void print_error(string error_message, int error_code) {
	if (error_code != -1) {
		cerr << "[" << error_code << "] ERROR: ";
		cerr << error_message << " [" << error_code << ']' << endl;
		exit(error_code);
	}
	cerr << "[!] ERROR: " << error_message << " [!]" << endl;
}

//Prompt the user for a string, then return it.
string prompt_string(string prompt) {
	cout << prompt << endl;
	cout << "<str>>>";
	string rets;
	cin >> ws;
	getline(cin, rets);
	return rets;
}

//Prompt the user for a bool, then return it.
bool prompt_yesno(string prompt) {
	cout << prompt << endl;
	cout << "<y/n>>>";
	string inp;
	cin >> inp;
	return (
		inp == "y" ||
		inp == "" ||
		inp == "Y" ||
		inp == "1" ||
		inp == "yes" ||
		inp == "true" ||
		inp == "Yes" ||
		inp == "True" ||
		inp == "YES" ||
		inp == "TRUE" ||
		inp == "yeah" ||
		inp == "totally" ||
		inp == "affirmative" ||
		inp == "is c++ the best programming language?"
		);
}

void print_options(vector<string>& options) {
	cout << '{' << endl;
	for (unsigned int i = 0; i < options.size(); i++) {
		cout << '\t' << i << ": " << options[i] << endl;
	}
	cout << '}' << endl;
}

int prompt_int(string prompt) {
	cout << prompt << endl;
	cout << "<int>>>";
	int reti;
	cin >> reti;
	return reti;
}

int prompt_option(string prompt, vector<string> options) {
	cout << '(' << prompt << ')';
	print_options(options);
	return prompt_int("") % options.size();
}

//Good for letting the user know a change was made to the file, for example
void print_update_message(string message) {
	cout << '[' << message << ']' << endl;
}

unsigned int prompt_command(string prompt, vector<int> specify) {
	cout << prompt << endl;
	cout << "<...>>>";
	string rets;
	cin.clear();
	cin >> rets;
	int reti = vcom(rets, specify);
	if (reti == 0 || reti == 1) {
		reti = prompt_command(prompt, specify);
	}
	return reti;
}

//Loads a .ten file into the software 

tenstat_project load_project(string filename) {
	ifstream file;
	file.open(filename);
	tenstat_project retp;
	if (file.is_open()) {
		//File parsing and interpreting stuff

	}
	else {
		cerr << "ERROR: Could not open file '" << filename << "'" << endl;
	}

	return retp;
}

//Show's all available info about this project through the console
void print_project_info(tenstat_project& proj) {
	print_information_header("Project Information: " + proj.get_name());
	//Print information about variable web:
	print_subheader("Variables");
	//Pointers to the web and its clusters
	web* webb = &proj.get_web();
	vector<cluster>* clust = &proj.get_web().get_clusters();
	//Iterate through every cluster
	for (unsigned int i = 0; i < clust->size(); i++) {
		//Intruduce the cluster by name and by size
		cout << "Cluster #" << i << ": " << (*clust)[i].get_name() << endl;
		cout << (*clust)[i].get_count() << " variables" << endl;
		//Iterate through every variable in this cluster
		for (unsigned int j = 0; j < (*clust)[i].get_count(); j++) {
			//Display the variable's name
			cout << j << "-" << webb->get_contents()[j + (*clust)[i].get_start()].get_name();
			cout << " - " << webb->get_contents()[j + (*clust)[i].get_start()].vartype_name();
			if (webb->get_contents()[j + (*clust)[i].get_start()].get_vartype() == 3) {
				cout << " - " << webb->get_contents()[j + (*clust)[i].get_start()].get_catc() <<
					" categories";
			}
			cout << endl;
		}
	}
	//Iterate through every variable in the web
	for (unsigned int i = 0; i < webb->get_contents().size(); i++) {
		//But first we have to screen out the Clustered variables we've already displayed
		bool in_cluster = false;
		for (unsigned int j = 0; j < clust->size(); j++) {
			if (i >= (*clust)[j].get_start() && i < (*clust)[j].get_count()) {
				in_cluster = true;
			}
		}
		//If it turns out this variable is not a member of a cluster,
		if (!in_cluster) {
			//Display it's name in the list
			cout << webb->get_contents()[i].get_name() << " - " <<
				webb->get_contents()[i].vartype_name() << " - " <<
				webb->get_contents()[i].get_description() << endl;
		}
	}
	//Print information about data structures
	//NOTDONE
}



#endif