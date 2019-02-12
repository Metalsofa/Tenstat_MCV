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
		command = prompt_command(".o. I am awaiting your input.", { 1, 2, 5, 6 , 16, 17 });
		if (command == 16) { //'debug'
			command = prompt_command("Debug what?", { 17 });
			if (command == 17) { //'crunch'
				test_analysis(prompt_string("Enter an input file:"));
			}
		}
		if (command == 17) { //'crunch

		}
		if (command == 5) { //'exit'
			go = false;
		}
		if (command == 6) { //'new'
			command = prompt_command("New what?", { 4, 8, 9, 15 });
			if (command == 15) { //'source'
				//wiz_new_source(this_project);
				this_project.add_data_source(data_source(prompt_string("Enter file name or file path.")));
			}
			if (command == 4) { //'struct'
				command = prompt_command("What kind of struct?", { 7 });
				if (command == 7) { //'tensor'
					//wiz_new_tensor(this_project);
				}

			}
			if (command == 9) { //'variable'
				wiz_new_variable(this_project);
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