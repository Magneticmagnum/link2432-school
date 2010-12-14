/**
 * RandomInt.h
 *
 *  Created on: Nov 13, 2010
 *      Author: Maya Pevzner
 */

#ifndef RANDOMINT_H_
#define RANDOMINT_H_

class RandomInt {
public:

	/** RandomInt:
	 * Default consructor
	 */
	RandomInt();

	/** RandomInt:
	 * Constructor. Arguments are range within which the returned random number should be.
	 */
	RandomInt(int lower, int upper);

	/**
	 * Function object. When you call it, you get an int between lower_ and upper_
	 */
	virtual int operator ()();

	friend class RandomIntTest;

protected:
	int lower_;
	int upper_;
	int value_;
};

#endif /* RANDOMINT_H_ */
