/*This .cpp file is for handling various syntheses and setups in a user-friendly way
Also contains the main loop*/

#ifndef __wizard_h
#define __wizard_h

#include "variable.h"
#include "tensor.h"
#include "index.h"
#include "project.h"
#include "console_interface.h"

#include <iostream>
#include <string>


using namespace std;

/*TABLE WIZARD: Walk the user through the creation of a new table from a file.*/
void wiz_new_table(tenstat_project& proj) {
	print_wizard_header("New Table");
	table<float> addtab;
	string tabn = prompt_string("What is the name for this table?");
	int indexrow = prompt_option("What index is to be used for the row labels?", proj.index_names());
	int indexcol = prompt_option("What index is to be used for the column labels?", proj.index_names());
	if (prompt_yesno("Assign description indices as well?")) {
		int descrow = prompt_option("What index is to be used for the row descriptions?", proj.index_names());
		int desccol = prompt_option("What index is to be used for the column descriptions?", proj.index_names());
		addtab = table<float>(tabn, proj.get_index(indexrow), proj.get_index(indexcol),
			proj.get_index(descrow), proj.get_index(desccol));
		return;
	}
	addtab = table<float>(tabn, proj.get_index(indexrow), proj.get_index(indexcol));
	proj.add_ftable(addtab);
}

/*TENSOR WIZARD: Walk the user through the creation of a new tensor from some other tensors.*/
void wiz_new_tensor(tenstat_project& proj) {
	print_wizard_header("New Tensor");
	string tname = prompt_string("Enter a name for this tensor.");
	unsigned long dimc = prompt_int("How many dimensions should this tensor have?");
	vector<variable*> tvar;
	for (unsigned int i = 0; i < tvar.size(); i++) {
		tvar.push_back(&(proj.get_web().get_contents()[prompt_option(
			"Assign a variable to dimension " + to_string(i) + ".",
			proj.variable_names())]));
	}
	tensor<float> newtens(tname, tvar);
	proj.add_tensor(newtens);
}

/*VARIABLE WIZARD: Walk the user through the definition of a new variable.
Be sure to pass a refrence to the project this variable is being added to*/
void wiz_new_variable(tenstat_project& proj) {
	print_wizard_header("New Variable");
	//Initialize a bunch of parts of what this new variable could potentially be
	variable addvar;
	int vartype;
	string varname = prompt_string("Enter a name for new this variable:");
	string vardesc = prompt_string("Enter a description for " + varname + ":");
	unsigned long catc;
	index* labdex;
	index* descdex;
	bool use_lab;
	bool use_desc;
	int command = prompt_command("What kind of variable?", { 10, 11 });
	if (command == 10) { //'quantitative'
		//Set this variable's 'type'
		command = prompt_command("Discrete, continuous, or intervaled?", { 12, 13, 14 });
		switch (command) {
		case 12: //'discrete'
			vartype = 1;
			break;
		case 13: //'continuous'
			vartype = 0;
			break;
		case 14: //'intervaled'
			vartype = 2;
			break;
		}
		//Finally construct the QUANTITATIVE variable
		addvar = variable(varname, vardesc, vartype);
	}
	else if (command == 11) { //Categorical
		//Get from the user the number of categories
		catc = prompt_int("How many categories does this variable have?");
		//Double check if they mean intervaled or not
		switch (prompt_yesno("Is this 'categorical' variable actually just an intervaled quantitative? In otherwords, is there some meaningfil (eg. chronological) order to it?")) {
		case 0: //No, it's really categorical
			vartype = 3;
			switch (prompt_yesno("Attach index for category labels?")) {
			case 1: //Yes, attach an index for labels
				use_lab = true;
				//Show list of indexes for this project and let them choose or cancel

				//labdex = &(something)
				switch (prompt_yesno("Attach index for category descriptions?")) {
				case 1: //Yes, attach an index for descriptions
					use_desc = true;
					//Show list of indexes for this project and let them choose or cancel

					//descdex = &(something)
					break;
				case 0://No thanks, just label indices
					break;
				}
				break;
			case 0: //No thanks, no indices neccessary
				break;
			}

			//Finally construct the CATEGORICAL variable
			if (use_lab) {
				if (use_desc) {
					//Initialize with label and description indices
					///addvar = variable(varname, vardesc, catc, labdex, descdex);
					//We'll ad these back in when it's time
				}
				else {
					//Initialize with label indexes only
					///addvar = variable(varname, vardesc, catc, labdex);
				}
			}
			else {
				//Initialize with no indexes
				addvar = variable(varname, vardesc, catc);
			}
			break;
		case 1: //Yeah, it's actually intervaled
			vartype = 2;
			//Finally construct the INTERVALED variable
			addvar = variable(varname, vardesc, catc, true);
			break;
		}
	}
	int where = -1;
	//Check if the user wishes to group this variable with an existing cluster
	if (proj.get_web().get_clusters().size() > 0) {
		if (prompt_yesno("Add this variable to an existing cluster?")) {
			where = prompt_option("Which cluster should '" + varname + "' be added to?",
				proj.get_web().get_cluster_names());
		}
	}
	else {
		cout << "(There exist no clusters to add this variable to)" << endl;
	}
	//Add the constructed variable to the web at the desired location.
	proj.get_web().add_variable(addvar, where);
	print_update_message(varname + " added to this project's web");
}

/*INDEX WIZARD: Walk the user through the definition of a new index.
Be sure to pass a refrence to the project this variable is being added to*/
void wiz_new_index(tenstat_project& proj) {
	print_wizard_header("New Index");
	index addex(prompt_string("Enter a name for this index."),
		prompt_string("Enter a description for this index."));
	proj.add_index(addex);
	return;
}



#endif