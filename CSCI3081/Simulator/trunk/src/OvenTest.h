/** OvenTest.h:
 * Unit tests for the Oven class.
 */
#ifndef OVENTEST_H_
#define OVENTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Oven.h"

class OvenTest: public CxxTest::TestSuite {
public:

	/** test_oven_constructor:
	 * Tests that the Oven constructor creates a new instance, and
	 * that the instance starts out at 0 ticks and not running.
	 */
	void test_oven_constructor() {
		Oven oven;
		TS_ASSERT_EQUALS(oven.getTicks(), 0);
		TS_ASSERT_EQUALS(oven.isRunning(), false);
		TS_ASSERT_EQUALS(oven.stateChanged_, true);
		TS_ASSERT_EQUALS(oven.getName(), "DefaultOven");
	}

	/** test_oven_tick:
	 * Tests that the Oven tick method only increments its internal ticks or uses power when it's running.
	 */
	void test_oven_tick() {
		Oven oven;
		// Check that oven starts out at 0 ticks and not running.
		TS_ASSERT_EQUALS(oven.getTicks(), 0);
		TS_ASSERT_EQUALS(oven.isRunning(), false);
		TS_ASSERT_EQUALS(oven.getPower(), 0);
		// Check that ticks are incremented correctly.
		oven.tick();
		TS_ASSERT_EQUALS(oven.getTicks(), 0);
		oven.tick();
		TS_ASSERT_EQUALS(oven.getTicks(), 0);
		// Check that oven uses 7.5 kW power for 10 minutes when activated, and then 2.5 kW power for another 30 minutes
		oven.activate(30);
		oven.tick();
		TS_ASSERT_EQUALS(oven.getTicks(), 1);
		TS_ASSERT_EQUALS(oven.getPower(), 7.5);
		oven.tick();
		TS_ASSERT_EQUALS(oven.getTicks(), 2);
		TS_ASSERT_EQUALS(oven.getPower(), 7.5);
		for (int i = 1; i <= 8; i++) {
			oven.tick();
		}
		TS_ASSERT_EQUALS(oven.getTicks(), 10);
		TS_ASSERT_EQUALS(oven.getPower(), 2.5);
		for (int i = 1; i <= 30; i++) {
			oven.tick();
		}
		TS_ASSERT_EQUALS(oven.getTicks(), 40);
		TS_ASSERT_EQUALS(oven.getPower(), 0);
		oven.tick();
		TS_ASSERT_EQUALS(oven.getTicks(), 0);
		TS_ASSERT_EQUALS(oven.getPower(), 0);
	}

	/** test_oven_getEnergy:
	 * Tests that the Oven energy is correctly updated.
	 */
	void test_oven_getEnergy() {
		Oven oven;
		oven.tick();
		oven.activate(30);
		for (int i = 0; i < 40; i++) { // run oven's 40 minute cycle
			oven.tick();
		}
		TS_ASSERT_DELTA(oven.getEnergy(), ((10*7.5 + 30*2.5)/60), 1e-6);
	}

	/** test_oven_configure:
	 *  Tests that configuring the Oven from a property table works correctly.
	 */
	void test_oven_configure() {
		Oven ov;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "Oven"));
		props->insert(make_pair("powerPreheat", "8.5"));
		props->insert(make_pair("powerOn", "3.5"));
		props->insert(make_pair("powerOff", "0.001"));
		ov.configure(props);
		TS_ASSERT(ov.getName() == "Oven");
		TS_ASSERT_EQUALS(ov.powerPreheat_, 8.5);
		TS_ASSERT_EQUALS(ov.powerOn_, 3.5);
		TS_ASSERT_EQUALS(ov.powerOff_, 0.001);
	}

	/** test_oven_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_oven_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Oven oven;
		cout << endl << "test_oven_shift(): " << oven;
	}

	/** test_oven_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_oven_cast() {
		Oven oven;
		cout << endl << "test_oven_cast(): int = " << (int)oven << ", double = " << (double)oven << endl;
		// Compare (int)oven with 0
		TS_ASSERT_EQUALS((int)oven, 0);
		TS_ASSERT_EQUALS((double)oven, 0.0);
	}
};

#endif /*OVENTEST_H_*/
