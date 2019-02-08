/*The purpose of this program is to crunch numbers from many sources,
across multiple variables and variable types, especially with really big data,
in a way that any data analysis the user possibly wanted to do could be done
intuitively.*/
/*This .cpp file in particular pertains to a console-only version of this software
which will later be fitted to GUI.*/

//Custom Headers
#include "software_info.h" //For managing simple messages about the software itself
#include "variable.h" //Data layout for the user is in terms of this
#include "table.h" //Another important data structure
#include "tensor.h" //The header file that started it all
#include "data_source.h" //Paths to the data the user is crunching
#include "console.h" //Contains user-console interactivity
#include "project.h" //Contains the information about tenstat_projects

//Standard Libraries:
#include <iostream> //For the Console User Interface

using namespace std;

//Main loop for console-based user-interface
void loop_console(tenstat_project& this_project) {
	//This loop is expected to be broken by the user typing 'exit'
	//Basic console variables
	bool go = true;
	int command;
	//Show project info
	print_project_info(this_project);
	cout << "Type help for a list of commands." << endl;
	while (go) {
		command = prompt_command(".o. I am awaiting your input.", vector<int>({ 1, 2, 5, 6 }));
		if (command == 5) { //'exit'
			go = false;
		}
		if (command == 6) { //'new'
			command = prompt_command("New what?", { 4, 8, 9, 15 });
			if (command == 15) { //'source'
				this_project.add_data_source(data_source(prompt_string("Enter file name or file path.")));
			}
			if (command == 4) { //'struct'
				command = prompt_command("What kind of struct?", { 7 });
				if (command == 7) { //'tensor'

				}

			}
			if (command == 9) { //'variable'
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
				command = prompt_command("What kind of variable?", { 10, 11 });
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
				if (this_project.get_web().get_clusters().size() > 0) {
					if (prompt_yesno("Add this variable to an existing cluster?")) {
						where = prompt_option("Which cluster should '" + varname + "' be added to?",
							this_project.get_web().get_cluster_names());
					}
				}
				else {
					cout << "(There exist no clusters to add this variable to)" << endl;
				}
				//Add the constructed variable to the web at the desired location.
				this_project.get_web().add_variable(addvar, where);
				print_update_message(varname + " added to this project's web");
			}
		}
		if (command == 2) { //'show'
			command = prompt_command("Show what?", { 8 });
			if (command == 8) { //'project'
				print_project_info(this_project);
			}
		}
	}
	cout << "Exiting Tenstat..." << endl;
	cout << "<.o/>>> Goodbye!" << endl;
}


//Main Function
int main(int argc, char* argv[]) {
	//Initial stuff
	startup_message();
	//Start new project
	cout << "Starting new Tenstat Project" << endl;
	tenstat_project this_project = tenstat_project("Untitled.ten");
	//Enter the console loop
	loop_console(this_project);
	//Exited the console loop

	//Done
	return 0;
}