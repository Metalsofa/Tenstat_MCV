/*This header file, as it appears here, should be no harder to use than a vector in programming.
Other heder files will worry about methods relating specifically to MCV Tensor Analysis*/
/*This header file defines the "tensor" object, which is an 
n-dimensional data tensor meant for multicategorical variable 
statistics.*/
#ifndef __tensor_h
#define __tensor_h

#include "variable.h"

#include <string>
#include <vector>



/*Recall that even at the lowest level of a Data Tensor,
there can be multiple values. For the purposes of 
Tenstat, the data type for all analytic tensors 
is <float*>, and they simply point where they need to
in order to properly track the variables
assigned to them.*/

//Used in the internal workings of a tensor
//Corrected so it no longer unnecessarily frees up already-dynamic memory.
template<class tensor_data_type>
class subtensor {
private:
	//How many dimensions this subtensor has; 0 = scalar
	unsigned int depth;
	//How long the tensor is in this particular dimension?
	//(length is 1 if  depth = 0, until time-wise is implemented)
	long unsigned int length;
	//Pointer to the next subtensor, defined only if depth > 0
	subtensor<tensor_data_type>* next;
	//Defined if and only if depth < 0;
	tensor_data_type* value;
public:
	//Default constructor, called in recursion
	subtensor() {
		/*This constructor exists so that the "new" function
		can use it to allocate space for member pointer
		"next"*/
	}
	//Returns a refrence to value (t) of this element, if (t) is specified
	tensor_data_type& operator() (int t){
		if (depth != 0) {
			std::cerr << "Warning: subtensor::operator() used on non-final subtensor" << std::endl;
		}
		//Corrects for indexing out of bounds
		return value[t % length];
	}
	//Returns a refrence to the first (possibly only) value of this element
	tensor_data_type& operator() (){
		if (depth != 0) {
			std::cerr << "Warning: subtensor::operator() used on non-final subtensor" << std::endl;
		}
		return value[0];
	}
	//Subscripting returns a refrence to the next subtensor, which itself will be indexible
	subtensor& operator[] (const int index) {
		if (depth != 0)
			return next[index];
		else {
			std::cerr << "Warning: subtensor of " << depth << " dimensions subscripted without children. Scalar returned." << std::endl;
			return value;
		}
	}
	//Returns the size of the tensor in this dimension
	long unsigned int size() {
		return length;
	}
	//Returns the memory address of this subtensor
	subtensor* location() {
		return this;
	}
	//Use the recursive instruction book method to intialize this object
	subtensor(unsigned long* handbook, unsigned int pages) {
		//Set this subtensor's depth by the number of pages in the instructions
		depth = pages;
		//If there are any pages left,
		if (pages > 0) {
			//Set the subtensor's size according to the last page of the instructions
			length = handbook[pages - 1];
			//Tear out the last page of the handbook
			pages--;
			//Allocate space for this object's array of child subtensors
			subtensor<tensor_data_type> temp[length];
            next = temp;
			//For each element in this object's array of child subtensors,
			for (unsigned long i = 0 ; i < length ; i++) {
				//Pass the child a copy of the truncated inctructions.
				next[i] = subtensor<tensor_data_type>(handbook, pages);
			}
		}	
		else { //But if there are no pages left:
			//This object's size defaults to 1, 
			length = 1;
			//So the container size is also 1,
            tensor_data_type temp [length];
			value = temp;
			//And the data it contains defaults to 0 as well
			value[0] = 0;
		}
	}
};

//(RIP categorical_dimension)

//This class is what we expect the user to be using
template<class tensor_entry_type>
class tensor {
private:
	//For the user's purposes, this tensor must have a name.
	std::string label;
	/*There should be variables assigned to each dimension of this tensor.
	"categories" includes intervaled quantitatives*/
    std::vector<variable*> categories;
    //Each cell wants a value for each of these
    std::vector<variable*> quantities;
	//Pointer to the location of the first dimension on the heap.
	subtensor<tensor_entry_type>* contents;
public:
	//Grants read-only access to the contents of this tensor
	subtensor<tensor_entry_type>& operator[] (unsigned long index) {
		return (*contents)[index];
	}
    //Accessors
    const std::vector<variable>& get_categories() {
        return categories;
    }

	//Array-based constructor, doesn't use variables. This constructor is a relic for testing purposes; don't use it unless for testing purposes.
	tensor(std::string tensor_name //Name this tensor, if you like. This argument is skippable.
	, unsigned int dimc //Pass the number of dimensions as an integer
	, unsigned long* dims) { //Pass the address of an array of integers, representing the sizes
		label = tensor_name;
		contents = new subtensor<tensor_entry_type>(dims, dimc);
	}
	//Vector-based constructor
	tensor(std::string name, std::vector<variable>* variables) { //Pass all of the category information through a std::vector
		label = name;
		categories = variables;
		unsigned int dimc = variables -> size();
		unsigned long dims[dimc];
		for(unsigned int i = 0; i < dimc; i++) {
			dims[i] = (*variables)[i].size();
		}
		contents = new subtensor<tensor_entry_type>(dims, dimc);
	}
	//Set in motion the self-destruct sequence of this tensor
	~tensor() {
		delete contents;
	}
};


#endif