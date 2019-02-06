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


//This list contains ALL commands, even sub-commands.
const std::vector<std::string> legal_commands = {
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
const int search(guts needle, std::vector<guts> haystack/*Const, so can't be passed by ref. Don't try.*/) {
    for (unsigned int owo = 0; owo < haystack.size(); owo++) {
        if (haystack[owo] == needle) {
            return owo;
        }
    }
    std::cerr << "Failed search: vector of size " << haystack.size() << " has no corresponding member" << std::endl;
    return -1;
}

//Validates a command, returning 0 if there is no such command as the searched one
unsigned int vcom (std::string command, std::vector<int> specify) {
    if (search(command, legal_commands) == -1 && search(search(command, legal_commands), specify))
        return 0;
    return search(command, legal_commands);
}

//For when the user now gets to use a wizard to edit or create a critical project element
void print_wizard_header (std::string title) {
    for (int i = 0; i < title.size() + 6; i++) {
        std::cout << '|';
    }
    std::cout << std::endl;
    std::cout << "|| " << title << " ||" << std::endl;
    for (int i = 0; i < title.size() + 6; i++) {
        std::cout << '|';
    }
    std::cout << std::endl;
}

//For any information display
void print_information_header (std::string title) {
    for (int i = 0; i < title.size() + 6; i++) {
        std::cout << '~';
    }
    std::cout << std::endl;
    std::cout << "   " << title << "   " << std::endl;
    for (int i = 0; i < title.size() + 6; i++) {
        std::cout << '~';
    }
    std::cout << std::endl;
}

//Subheader printing format
void print_subheader (std::string title) {
    std::cout << title << std::endl;
    for (unsigned int i = 0; i < title.size(); i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

void print_error(std::string error_message, int error_code) {
    if (error_code != -1) {
        std::cerr << "[" << error_code << "] ERROR: ";
        std::cerr << error_message << " [" << error_code << ']' << std::endl;
        exit (error_code);
    }
    std::cerr << "[!] ERROR: " << error_message << " [!]" << std::endl;
}

//Prompt the user for a string, then return it.
std::string prompt_string(std::string prompt) {
    std::cout << prompt << std::endl;
    std::cout << "<str>>>";
    std::string rets;
    std::cin >> std::ws;
    getline(std::cin, rets);
    return rets;
}

//Prompt the user for a bool, then return it.
bool prompt_yesno(std::string prompt) {
    std::cout << prompt << std::endl;
    std::cout << "<y/n>>>";
    std::string inp;
    std::cin >> inp;
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

void print_options(std::vector<std::string>& options) {
    std::cout << '{' << std::endl;
    for (unsigned int i = 0; i < options.size(); i++) {
        std::cout << '\t' << i << ": " << options[i] << std::endl;
    }
    std::cout << '}' << std::endl;
}

int prompt_int(std::string prompt){
    std::cout << prompt << std::endl;
    std::cout << "<int>>>";
    int reti;
    std::cin >> reti;
    return reti;
}

int prompt_option(std::string prompt, std::vector<std::string> options) {
    std::cout << '(' <<  prompt << ')';
    print_options(options);
    return prompt_int("") % options.size();
}

//Good for letting the user know a change was made to the file, for example
void print_update_message(std::string message) {
    std::cout << '[' << message << ']' << std::endl;
}

unsigned int prompt_command(std::string prompt, std::vector<int> specify) {
    std::cout << prompt << std::endl;
    std::cout << "<...>>>";
    std::string rets;
    std::cin.clear();
    std::cin >> rets;
    return vcom(rets, specify);
}

//Loads a .ten file into the software 

tenstat_project load_project(std::string filename) {
    
}

//Show's all available info about this project through the console
void print_project_info(tenstat_project& proj) {
    print_information_header("Project Information: " + proj.get_name());
    //Print information about variable web:
    print_subheader("Variables");
    //Pointers to the web and its clusters
    web* webb = &proj.get_web();
    std::vector<cluster>* clust = &proj.get_web().get_clusters();
    //Iterate through every cluster
    for (unsigned int i = 0; i < clust -> size(); i++) {
        //Intruduce the cluster by name and by size
        std::cout << "Cluster #" << i << ": " << (*clust)[i].get_name() << std::endl;
        std::cout << (*clust)[i].get_count() << " variables" << std::endl;
        //Iterate through every variable in this cluster
        for (unsigned int j = 0; j < (*clust)[i].get_count(); j++) {
            //Display the variable's name
            std::cout << j << "-" << webb -> get_contents()[j + (*clust)[i].get_start()].get_name();
            std::cout << " - " <<  webb -> get_contents()[j + (*clust)[i].get_start()].vartype_name();
            if (webb -> get_contents()[j + (*clust)[i].get_start()].get_vartype() == 3) {
                std::cout << " - " << webb -> get_contents()[j + (*clust)[i].get_start()].get_catc() <<
                " categories";
            }
            std::cout << std::endl;
        }
    }
    //Iterate through every variable in the web
    for (unsigned int i = 0; i < webb -> get_contents().size(); i++) {
        //But first we have to screen out the Clustered variables we've already displayed
        bool in_cluster = false;
        for (unsigned int j = 0; j < clust -> size(); j++) {
            if (i >= (*clust)[j].get_start() && i < (*clust)[j].get_count()) {
                in_cluster = true;
            }
        }
        //If it turns out this variable is not a member of a cluster,
        if (!in_cluster) {
            //Display it's name in the list
            std::cout << webb -> get_contents()[i].get_name() << " - " << 
            webb -> get_contents()[i].vartype_name() << " - " << 
            webb -> get_contents()[i].get_description() << std::endl;
        }
    }
    //Print information about data structures
    //NOTDONE
}



#endif