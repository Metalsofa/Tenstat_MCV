//For our basic methods, we'll need

#ifndef __analysis_h
#define __analysis_h

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

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
	void errorthrow(string error_message) const {
		cerr << "ERROR:Analysis.h:datarange:" << error_message << endl;
		exit(1);
	}
	//Throw if the agrument is not less than datacount
	void validate_index(unsigned long i) const {
		if (i >= datacount) {
			errorthrow("Indexing out of bounds.");
		}
	}
	//Throw if the argument is unequal to datacount
	void validate_size(unsigned long s) const {
		if (s != datacount) {
			errorthrow("Size mismatch.");
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
		contents = new datum*[datacount];
	}
	//Vector-refrence constructor
	datarange(vector<datum>& input_vector) {
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
	//Table range constructor
	/*
	datarange(table<datum>& literature, unsigned long i1, unsigned long j1, unsigned long i2, unsigned long j2) {
		if (i1 > i2)
			swap(i1, i2);
		if (i1 > j2)
			swap(j1, j2);
		datacount = (i2 - i1 + 1) * (j2 - j1 + 1);
		contents = new datum*[datacount];
		unsigned long walk = 0;
		for (unsigned long i = i1; i <= i2; i++) {
			for (unsigned long j = j1; j <= j2; j++) {
				contents[walk] = &(literature(i, j));
				walk++;
			}
		}
	}
	*/
	//Copy constructor
	datarange(datarange<datum>& cop) {
		datacount = cop.size();
		contents = new datum*[datacount];
		for (unsigned int i = 0; i < datacount; i++) {
			contents[i] = cop(i);
		}
	}
	//Copy constructor
	datarange(const datarange<datum>& cop) {
		datacount = cop.size();
		contents = new datum*[datacount];
		for (unsigned int i = 0; i < datacount; i++) {
			contents[i] = cop(i);
		}
	}

	//OPERATORS
	//Returns a direct refrence to data point i
	datum& operator[] (unsigned long i) {
		validate_index(i);
		return *(contents[i]);
	}
	//Returns a direct refrence to the pointer to data point i
	datum*& operator() (unsigned long int i) {
		validate_index(i);
		return contents[i];
	}
	//Returns a direct refrence to the pointer to data point i
	datum*& operator() (unsigned long int i) const {
		validate_index(i);
		return contents[i];
	}

	//Assignment operator - const
	datarange& operator= (const datarange other) {
		if (datacount > 0) {
			delete[] contents;
		}
		datacount = other.size();
		contents = new datum*[other.size()];
		for (unsigned long i = 0; i < other.size(); i++) {
			contents[i] = other(i);
		}
		return *this;
	}
	//ACCESSORS
	//Set the value of a data point this structure points to
	void assign(unsigned long which, datum& what) {
		validate_index(which);
		contents[which] = &what;
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
float meanf(datarange<num>& data) {
	return float(sum(data)) / float(data.size());
}

//Returns the mean of a datarange
template<class num>
double mean(datarange<num>& data) {
	return double(sum(data)) / double(data.size());
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
		n[i] = powf(float(data[i]) - meanf(data), 2);
	}
	datarange<float> rsm(n, data.size());
	float meen = sum(rsm) / float(data.size() - 1);
	delete[] n;
	meen = sqrtf(meen);
	return meen;
}

//Returns the sample standard deviation of a datarange as a double
template<class num>
double stddev(datarange<num>& data) {
	double* n = new double[data.size()];
	for (int i = 0; i < data.size(); i++) {
		n[i] = pow(double(data[i]) - mean(data), 2.0);
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
		cerr << "ERROR:analysis.h:corrf:datarange size mismatch" << endl;
	}
	float xbar = meanf(datax);
	float ybar = meanf(datay);
	float sx = stddevf(datax);
	float sy = stddevf(datay);
	float* seq = new float[datax.size()];
	for (int i = 0; i < datax.size(); i++) {
		seq[i] = (datax[i] - xbar) / sx * (datay[i] - ybar) / sy;
	}
	datarange<float> lval(seq, datax.size());
	float ret = sum(lval) / float(datax.size() - 1);
	delete[] seq;
	return ret;
}

//Returns the correlation between two dataranges as a double
template<class num>
double corr(datarange<num>& datax, datarange<num>& datay) {
	if (datax.size() != datay.size()) {
		cerr << "ERROR:analysis.h:corr:datarange size mismatch" << endl;
	}
	double xbar = mean(datax);
	double ybar = mean(datay);
	double sx = stddev(datax);
	double sy = stddev(datay);
	double* seq = new double[datax.size()];
	for (int i = 0; i < datax.size(); i++) {
		seq[i] = (datax[i] - xbar) / sx * (datay[i] - ybar) / sy;
	}
	datarange<double> lval(seq, datax.size());
	double ret = sum(lval) / double(datax.size() - 1);
	delete[] seq;
	return ret;
}

//Returns the proportionality of two data ranges as a float
template<class num>
float propf(datarange<num>& datax, datarange<num>& datay) {
	if (datax.size() != datay.size()) {
		cerr << "ERROR:analysis.h:propf:datarange size mismatch" << endl;
	}
	float* nume = new float[datax.size()];
	float* deno = new float[datax.size()];
	float xbar = meanf(datax);
	float ybar = meanf(datay);
	for (int i = 0; i < datax.size(); i++) {
		nume[i] = ((float(datax[i]) - xbar) * (float(datay[i]) - ybar));
		deno[i] = powf((float(datax[i]) - xbar),2);
	}
	datarange<float> lvaln(nume, datax.size());
	datarange<float> lvald(deno, datay.size());
	float ret = sum(lvaln) / sum(lvald);
	delete[] nume;
	delete[] deno;
	return ret;
}

//Returns the proportionality of two data ranges as a double
template<class num>
double prop(datarange<num>& datax, datarange<num>& datay) {
	if (datax.size() != datay.size()) {
		cerr << "ERROR:analysis.h:prop:datarange size mismatch" << endl;
	}
	double* nume = new double[datax.size()];
	double* deno = new double[datax.size()];
	double xbar = mean(datax);
	double ybar = mean(datay);
	for (int i = 0; i < datax.size(); i++) {
		nume[i] = (double(datax[i]) - xbar * (double(datay[i]) - ybar));
		deno[i] = (double(datay[i]) - ybar);
	}
	datarange<double> lvaln(nume, datax.size());
	datarange<double> lvald(deno, datay.size());
	double ret = sum(lvaln) / sum(lvald);
	delete[] nume;
	delete[] deno;
	return ret;
}

//Returns the segment correlation about a particular x-value restricted to a given range as a float
template<class num>
float segcorrf(datarange<num>& datax, datarange<num>& datay, unsigned long center, unsigned long radius) {
	unsigned long start = center - radius;
	unsigned long end = center + radius;
	if (center < radius) {
		start = 0;
	}
	if (center + radius >= datax.size()) {
		end = datax.size() - 1;
	}
	datarange<num> crunchx(1 + end - start);
	datarange<num> crunchy(1 + end - start);
	//Inclusively copy in the appropriate values
	for (unsigned long i = start; i <= end; i++) {
		crunchx(i - start) = datax(i);
		crunchy(i - start) = datay(i);
	}
	return corrf(crunchx, crunchy);
}

//Returns the segment correlation about a particular x-value restricted to a given range as a double
template<class num>
double segcorr(datarange<num>& datax, datarange<num>& datay, unsigned long center, unsigned long radius) {
	unsigned long start = center - radius;
	unsigned long end = center + radius;
	if (center < radius) {
		start = 0;
	}
	if (center + radius >= datax.size()) {
		end = datax.size() - 1;
	}
	datarange<num> crunchx(1 + end - start);
	datarange<num> crunchy(1 + end - start);
	//Inclusively copy in the appropriate values
	for (unsigned long i = start; i <= end; i++) {
		crunchx(i - start) = datax(i);
		crunchy(i - start) = datay(i);
	}
	return corr(crunchx, crunchy);
}

//Returns the segment proportionality about a particular x-value restricted to a given range as a float
template<class num>
float segpropf(datarange<num>& datax, datarange<num>& datay, unsigned long center, unsigned long radius) {
	unsigned long start = center - radius;
	unsigned long end = center + radius;
	if (center < radius) {
		start = 0;
	}
	if (center + radius >= datax.size()) {
		end = datax.size() - 1;
	}
	datarange<num> crunchx(1 + end - start);
	datarange<num> crunchy(1 + end - start);
	//Inclusively copy in the appropriate values
	for (unsigned long i = start; i <= end; i++) {
		crunchx(i - start) = datax(i);
		crunchy(i - start) = datay(i);
	}
	return propf(crunchx, crunchy);
}

//Returns the segment proportionality about a particular x-value restricted to a given range as a float
template<class num>
double segprop(datarange<num>& datax, datarange<num>& datay, unsigned long center, unsigned long radius) {
	unsigned long start = center - radius;
	unsigned long end = center + radius;
	if (center < radius) {
		start = 0;
	}
	if (center + radius >= datax.size()) {
		end = datax.size() - 1;
	}
	datarange<num> crunchx(1 + end - start);
	datarange<num> crunchy(1 + end - start);
	//Inclusively copy in the appropriate values
	for (unsigned long i = start; i <= end; i++) {
		crunchx(i - start) = datax(i);
		crunchy(i - start) = datay(i);
	}
	return prop(crunchx, crunchy);
}

//Returns a vector of correlations (as floats) of each point's segmented correlation for a given radius
template<class num>
vector<float> segcorrf_range(datarange<num>& datax, datarange<num>& datay, unsigned long radius) {
	vector<float> retv;
	for (unsigned long i = 0; i < datax.size(); i++) {
		retv.push_back(segcorrf(datax, datay, i, radius));
	}
	return retv;
}

//Returns a vector of correlations (as doubles) of each point's segmented correlation for a given radius
template<class num>
vector<double> segcorr_range(datarange<num>& datax, datarange<num>& datay, unsigned long radius) {
	vector<float> retv;
	for (unsigned long i = 0; i < datax.size(); i++) {
		retv.push_back(segcorr(datax, datay, i, radius));
	}
	retv;
}


//Returns a vector of proportionality (as floats) of each point's segmented proportionalty for a given radius
template<class num>
vector<float> segpropf_range(datarange<num>& datax, datarange<num>& datay, unsigned long radius) {
	vector<float> retv;
	for (unsigned long i = 0; i < datax.size(); i++) {
		retv.push_back(segpropf(datax, datay, i, radius));
	}
	retv;
}

//Returns a vector of proportionality (as doubles) of each point's segmented proportionalty for a given radius
template<class num>
vector<double> segprop_range(datarange<num>& datax, datarange<num>& datay, unsigned long radius) {
	vector<double> retv;
	for (unsigned long i = 0; i < datax.size(); i++) {
		retv.push_back(segprop(datax, datay, i, radius));
	}
	retv;
}

//Returns the segmented correlation product as a float for two data sets at a given radius
template<class num>
float segcorrf_product(datarange<num>& datax, datarange<num>& datay, unsigned long radius) {
	vector<float> lo = segcorrf_range(datax, datay, radius);
	datarange<float> lval(lo);
	return fabsf(prod(lval));
}

//Returns the segmented correlation product as a double for two data sets at a given radius
template<class num>
double segcorr_product(datarange<num>& datax, datarange<num>& datay, unsigned long radius) {
	vector<float> lo = segcorr_range(datax, datay, radius);
	datarange<double> lval(lo);
	return fabs(prod(lval));
}

//Returns the optimum radius (first) and optimized product as a double (second) for segmentation between two data sets
template<class num>
pair<unsigned long, double> opt_seg_rad(datarange<num>& datax, datarange<num>& datay) {
	pair<unsigned long, double> opt(0, 0);
	//We may want to at some point consider a minimum value for this r. Maybe not though!
	for (unsigned long r = 1; r < datax.size(); r++) {
		if (segcorr_product(datax, datay, r) > opt.second) {
			opt.second = segcorr_product(datax, datay, r);
			opt.first = r;
		}
	}
	return opt;
}

//Returns the optimum radius (first) and optimized product as a float (second) for segmentation between two data sets
template<class num>
pair<unsigned long, float> opt_segf_rad(datarange<num>& datax, datarange<num>& datay) {
	pair<unsigned long, float> opt(0, 0);
	//We may want to at some point consider a minimum value for this r. Maybe not though!
	for (unsigned long r = 1; r < datax.size(); r++) {
		if (segcorrf_product(datax, datay, r) > opt.second) {
			opt.second = segcorrf_product(datax, datay, r);
			opt.first = r;
		}
	}
	return opt;
}

//Returns a vector of a-values (as floats) that relate two data sets at the optimum segmentation radius
template<class num>
vector<float> opt_segpropf_range(datarange<num>& datax, datarange<num>& datay) {
	vector<float> retv;
	unsigned long optrad = opt_segf_rad(datax, datay).first;
	for (unsigned long i = 0; i < datax.size(); i++) {
		retv.push_back(segpropf(datax, datay, i, optrad));
	}
	return retv;
}

//Returns a vector of a-values (as doubles) that relate two data sets at the optimum segmentation radius
template<class num>
vector<double> opt_segprop_range(datarange<num>& datax, datarange<num>& datay) {
	vector<double> retv;
	unsigned long optrad = opt_seg_rad(datax, datay);
	for (unsigned long i = 0; i < datax.size(); i++) {
		retv.push_back(segprop(datax, datay, i, optrad));
	}
	return retv;
}

//Structures the output data of relating two data sets //Looks like I'm settling on floats
class seginter_results {
private:
	//Keep track of the data
	datarange<float> xdata;
	datarange<float> ydata;
	//Labels
	string datax_label;
	string datay_label;
	//Correlation coefficient
	float correlation;
	//Proportionality coefficient
	float proportionality;
	//Optimum Segmentation Radius
	unsigned long opt_rad;
	//Optimized Segmented Correlation Coefficient Product
	float opt_seg_corr_prod;
	//Optimized Segemented Proportionality Values
	vector<float> opt_seg_prop_range;
public:
	seginter_results(datarange<float>& datax, datarange<float>& datay, string& labelx, string& labely) {
		xdata = datax;
		ydata = datay;
		datax_label = labelx;
		datay_label = labely;
		correlation = corrf(datax, datay);
		proportionality = propf(datax, datay);
		opt_rad = opt_segf_rad(datax, datay).first;
		opt_seg_corr_prod = opt_segf_rad(datax, datay).second;
		opt_seg_prop_range = opt_segpropf_range(datax, datay);
	}
	//Output the information found from this statistical analysis
	void print(ostream& dest) {
		dest << "Tenseg Output for " << datay_label << " versus " << datax_label << endl;
		dest << "Correlation:		r = " << correlation << endl;
		dest << "Proportionality:	a = " << proportionality << endl;
		dest << "Opt. Seg. Radius:		" << opt_rad << endl;
		dest << "Opt. Seg. Corr. Prod:  " << opt_seg_corr_prod << endl;
		dest << "Opt. Seg. Prop. Range: [";
		for (unsigned int i = 0; i < opt_seg_prop_range.size() - 1; i++) {
			dest << opt_seg_prop_range[i] << ", ";
		}
		dest << opt_seg_prop_range[opt_seg_prop_range.size() - 1] << ']' << endl;
	}
	//Access the segmentation results
	vector<float>& get_results() {
		return opt_seg_prop_range;
	}
};

//Multivariate Segmented Differential Statistical Analysis output, makes use of recursion
template<class num>
vector<seginter_results> multi_seg_diff_stat(datarange<datarange<num>>& big_data, datarange<string> varnames, unsigned int depth) {
	vector<seginter_results> retv;
	cout << "Crunching... " << big_data.size() << " variables..." << endl;
	cout << endl;
	for (unsigned long i = 0; i < big_data.size(); i++) {
		cout << i + 1<< " / " << big_data.size() << endl;
		for (unsigned long j = 0; j < big_data.size(); j++) {
			cout << '\t' << j + 1 << " / " << big_data.size() << '\t';
			recursive_crunch(big_data[i], big_data[j], depth, retv, varnames[i], varnames[j], big_data, varnames);
			cout << endl;
		}
	}
	return retv;
}

//Perform Seg Diff. Analysis on a set of dataranges and push the results into their appropriate repository
template<class num>
void seginter_crunch(datarange<datarange<num>>& big_data, datarange<string> varnames, unsigned int depth, vector<seginter_results>& repository) {
	vector<seginter_results> results = multi_seg_diff_stat(big_data, varnames, depth);
	for (unsigned int i = 0; i < results.size(); i++) {
		repository.push_back(results[i]);
	}
}

template<class num> 
num range_min(datarange<num>& data) {
	num ret = data[0];
	for (unsigned long i = 0; i < data.size(); i++) {
		if (data[i] < ret)
			ret = data[i];
	}
	return ret;
}

template<class num>
num range_max(datarange<num>& data) {
	num ret = data[0];
	for (unsigned long i = 0; i < data.size(); i++) {
		if (data[i] > ret)
			ret = data[i];
	}
	return ret;
}

//Get a whole bunch of relations crunched out recursively and iteratively
void recursive_crunch(datarange<float>& datax, datarange<float>& datay,
	unsigned int depth, vector<seginter_results>& output_repository,
	string& labelx, string& labely,
	datarange<datarange<float>>& big_data, datarange<string>& varnames) {
	unsigned int last = output_repository.size();
	output_repository.push_back(seginter_results(datax, datay, labelx, labely));
	cout << depth;
	for (unsigned int i = 0; i < depth; i++) {
		cout << '-';
	}
	if (depth == 0) {
		return;
	}
	string dname = "d(" + labely + ")/d(" + labelx + ")";
	datarange<float> diff(output_repository[last].get_results());
	for (unsigned int i = 0; i < big_data.size(); i++) {
		recursive_crunch(big_data[i], diff, depth - 1, output_repository, varnames[i], dname, big_data, varnames);
	}
	cout << ' ';
}



//Also for testing purposes
vector<vector<float>> datatable_fromfile(string filename) {
	ifstream textfile;
	textfile.open(filename);
	vector<string> contents;
	string line;
	if (textfile.is_open()) {
		cout << "Opened '" << filename << "' successfully." << endl;
		//This while loop pushes each line into an element
		while (getline(textfile, line)) {
			contents.emplace_back(line);
		}
		textfile.close();
	}
	else {
		cerr << "Failed to open '" << filename << "'" << endl;
		return vector<vector<float>>(0);
	}
	vector<vector<float>> retv;
	stringstream reader;
	for (unsigned int i = 0; i < contents.size(); i++) {
		retv.push_back(vector<float>(0));
		reader = stringstream(contents[i]);
		float temp;
		while (reader >> temp) {
			retv[i].push_back(temp);
		}
	}
	return retv;
}



//Testing purposes only
void test_analysis(string filename) {
	cout << "Now testing Multivariate Segmented Differential Statistical Intercorrelaion and Interproportionality Analysis." << endl;
	if (filename.size() == 0) {
		cout << "Using default file: sample_input.txt" << endl;
		filename = "sample_input.txt";
	}
	vector<vector<float>> numbers = datatable_fromfile(filename);
	vector<datarange<float>> watchnums;
	vector<string> names;
	for (unsigned int i = 0; i < numbers.size(); i++) {
		datarange<float> temp(numbers[i]);
		watchnums.push_back(temp);
		names.push_back("");
		names.back() += char(65 + i);
	}
	datarange<datarange<float>> beeg_data(watchnums);
	datarange<string> namen(names);
	unsigned int deep = 2;
	ofstream write;
	write.open("output.txt");
	vector<seginter_results> testbatch = multi_seg_diff_stat(beeg_data, namen, deep);
	for (unsigned int i = 0; i < testbatch.size(); i++) {
		testbatch[i].print(write);
		write << endl;
	}
	write.close();
	cout << "Done." << endl;
}



#endif