#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_
/*
 * Stringutil.h
 *
 *  Created on: Dec 13, 2010
 *      Author: Joe Houlton
 */

#include <string>
#include <vector>

// Parses a string into a vector with seperator sep
// "this-is-a-test" "-"
// [this, is, a, test]
#define MAX_LEN 1023

// Need this because it's a template return?  Not sure really
extern std::vector<std::string>* parseString(std::string &str, const char* sep);

#endif /* STRINGUTIL_H_ */
