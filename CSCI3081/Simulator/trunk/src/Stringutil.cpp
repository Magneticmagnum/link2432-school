/*
 * Stringutil.cpp
 *
 *  Created on: Dec 14, 2010
 *      Author: Joe
 */
#include "Stringutil.h"
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

std::vector<std::string>* parseString(std::string &str, const char* sep) {
	char *cstr = (char*) malloc(MAX_LEN);
	strncpy(cstr, str.c_str(), MAX_LEN);
	std::vector<std::string> *ret = new std::vector<std::string>();
	char *cur = strtok(cstr, sep);
	ret->push_back(std::string(cur));
	while ((cur = strtok(NULL, sep)) != NULL) {
		ret->push_back(std::string(cur));
	}
	delete[] cstr;
	return ret;
}
