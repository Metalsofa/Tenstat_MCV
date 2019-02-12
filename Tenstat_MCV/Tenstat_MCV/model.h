/*This header file defines */
#pragma once
#ifndef __model_h
#define __model_h

#include "variable.h"
#include <string>

//Qualifies and quantifies the relationship between two dataranges
class relationship_info {
private:
	//Describes the relationship, for example, linear, sqrt, inverse, logarithmic, polynomial
	std::string name;
};



#endif
