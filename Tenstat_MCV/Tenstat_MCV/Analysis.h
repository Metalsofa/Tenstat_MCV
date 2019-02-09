#pragma once
//More advanced statistical computing techniques go in here

//For our basic methods, we'll need

#ifndef __analysis_h
#define __analysis_h

#include <string>
#include <iostream>
#include <vector>
#include <cmath>

/*Lightweight pointer structure, remembers a bunch of memory addresses and
makes accesssing their values easy. Picture an array of non-array pointers.
Also has good member-functions for analysis.
Be sure not to let something this referr to something that then goes out of range*/
template<class datum>
class datarange {
private:
	//Array of pointers to the concerned numbers
	datum** contents;
	//Number of entries it's looking at
	unsigned long datacount;
	//Throw with error message
	void errorthrow(std::string error_message) {
		std::cerr << "ERROR:Analysis.h:datarange:" << error_message << std::endl;
		exit(1);
	}
	//Throw if the agrument is not less than datacount
	void validate_index(unsigned long i) {
		if (i >= datacount) {
			errorthrow("Indexing out of bounds.");
		}
	}
	//Throw if the argument is unequal to datacount
	void validate_size(unsigned logn s) {
		if (s != datacount) {
			errorthrow("Size mismatch.")
		}
	}
public:
	//CONSTRUCTORS
	//Default constructor
	datarange() {
		datacount = 0;
	}
	datarange(unsigned long length) {
		datacount = length;
		contents = new datum*[data];
	}
	//Vector-refrence constructor
	datarange(std::vector<datum>& input_vector) {
		datacount = input_vector.size();
		contents = new datum*[datacount];
		for (unsigned long i = 0; i < datacount; i++) {
			contents[i] = &input_vector[i];
		}
	}
	//Array constructor
	datarange(datum* input_array, unsigned long array_size) {
		datacount = array_size;
		contents = new datum*[datacount];
		for (unsigned long i = 0; i < datacount; i++) {
			assign(i, input_array[i]);
		}
	}
	//OPERATORS
	//Returns a direct refrence to data point i
	datum& operator[] (unsigned long i) {
		return *(contents[i]);
	}
	//Returns a direct refrence to the pointer to data point i
	datum*& operator() (unsigned long i) {
		validate_index(i);
		return contents[i];
	}
	//Assignment operator
	datarange operator= (datarange other) {
		delete[] contents;
		contents = new datum*[other.size()]
		for (unsigned long i = 0; i < other.size(); i++) {
			contents[i] = other(i);
		}
	}
	//ACCESSORS
	//Set the value of a data point this structure points to
	void assign(unsigned long which, datum& what) {
		validate_index(which);
		datum[which] = &what;
	}
	//Returns the size of this datarange
	const unsigned long size() const {
		return datacount;
	}
	/*Copy the contents of this datarange into an array by passing the array's location
	Be sure the allocated size of the array matches this data range's size, or else
	you'll get a segmentation fault*/
	void copy_contents(datum* location) {
		for (unsigned long i = 0; i < datacount; i++) {
			location[i] = *(contents[i]);
		}
	}
	//DESTRUCTOR
	~datarange() {
		//This array is the only thing we should need to delete
		delete[] contents;
	}
};

//--------------------ANALYSIS METHODS-----------------------------

//Returns the sum of a datarange
template<class num>
num sum(datarange<num>& data) {
	float ret = 0;
	for (unsigned long i = 0; i < data.size(); i++) {
		ret += data[i];
	}
	return ret;
}

//Returns the mean of a datarange
template<class num>
num meanf(datarange<num>& data) {
	return float(sum(datarange)) / float(data.size());
}

//Returns the mean of a datarange
template<class num>
num mean(datarange<num>& data) {
	return double(sum(datarange)) / double(data.size());
}

//Returns the cumulative product of a datarange
template<class num>
num prod(datarange<num>& data) {
	num ret = 1;
	for (unsigned long i = 0; i < data.size(); i++) {
		ret *= data[i];
	}
	return ret;
}

//Returns the sample standard deviation of a datarange as a float
template<class num>
float stddevf(datarange<num>& data) {
	float* n = new float[data.size()];
	for (int i = 0; i < data.size(); i++) {
		n[i] += powf(float(data[i]) - float(mean(data, datacount)), 2.0f);
	}
	datarange<float> rsm(n, data.size());
	float meen = sum(rsm)/float(data.size() - 1);
	delete[] n;
	return sqrtf(meen);
}

//Returns the sample standard deviation of a datarange as a double
template<class num>
double stddev(datarange<num>& data) {
	double* n = new double[data.size()];
	for (int i = 0; i < data.size(); i++) {
		n[i] += pow(double(data[i]) - double(mean(data, datacount)), 2.0);
	}
	datarange<double> rsm(n, data.size());
	double meen = sum(rsm) / double(data.size() - 1);
	delete[] n;
	return sqrt(meen);
}

//Returns the correlation between two dataranges as a float
template<class num>
float corrf(datarange<num>& datax, datarange<num>& datay) {
	if (datax.size() != datay.size()) {
		std::cerr << "ERROR:analysis.h:corrf:datarange size mismatch" << std::endl;
	}
	float xbar = meanf(datax);
	float ybar = meanf(datay);
	float sx = stddevf(datax);
	float sy = stddevf(datay);
	float* seq = new float[datax.size()];
	for (int i = 0; i < data.size(); i++) {
		seq[i] = (datax[i] - xbar) / sx * (datay[i] - ybar) / sy;
	}
	float ret = sum(datarange<float>(seq,datax.size())) / float(datax.size() - 1);
	delete[] seq;
	return ret;
}

//Returns the correlation between two dataranges as a double
template<class num>
float corr(datarange<num>& datax, datarange<num>& datay) {
	if (datax.size() != datay.size()) {
		std::cerr << "ERROR:analysis.h:corr:datarange size mismatch" << std::endl;
	}
	float xbar = mean(datax);
	float ybar = mean(datay);
	float sx = stddev(datax);
	float sy = stddev(datay);
	float* seq = new double[datax.size()];
	for (int i = 0; i < data.size(); i++) {
		seq[i] = (datax[i] - xbar) / sx * (datay[i] - ybar) / sy;
	}
	float ret = sum(datarange<double>(seq, datax.size())) / double(datax.size() - 1);
	delete[] seq;
	return ret;
}

//Returns the proportionality of two data ranges as a float
template<class num>
float propf(datarange<num>& datax, datarange<num>& datay) {
	if (datax.size() != datay.size()) {
		std::cerr << "ERROR:analysis.h:propf:datarange size mismatch" << std::endl;
	}
	float* nume = new float[datax.size()];
	float* deno = new float[datax.size()];
	float xbar = meanf(datax);
	float ybar = meanf(datay);
	for (int i = 0; 0 < datax.size(); i++) {
		nume[i] = (float(datax[i]) - xbar * (float(datay[i]) - ybar));
		deno[i] = (float(y[i]) - ybar);
	}
	float ret = sum(datarange<float>(nume, datax.size())) / sum(datarange<float>(deno, datay.size()));
	delete[] nume;
	delete[] deno;
	return ret;
}

//Returns the proportionality of two data ranges as a double
template<class num>
double propf(datarange<num>& datax, datarange<num>& datay) {
	if (datax.size() != datay.size()) {
		std::cerr << "ERROR:analysis.h:prop:datarange size mismatch" << std::endl;
	}
	double* nume = new double[datax.size()];
	double* deno = new double[datax.size()];
	double xbar = mean(datax);
	double ybar = mean(datay);
	for (int i = 0; 0 < datax.size(); i++) {
		nume[i] = (double(datax[i]) - xbar * (double(datay[i]) - ybar));
		deno[i] = (double(y[i]) - ybar);
	}
	double ret = sum(datarange<double>(nume, datax.size())) / sum(datarange<double>(deno, datay.size()));
	delete[] nume;
	delete[] deno;
	return ret;
}

//Returns the segment correlation about a particular x-value restricted to a given range
template<class num>
float segcorrf(datarange<num> datax, datarange<num> datay, unsigned long center, unsigned long radius) {
	
}

#endif