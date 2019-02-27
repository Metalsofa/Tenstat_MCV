/*The purpose of this program is to crunch numbers from many sources,
across multiple variables and variable types, especially with really big data,
in a way that any data analysis the user possibly wanted to do could be done
intuitively.*/
/*This .cpp file in particular pertains to a console-only version of this software
which will later be fitted to GUI.*/

//Custom Headers
#include "software_info.h" //For managing simple messages about the software itself
#include "project.h" //Contains the information about tenstat_projects
#include "wizard.h"
#include "console_interface.h"
#include "analysis.h"
#include "crunch.h"

//Standard Libraries:
#include <iostream> //For the Console User Interface

using namespace std;

//Main loop for console-based user-interface
void loop_console(tenstat_project& this_project) {
	//This loop is expected to be broken by the user typing 'exit'
	//Basic console variables
	bool go = true;
	int command;
	string input;
	//Show project info
	print_project_info(this_project);
	cout << "Type help for a list of commands." << endl;
	while (go) {
		command = prompt_command(".o. I am awaiting your input.", { chelp, cshow, cexit, cnew , cdebug, ccrunch });
		if (command == cdebug) { //'debug'
			command = prompt_command("Debug what?", { cdebug });
			if (command == ccrunch) { //'crunch'
				test_analysis(prompt_string("Enter an input file:"));
			}
		}
		if (command == ccrunch) { //'crunch'
			int method = prompt_command("Crunch how?", { cseginter });
			int object = prompt_command("Crunch what?", { ctable });
			if (object == ctable) {//'table'
				object = prompt_option("Which table?", this_project.table_names());
				if (method == cseginter) { //'seginter'
					unsigned int depth = prompt_int("Analyze at what differential depth?");
					vector<int> chosen = prompt_vec_int(
						"Enter line numbers for analysis, or press enter to use fill table.");
					seginter_crunch_table(object, chosen, depth, this_project);
				}
			}
		}
		if (command == cexit) { //'exit'
			go = false;
		}
		if (command == cnew) { //'new'
			command = prompt_command("New what?", { cstruct, cproject, cvariable, csource, cindex});
			if (command == csource) { //'source'
				//wiz_new_source(this_project);
				this_project.add_data_source(data_source(prompt_string("Enter file name or file path.")));
			}
			if (command == cstruct) { //'struct'
				command = prompt_command("What kind of struct?", { ctensor, ctable });
				if (command == ctensor) { //'tensor'
					//wiz_new_tensor(this_project);
				}
				if (command == ctable) {//'table'
					wiz_new_table(this_project);
				}
			}
			if (command == cvariable) { //'variable'
				wiz_new_variable(this_project);
			}
			if (command == cindex) { //'index'
				wiz_new_index(this_project);
			}
			if (command == cproject) { //'project'

			}
		}
		if (command == cshow) { //'show'
			command = prompt_command("Show what?", { cproject, cindices, ctables });
			if (command == cproject) { //'project'
				print_project_info(this_project);
			}
			if (command == cindices) {
				for (unsigned int i = 0; i < this_project.index_names().size(); i++) {
					print_object(this_project.get_index(i));
				}
			}
			if (command == ctables) {
				for (unsigned int i = 0; i < this_project.table_names().size(); i++) {
					print_object(this_project.get_float_table(i));
				}
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