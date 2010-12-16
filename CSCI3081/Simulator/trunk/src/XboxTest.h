/** XboxTest.h:
 * Unit test for the Xbox class.
 */
#ifndef XBOXTEST_H_
#define XBOXTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Xbox.h"

class XboxTest: public CxxTest::TestSuite {
public:

	/** test_xbox_constructor:
	 * Tests that the Xbox constructor creates a new instance, and
	 * that the instance starts out at 0 ticks and running.
	 */
	void test_xbox_constructor() {
		Xbox xbox;
		TS_ASSERT_EQUALS(xbox.isRunning(), false);
		TS_ASSERT_EQUALS(xbox.getName(), "DefaultXbox");
	}

	/** test_xbox_tick:
	 * Tests that the Xbox tick method correctly updates the power depending on if it's running or not.
	 */
	void test_xbox_tick() {
		Xbox xbox;
		// Check that TV starts out not running.
		TS_ASSERT_EQUALS(xbox.isRunning(), false);
		TS_ASSERT_EQUALS(xbox.getPower(), 0.002);
		xbox.tick();
		TS_ASSERT_EQUALS(xbox.getPower(), 0.002);
		// Check that turning Xbox on updates power usage & running state
		xbox.activate("on");
		xbox.tick();
		TS_ASSERT_EQUALS(xbox.isRunning(), true);
		TS_ASSERT_EQUALS(xbox.getPower(), 0.165);
		xbox.tick();
		TS_ASSERT_EQUALS(xbox.getPower(), 0.165);
		// Check that turning Xbox off updates power usage & running state
		xbox.activate("off");
		xbox.tick();
		TS_ASSERT_EQUALS(xbox.isRunning(), false);
		TS_ASSERT_EQUALS(xbox.getPower(), 0.002);
	}

	/** test_xbox_getEnergy:
	 * Tests that the Xbox energy is correctly updated.
	 */
	void test_xbox_getEnergy() {
		Xbox xbox;
		xbox.tick();
		xbox.activate("on");
		xbox.tick();
		xbox.activate("off");
		xbox.tick();
		TS_ASSERT_DELTA(xbox.getEnergy(), (0.002*2 + 0.165)/60, 1e-6);
	}

	/** test_xbox_configure:
	 *  Tests that configuring the Xbox from a property table works correctly.
	 */
	void test_xbox_configure() {
		Xbox xb;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "XBox"));
		props->insert(make_pair("powerOn", "0.2"));
		props->insert(make_pair("powerOff", "0.003"));
		xb.configure(props);
		TS_ASSERT(xb.getName() == "XBox");
		TS_ASSERT_EQUALS(xb.powerOn_, 0.2);
		TS_ASSERT_EQUALS(xb.powerOff_, 0.003);
	}

	/** test_xbox_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_xbox_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Xbox xbox;
		cout << endl << "test_xbox_shift(): "  << xbox;
	}

	/** test_xbox_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_xbox_cast() {
		Xbox xbox;
		xbox.tick();
		cout << endl << "test_xbox_cast(): int = " << (int)xbox << ", double = " << (double)xbox << endl;
		// Compare with 0.01/60
		TS_ASSERT_EQUALS((int)xbox, 0);
		TS_ASSERT_EQUALS((double)xbox, 0.002/60);
	}
};

#endif /*XBOXTEST_H_*/
