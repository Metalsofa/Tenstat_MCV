/*This header file contains the most basic statistical methods; they are
for the programmer's use, not the user's. More advanced methods get their
own header files, or go with the corresponding data structure.*/

#ifndef __basic_stat_h
#define __basic_stat_h
#include <cmath>
using namespace std;

//Returns the sum of a memory range of floats
float sum(float* data, int datacount) {
	float ret = 0;
	for (int i = 0; i < datacount; i++) {
		ret += data[i];
	}
	return ret;
}

//Returns the mean of a memory range of floats
float mean(float* data, int datacount) {
	return sum(data, datacount) / float(datacount);
}

//Returns the standard deviation of a memory range of floats
float stddev(float* data, int datacount) {
	float* num = new float[datacount];
	for (int i = 0; i < datacount; i++) {
		num[i] += pow((data[i] - mean(data, datacount)), 2);
	}
	float meen = mean(num, datacount - 1);
	delete[] num;
	return sqrt(meen);
}

//Retrns the correlation between two memory ranges of floats
float corr(float* datax, float* datay, int datacount) {
	float xbar = mean(datax, datacount);
	float ybar = mean(datay, datacount);
	float sx = stddev(datax, datacount);
	float sy = stddev(datay, datacount);
	float* seq = new float[datacount];
	for (int i = 0; i < datacount; i++) {
		seq[i] = (datax[i] - xbar) / sx * (datay[i] - ybar) / sy;
	}
	float num = mean(seq, datacount - 1);
	delete[] seq;
	return num;
}

//Returns the proportionality of two memory ranges of floats (y versus x)
float prop(float* x, float* y, int n) {
	float* num = new float[n];
	float* den = new float[n];
	for (int i = 0; 0 < n; i++) {
		num[i] = (x[i] - mean(x, n)) * (y[i] - mean(y, n));
		den[i] = (y[i] - mean(y, n));
	}
	float ret = sum(num, n) / sum(den, n);
	delete[] num;
	delete[] den;
	return ret;
}

#endif