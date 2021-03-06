/*Your one-stop shop for iostream-based user interactivity.
Also potentially useful beyond that, so don't go scrapping it.*/

#ifndef __cons_h
#define __cons_h

#include "variable.h"
#include "tensor.h"
#include "project.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;


//This list contains ALL commands, even sub-commands.
static vector<string> legal_commands = {
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
	"debug",			//16
	"crunch",			//17
	"seginter",			//18
	"table",			//19
	"index",			//20
	"edit",				//21
	"tables",
	"variables",
	"indices",

};

//Let us enumerate these also!
enum numcommand {
	cINVALID_COMMAND,   //0
	chelp,              //1
	cshow,              //2
	cweb,               //3
	cstruct,            //4
	cexit,              //5
	cnew,		        //6
	ctensor,            //7
	cproject,           //8
	cvariable,          //9
	cquantitative,      //10
	ccategorical,       //11
	cdiscrete,          //12
	ccontinuous,        //13
	cintervaled,        //14
	csource,            //15
	cdebug,				//16
	ccrunch,			//17
	cseginter,			//18
	ctable,				//19
	cindex,				//20
	cedit,				//21
	ctables,
	cvariables,
	cindices,

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
		for (int i = 1; i < int(legal_commands.size()); i++) {
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

vector<int> prompt_vec_int(string prompt) {
	cout << prompt << endl;
	cout << "<str>>>";
	string line;
	cin >> ws;
	getline(cin, line);
	stringstream input(line);
	vector<int> retv;
	int push;
	while (input >> push) {
		retv.push_back(push);
	}
	return retv;
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

//Print out a table all nice-like
template <class entry>
void print_object(table<entry>& inkroll) {
	cout << inkroll.get_name() << " - " << inkroll.height() << 'x' << inkroll.width() << endl;
	for (unsigned int i = 0; i < inkroll.width(); i++) {
		cout << '\t' << inkroll.get_column_labels()[i];
	}
	for (unsigned int i = 0; i < inkroll.height(); i++) {
		cout << inkroll.get_row_labels()[i] << '\t';
		for (unsigned int j = 0; j < inkroll.width(); j++) {
			cout << inkroll(i, j) << '\t';
		}
		cout << endl;
	}
}

//Print out an index all nice-like
void print_object(index& dex) {
	cout << dex.get_label() << endl;
	for (unsigned int j = 0; j < dex.size(); j++) {
		cout << '\t' << j << ".\t" << dex[j] << endl;
	}
	cout << endl;
}

//Show's all available info about this project through the console
void print_project_info(tenstat_project& proj) {
	print_information_header("Project Information: " + proj.get_name());
	//Print information about variable web:
	cout << endl;
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
	cout << endl;
	//Print information about indexes
	print_subheader("Indexes");
	vector<string> temp = proj.index_names();
	for (unsigned int i = 0; i < temp.size(); i++) {
		cout << temp[i] + " - " << proj.get_index(i).size() << " entries" << endl;
	}
	cout << endl;
	//Print information about data structures
	//Tables & other Tensors
	//Tables
	print_subheader("Tables");
	temp = proj.table_names();
	for (unsigned int i = 0; i < temp.size(); i++) {
		cout << temp[i] << endl;
	}
	cout << endl;
	//NOTDONE
}

#endif