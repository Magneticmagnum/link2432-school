/*
 * ConfigFile.cpp
 *
 *  Created on: Nov 25, 2010
 *      Author: Joe Houlton
 */

#include "ConfigFile.h"
#include <fstream>
#include <iostream>
#include <string>
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
	while (getline(ifs, line, '\n')) {
		// read file in
		vector<string> parsed;
		// parse2vector by space char
		int ind = 0;
		PropertyTable *pair = new PropertyTable;
		while (ind >= 0) {
			// presume first field is name (requirement)
			int firstSpace = line.find(" ");
			int firstHyphen = line.find("-");
			string first = line.substr(0, firstHyphen);
			string second = line.substr(firstHyphen + 1, firstSpace
					- firstHyphen - 1);
			cout << "Loading config property pair (" << first << ", " << second << ")" << endl;
			LOG4CXX_INFO(log, "Loading config property pair (" << first << ", " << second << ")");
			// ex. name Microwave
			pair->insert(make_pair(first, second));
			if (first == "name") {
				// insert pair into map with name field
				modelprops_.insert(make_pair(second, pair));

			}
			// slice out parsed part
			line = line.substr(firstSpace + 1, line.size() - firstSpace - 1);
			ind = firstSpace;
		}
	}
	cout << "Finished loading configuration file." << endl;
	LOG4CXX_INFO(log, "Finished loading configuration file.");
}
map<string, PropertyTable* >::iterator ConfigFile::begin() {
	return modelprops_.begin();
}
map<string, PropertyTable* >::iterator ConfigFile::end() {
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

PropertyTable* ConfigFile::getProps(string model) {
	return modelprops_[model];
}

std::map<std::string, PropertyTable *> ConfigFile::getPropsMap() {
	return modelprops_;
}
