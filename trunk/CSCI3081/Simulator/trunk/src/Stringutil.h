/*
 * Stringutil.h
 *
 *  Created on: Dec 13, 2010
 *      Author: Joe Houlton
 */

#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>

// Parses a string into a vector with seperator sep
// "this-is-a-test" "-"
// [this, is, a, test]
std::vector<std::string>* parseString2List(std::string &str, const char* sep) {
	char* cstr = (char*) malloc(strlen(str.c_str()));
	strcpy(cstr, str.c_str());
	std::vector<std::string> *ret = new std::vector<std::string> ();
	char *cur = strtok(cstr, sep);
	ret->push_back(std::string(cur));
	while ((cur = strtok(NULL, sep)) != NULL) {
		ret->push_back(std::string(cur));
	}
	delete[] cstr;
	return ret;
}

#endif /* STRINGUTIL_H_ */
