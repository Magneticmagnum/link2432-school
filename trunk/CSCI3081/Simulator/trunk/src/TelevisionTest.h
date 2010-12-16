/** TelevisionTest.h:
 * Unit test for the Television class.
 */
#ifndef TELEVISIONTEST_H_
#define TELEVISIONTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Television.h"

class TelevisionTest: public CxxTest::TestSuite {
public:

	/** test_television_constructor:
	 * Tests that the Television constructor creates a new instance, and
	 * that the instance starts out at 0 ticks and running.
	 */
	void test_television_constructor() {
		Television tv;
		TS_ASSERT_EQUALS(tv.isRunning(), false);
		TS_ASSERT_EQUALS(tv.getPower(), 0.01);
		TS_ASSERT_EQUALS(tv.getName(), "DefaultTelevision");
	}

	/** test_television_tick:
	 * Tests that the Television tick method correctly updates the power depending on if it's running or not.
	 */
	void test_television_tick() {
		Television tv;
		// Check that TV starts out not running.
		TS_ASSERT_EQUALS(tv.isRunning(), false);
		TS_ASSERT_EQUALS(tv.getPower(), 0.01);
		tv.tick();
		TS_ASSERT_EQUALS(tv.getPower(), 0.01);
		// Check that turning TV on updates power usage & running state
		tv.activate("on");
		tv.tick();
		TS_ASSERT_EQUALS(tv.isRunning(), true);
		TS_ASSERT_EQUALS(tv.getPower(), 0.18);
		tv.tick();
		TS_ASSERT_EQUALS(tv.getPower(), 0.18);
		// Check that turning TV off updates power usage & running state
		tv.activate("off");
		tv.tick();
		TS_ASSERT_EQUALS(tv.isRunning(), false);
		TS_ASSERT_EQUALS(tv.getPower(), 0.01);
	}

	/** test_television_getEnergy:
	 * Tests that the Television energy is correctly updated.
	 */
	void test_television_getEnergy() {
		Television tv;
		tv.tick();
		tv.activate("on");
		tv.tick();
		tv.activate("off");
		tv.tick();
		TS_ASSERT_DELTA(tv.getEnergy(), (0.01*2 + 0.18)/60, 1e-6);
	}

	/** test_television_configure:
	 *  Tests that configuring the Television from a property table works correctly.
	 */
	void test_television_configure() {
		Television tv;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "TV"));
		props->insert(make_pair("powerOn", "0.2"));
		props->insert(make_pair("powerOff", "0.001"));
		tv.configure(props);
		TS_ASSERT(tv.getName() == "TV");
		TS_ASSERT_EQUALS(tv.powerOn_, 0.2);
		TS_ASSERT_EQUALS(tv.powerOff_, 0.001);
	}

	/** test_television_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_television_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Television tv;
		cout << endl << "test_television_shift(): "  << tv;
	}

	/** test_television_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_television_cast() {
		Television tv;
		tv.tick();
		cout << endl << "test_television_cast(): int = " << (int)tv << ", double = " << (double)tv << endl;
		// Compare with 0.01/60
		TS_ASSERT_EQUALS((int)tv, 0);
		TS_ASSERT_EQUALS((double)tv, 0.01/60);
	}
};

#endif /*TELEVISIONTEST_H_*/
