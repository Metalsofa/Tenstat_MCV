/*The data management in this software is in terms of variables.
They don't hold any values; their role is as the paramaters through
which all data management in this software is done.*/
#ifndef __variable_h
#define __variable_h

#include "table.h"
#include "index.h"

#include <string>
#include <vector>



//The essential object type of understanding the data we're working with. More of a parameter, really
class variable {
private:
    //Name of this particular variable
    std::string label;
	//The description for this variable
	std::string description;
    //Color
    float r, g, b;
    /*Integer representing type
        Quick refrence:
        0: Quantitative Continuous  "Continuous Quantitative Variable"
        1: Quantitative Discrete    "Discrete by nature"
        2: Quantitative Intervaled  "Discrete/Categorical by imposition of intervals"
        3: Categorical              "Unordered Categorical Variable"*/
    int vartype;
    //For CATEGORICALS
    //Number of categories
    unsigned long catc;
    //Address of any user-defined index this variable is attached to;
    index* label_index;
    //Address of any user-defined index of descriptions
    index* description_ind;
    //Data sources attached to this variable (tables)
    unsigned int attached_ic; //Integer Table Count
    table<int>* attached_itable_locations;
    unsigned int attached_fc; //Float Table Count
    table<float>* attached_ftable_locations;
public:
    //Default construcot
    variable() {
        r = 1; g = 1; b = 1;
        label = "Unnamed_variable";
        description = "[No description]";
        vartype = 0;
    }
    //Constructor for building a non-categorical variable from scratch
    variable(std::string& name, std::string& _description, int type) {
        label = name;
        description = _description;
        if (type == 3) {
            std::cerr << "Warning:Variable '" << name << 
            "' failed construction:cannot use this constructor for categoricals" << std::endl;
            type = 1;
            std::cerr << "Variable " << name << " defaulted to type (1)" << std::endl;
        }
        vartype = type;
    }
    //Constructor for building a categorical variable from scratch, without indexes
    variable(std::string& name, std::string& _description, unsigned long category_count) {
        label = name;
        description = _description;
        catc = category_count;
    }
    //Constructor for building a categorical variable from scratch, ft. index for category labels
    variable(std::string& name, std::string& _description, unsigned long category_count, index* labdex) {
        label = name;
        description = _description;
        catc = category_count;
        label_index = labdex;
    }
    //Constructor for building a categorical variable from scratch, ft. index for category labels
    variable(std::string& name, std::string& _description, unsigned long category_count, index* labdex, index* descdex) {
        label = name;
        description = _description;
        catc = category_count;
        label_index = labdex;
        description_ind = descdex;
        vartype = 3;
    }
    const unsigned long get_catc() {
        return catc;
    }
    //Returns the user-fristd::endly readable name for this variable
    const std::string vartype_name() {
        switch (vartype) {
        case 0:
            return "Quantitative Continuous";
        case 1:
            return "Quantitative Discrete";
        case 2:
            return "Quantitative Intervaled";
        case 3:
            return "Categorical";
        }
        return "ERR_INVALID_VAR_TYPE";
    }
    //Accessor for this variable's label
    std::string& get_name() {
        return label;
    }
    //Accessor for this variable's description
    std::string& get_description() {
        return description;
    }
    //Accessor for this variable's 'type'
    const int get_vartype() {
        return vartype;
    }
    //For CATEGORICALS:
	//Elaborate "know-it-all" constructor
	variable(std::string _label, std::string _description, unsigned long count, bool intervaled) {
		label = _label;
		description = _description;
		catc = count;
        if (intervaled)
            vartype = 2;
        else
            vartype = 3;
	}
    //Accesssor for this variable's size
    const unsigned long size() {
        if (vartype == 2 || vartype == 3) {
            return catc;
        }
        else {
            std::cerr << "Warning; Variable '" << label << "' has no '.size()', it is of type " 
            << vartype_name() << std::endl;
        }
    }
    /*A variable can produce a data range as an output value,
    and can often do so in more than one way. For example,
    a user might want a range of predicted values, or a range
    as given by a particular data source*/
};

//For keeping track of the way clusters of variables are grouped
class cluster {
private:
    std::string label;
    unsigned int start;
    unsigned int count;
public:
    std::string& get_name() {
        return label;
    }
    unsigned int& get_start() {
        return start;
    }
    unsigned int& get_count() {
        return count;
    }
};

/*The variable web is a data structure meant to keep track
of the variables a user*/
class web {
private:
    //The order of these variables does somewhat matter; think of the clusters
    std::vector<variable> contents;
    //Classic graph theory stuff. Directional.
    //Each matrix corresponds to a data source.
    std::vector<std::vector<std::vector<bool>>> adjacency_matrices;
    //The class for clusters explains what's going on with them
    std::vector<cluster> clusters;
public:
    /*Adding a variable to the web -1 if position doesn't
     matter. Cluster index if it does.*/
    void add_variable(variable what, long where) {
        if (where == -1) {
            contents.push_back(what);
        }
        else {
            //Put the variable where it goes
            contents.emplace(contents.begin() + 
            //That is to say, the end of the appropriate cluster
            clusters[where].get_start() + 
            //Increment this cluster's size
            clusters[where].get_count()++, 
            what);
            //For each cluster after that, increment position
            for (unsigned int i = where + 1; i < clusters.size(); i++) {
                clusters[i].get_start()++;
            }
        }
    }
    //Get a nice vector of all the cluster names
    const std::vector<std::string> get_cluster_names() {
        std::vector<std::string> retv;
        for (unsigned int i = 0; i < clusters.size(); i++) {
            retv.emplace_back(clusters[i].get_name());
        }
        return retv;
    }
    //Default constructor-san
    web() {
        //There's nothing to do here!!!
        //Um.....
        //Hope this doesn't make the program commit die :o
    }

    std::vector<variable>& get_contents() {
        return contents;
    }

    //Returns a refrence to the vector of clusters
    std::vector<cluster>& get_clusters() {
        return clusters;
    }
};

#endif