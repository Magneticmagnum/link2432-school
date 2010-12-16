/** StoveTest.h:
 * Unit tests for the Stove class.
 */
#ifndef STOVETEST_H_
#define STOVETEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Stove.h"

class StoveTest: public CxxTest::TestSuite {
public:

	/** test_stove_constructor:
	 * Tests that the Stove constructor creates a new instance, and
	 * that the instance starts out at 0 ticks and running.
	 */
	void test_stove_constructor() {
		Stove stove;
		TS_ASSERT_EQUALS(stove.isRunning(), false);
		TS_ASSERT_EQUALS(stove.getName(), "DefaultStove");
	}

	/** test_stove_tick:
	 * Tests that the Stove tick method correctly updates the power depending on if it's running or not,
	 * taking into account the number of burners that are on and the heat level of each burner.
	 */
	void test_stove_tick() {
		Stove stove;
		// Check that stove starts out not running.
		TS_ASSERT_EQUALS(stove.isRunning(), false);
		TS_ASSERT_EQUALS(stove.getPower(), 0);
		stove.tick();
		TS_ASSERT_EQUALS(stove.getPower(), 0);
		// Check that turning stove on updates power usage & running state.
		stove.activate("3,5,2,5,0,5,1,5");
		stove.tick();
		TS_ASSERT_EQUALS(stove.isRunning(), true);
		TS_ASSERT_EQUALS(stove.getPower(), (1.4 + 1.4*0.5 + 1.4*0.25));
		stove.tick();
		TS_ASSERT_EQUALS(stove.getPower(), (1.4 + 1.4*0.5 + 1.4*0.25));
		// Check that turning stove off updates power usage & running state
		stove.activate("0,0,0,0,0,0,0,0");
		stove.tick();
		TS_ASSERT_EQUALS(stove.isRunning(), false);
		TS_ASSERT_EQUALS(stove.getPower(), 0);
		// test that a notification is set and cleared
		stove.activate("3,1,0,0,0,0,0,0");
		TS_ASSERT_EQUALS(stove.notifyStateChanged_, true);
		stove.tick();
		TS_ASSERT_EQUALS(stove.notifyStateChanged_, false);
	}

	/** test_stove_getEnergy:
	 * Tests that the Stove energy is correctly updated.
	 */
	void test_stove_getEnergy() {
		Stove stove;
		stove.tick();
		stove.activate("3,5,2,5,0,5,1,5");
		stove.tick();
		stove.tick();
		stove.tick();
		stove.activate("0,0,0,0,0,0,0,0");
		stove.tick();
		TS_ASSERT_DELTA(stove.getEnergy(), (3*(1.4 + 1.4*0.5 + 1.4*0.25))/60, 1e-6);
	}

	/** test_stove_configure:
	 *  Tests that configuring the Stove from a property table works correctly.
	 */
	void test_stove_configure() {
		Stove st;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "Stove"));
		props->insert(make_pair("powerOn", "1.6"));
		props->insert(make_pair("powerOff", "0.001"));
		st.configure(props);
		TS_ASSERT(st.getName() == "Stove");
		TS_ASSERT_EQUALS(st.powerOn_, 1.6);
		TS_ASSERT_EQUALS(st.powerOff_, 0.001);
	}

	/** test_stove_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_stove_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Stove stove;
		cout << endl << "test_stove_shift(): " << stove;
	}

	/** test_stove_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_stove_cast() {
		Stove stove;
		cout << endl << "test_stove_cast(): int = " << (int) stove
				<< ", double = " << (double) stove << endl;
		// Compare (int)stove with 0
		TS_ASSERT_EQUALS((int)stove, 0);
	}
};

#endif /* STOVETEST_H_ */
