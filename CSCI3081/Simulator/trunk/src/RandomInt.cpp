/*
 * RandomInt.cpp
 *
 *  Created on: Nov 13, 2010
 *      Author: Maya Pevzner
 */
#include "RandomInt.h"
#include <boost/random.hpp>
#include <ctime>

RandomInt::RandomInt(int lower, int upper) {
	lower_ = lower;
	upper_ = upper;
}

RandomInt::RandomInt() {
	lower_ = 0;
	upper_ = 1;
}

int RandomInt::operator ()() {
	boost::mt19937 generator(time(NULL));
	generator.seed(static_cast<unsigned int>(std::time(0)));
	value_ = (int) (boost::uniform_real<double>(lower_, upper_)(generator));
	return value_;
}
