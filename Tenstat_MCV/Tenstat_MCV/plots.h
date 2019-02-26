/*Contains all of the methods that relate to creating, plotting, and customizing
various plot/graph objects. */
#ifndef __plots_h
#define __plots_h

#include "analysis.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Basic 2-D Plot
class plot {
private:
	//Internal representation for this thing's name
	string label;
	//Data
	datarange<float>* xdata;
	datarange<float>* ydata;
	unsigned int height;
	unsigned int width;
	unsigned long leftbound;
	unsigned long rightbound;
	unsigned long topbound;
	unsigned long bottombound;
	//Some custom settings
	//Whether or not to increase the size of precision of this plot by integrating numerals in appearance
	bool intcap;
	//Otherwise, what character to represent data points with
	char pointchar;
	//Character to draw stems with (use space if no stems)
	char stemchar;
	//Label the axes?
	bool marklab;
	//Label periodicity for x-axis
	int labperx;
	//First label for x-axis
	int labfirstx;
	//Label periodicity for y-axis
	int labpery;
	//First label for y-axis
	int labfirsty;
public:
	//Simple data-only constructor for two ranges, automatic formatting
	plot(string& title, datarange<float>& datax, datarange<float>& datay) {
		label = title;
		xdata = &datax;
		ydata = &datay;
		width = 80;
		height = 20;
		leftbound = long(ceil(range_min(datax)));
		rightbound = long(ceil(range_max(datax)));
		topbound = long(ceil(range_max(datay)));
		bottombound = long(ceil(range_min(datay)));
		intcap = true;
		pointchar = '*';
		stemchar = '|';
		marklab = true;
		labperx = 5;
		labpery = 5;
		labfirstx = 0; //pow(10, ceil(log10(leftbound)) - 1);
		labfirsty = 0; //pow(10, ceil(log10(bottombound)) - 1);
	}

	//Print this plot to an O-stream
	void print(ostream& dest) {
		dest << label << endl;
		vector<string> field;
		for (unsigned int i = 0; i < height; i++) {
			string line;
			for (unsigned int j = 0; j < width; j++) {
				line.push_back(' ');
			}
			field.push_back(line);
		}
		for (unsigned long v = 0; v < xdata -> size(); v++) {
			field[
				int(roundf((ydata -> operator[](v) - bottombound) / (topbound - bottombound) * height))
			][
				int(roundf((xdata -> operator[](v) - leftbound) / (rightbound - leftbound) * width))
			]
				= (pointchar);
		}
		for (unsigned int y = height - 1; y >= 0; y--) {
			if (marklab) {
				if ((y - labfirsty) % labpery == 0) {
					dest << (y / height) * (topbound - bottombound) + bottombound;
				}
				dest << '\t' << '|';
			}
			for (unsigned int x = 0; x < width; x++) {
				dest << field[y][x];
			}
			dest << endl;
		}
		dest << '\t' << '|';
		for (unsigned int x = 0; x < width; x++) {
			dest << '-';
		}
		for (unsigned int x = 0; x < width; x++) {
			if ((x - labfirstx) % labperx == 0) {
				dest << (x / height) * (rightbound - leftbound) + leftbound;
			}
		}
		dest << endl;
	}
};




#endif
