/** RandomIntTest.h:
 * Unit tests for RandomInt class.
 */
#ifndef RANDOMINTTEST_H_
#define RANDOMINTTEST_H_

#include <cxxtest/TestSuite.h>
#include "RandomInt.h"

class RandomIntTest: public CxxTest::TestSuite {
public:

	/** test_randomInt_constructor:
	 * Tests that the Person constructor creates a new instance, and
	 * that the instance initializes variables correctly.
	 */
	void test_randomInt_constructor() {
		RandomInt r(0,1);
		TS_ASSERT_EQUALS(r.lower_, 0);
		TS_ASSERT_EQUALS(r.upper_, 1);
	}

	/** test_randomInt_operator:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_randomInt_operator() {
		RandomInt r(0,1);
		int r1 = r();
		if (r1 == 0) {
			TS_ASSERT_EQUALS(r1, 0);
		}
		else {
			TS_ASSERT_EQUALS(r1, 1);
		}
	}

};
#endif /*RANDOMINTTEST_H_*/
