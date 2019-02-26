/*data crunching logistics, not mathematical methods, go here. Essentially it keeps
the main loop crunch-free*/
#pragma once
#ifndef __crunch_h
#define __crunch_h

#include "table.h"
#include "analysis.h"
#include "project.h"

#include <vector>

using namespace std;


//Perform Segmented Differential Intercorrelation Analysis on a table
void seginter_crunch_table(unsigned int table_ID, vector<int>& which_lines, unsigned int differential_depth, tenstat_project& proj) {
	if (which_lines.size() == 0) {
		for (unsigned int i = 0; i < proj.get_float_table(table_ID).height(); i++) {
			which_lines.push_back(i);
		}
	}
	vector<datarange<float>> subject;
	for (unsigned int i = 0; i < which_lines.size(); i++) {
		subject.push_back(datarange<float>(proj.get_float_table(table_ID).width()));
		for (unsigned int j = 0; j < proj.get_float_table(table_ID).width(); j++) {
			subject.back()(j) = &(proj.get_float_table(table_ID)(which_lines[i], j));
		}
	}
	datarange<datarange<float>> researchrange(subject);
	seginter_crunch(researchrange, proj.get_float_table(table_ID).get_row_labels().get_datarange(), differential_depth, proj.get_seginter_repository());
}

#endif