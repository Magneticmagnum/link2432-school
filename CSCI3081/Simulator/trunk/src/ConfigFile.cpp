/*
 * ConfigFile.cpp
 *
 *  Created on: Nov 25, 2010
 *      Author: Joe Houlton
 */

#include "ConfigFile.h"
#include "Stringutil.h"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <vector>

using namespace std;

ConfigFile::ConfigFile(std::string path) {
	LoggerPtr log(Logger::getLogger("Config"));
	cout << "Loading configuration file \'" << path << "\'..." << endl;
	LOG4CXX_INFO(log, "Loading configuration file \'" << path << "\'...");
	// TODO Auto-generated constructor stub
	ifstream ifs;
	ifs.open(path.c_str());
	string line;
	// use a holder for c_str
	char *cstr = (char*) malloc(MAX_LEN);
	while (getline(ifs, line, '\n')) {
		PropertyTable *pair = new PropertyTable;
		vector<string> *parsed = parseString(line, " ");
		std::vector<string>::iterator begin = parsed->begin();
		std::vector<string>::iterator end = parsed->end();
		while (begin != end) {
			strncpy(cstr, (*begin).c_str(), MAX_LEN);
			char *first, *second = NULL;
			first = strtok(cstr, "-");
			second = strtok(NULL, "-");
			if (first != NULL && second != NULL) {
				if (strcmp(first, "name") == 0) {
					// if it's the name, add it to the modelprops_ map
					modelprops_[second] = pair;
				}
				int seclen = strlen(second);
				// stupid line feeds...
				if (second[seclen] == '\r' || second[seclen] == '\n')
					second[seclen] = '\0';
				LOG4CXX_INFO(log, "Loading config property pair (" << string(
						first) << ", " << string(second) << ")");
				// add name-value entry to the property map (table)
				(*pair)[first] = string(second);
			}
			begin++;
		}
		delete parsed;
	}
	delete[] cstr;
	ifs.close();
	cout << "Finished loading configuration file." << endl;
	LOG4CXX_INFO(log, "Finished loading configuration file.");
}
map<string, PropertyTable*>::iterator ConfigFile::begin() {
	return modelprops_.begin();
}
map<string, PropertyTable*>::iterator ConfigFile::end() {
	return modelprops_.end();
}

ConfigFile::~ConfigFile() {
	// TODO Auto-generated destructor stub
	map<string, PropertyTable*>::iterator it = modelprops_.begin();
	map<string, PropertyTable*>::iterator end = modelprops_.end();
	while (it != end) {
		delete it->second;
		it++;
	}
}

PropertyTable* ConfigFile::getProps(std::string modelName) {
	return modelprops_[modelName];
}

std::map<std::string, PropertyTable *> &ConfigFile::getPropsMap() {
	return modelprops_;
}
