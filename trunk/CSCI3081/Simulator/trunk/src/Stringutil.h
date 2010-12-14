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

	// Parses a string into a vector with seperator sep
	// "this-is-a-test" "-"
	// [this, is, a, test]

std::vector<std::string>* parseString2List(std::string &str,
			const char* sep);
#endif /* STRINGUTIL_H_ */
