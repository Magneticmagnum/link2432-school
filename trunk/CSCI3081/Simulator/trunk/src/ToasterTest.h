/** ToasterTest.h:
 * Unit tests for the Toaster class.
 */
#ifndef TOASTERTEST_H_
#define TOASTERTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Toaster.h"

class ToasterTest: public CxxTest::TestSuite {
public:

	/** test_toaster_constructor:
	 * Tests that the Toaster constructor creates a new instance, and
	 * that the instance starts out at 0 ticks and not running.
	 */
	void test_toaster_constructor() {
		Toaster toaster;
		TS_ASSERT_EQUALS(toaster.getTicks(), 0);
		TS_ASSERT_EQUALS(toaster.isRunning(), false);
		TS_ASSERT_EQUALS(toaster.stateChanged_, true);
		TS_ASSERT_EQUALS(toaster.getName(), "DefaultToaster");
	}

	/** test_toaster_tick:
	 * Tests that the Toaster tick method only increments its internal ticks or uses power when it's running.
	 */
	void test_toaster_tick() {
		Toaster toaster;
		// Check that toaster starts out at 0 ticks and not running.
		TS_ASSERT_EQUALS(toaster.getTicks(), 0);
		TS_ASSERT_EQUALS(toaster.isRunning(), false);
		TS_ASSERT_EQUALS(toaster.getPower(), 0);
		// Check that ticks are incremented correctly.
		toaster.tick();
		TS_ASSERT_EQUALS(toaster.getTicks(), 0);
		toaster.tick();
		TS_ASSERT_EQUALS(toaster.getTicks(), 0);
		// Check that toaster uses power for 2 minutes when activated
		toaster.activate("");
		toaster.tick();
		TS_ASSERT_EQUALS(toaster.getTicks(), 1);
		TS_ASSERT_EQUALS(toaster.getPower(), 0.9);
		toaster.tick();
		TS_ASSERT_EQUALS(toaster.getTicks(), 2);
		TS_ASSERT_EQUALS(toaster.getPower(), 0.9);
		toaster.tick();
		TS_ASSERT_EQUALS(toaster.getTicks(), 0);
		TS_ASSERT_EQUALS(toaster.getPower(), 0);
	}

	/** test_toaster_getEnergy:
	 * Tests that the Toaster energy is correctly updated.
	 */
	void test_toaster_getEnergy() {
		Toaster toaster;
		toaster.tick();
		toaster.activate("");
		toaster.tick();
		toaster.tick();
		toaster.tick();
		TS_ASSERT_DELTA(toaster.getEnergy(), (0.9*2)/60, 1e-6);
	}

	/** test_toaster_configure:
	 *  Tests that configuring the Toaster from a property table works correctly.
	 */
	void test_toaster_configure() {
		Toaster ts;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "TS"));
		props->insert(make_pair("powerOn", "1.0"));
		props->insert(make_pair("powerOff", "0.001"));
		ts.configure(props);
		TS_ASSERT(ts.getName() == "TS");
		TS_ASSERT_EQUALS(ts.powerOn_, 1.0);
		TS_ASSERT_EQUALS(ts.powerOff_, 0.001);
	}

	/** test_toaster_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_toaster_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Toaster toaster;
		cout << endl << "test_toaster_shift(): " << toaster;
	}

	/** test_toaster_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_toaster_cast() {
		Toaster toaster;
		cout << endl << "test_toaster_cast(): int = " << (int)toaster << ", double = " << (double)toaster << endl;
		// Compare (int)toaster with 0
		TS_ASSERT_EQUALS((int)toaster, 0);
	}
};

#endif /*TOASTERTEST_H_*/
