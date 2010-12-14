/** MicrowaveTest.h:
 * Unit tests for the Microwave class.
 */
#ifndef MICROWAVETEST_H_
#define MICROWAVETEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Microwave.h"

class MicrowaveTest: public CxxTest::TestSuite {
public:

	/** test_microwave_constructor:
	 * Tests that the Microwave constructor creates a new instance, and
	 * that the instance starts out at 0 ticks and not running.
	 */
	void test_microwave_constructor() {
		Microwave mw;
		TS_ASSERT_EQUALS(mw.getTicks(), 0);
		TS_ASSERT_EQUALS(mw.isRunning(), false);
		TS_ASSERT_EQUALS(mw.stateChanged_, true);
		TS_ASSERT_EQUALS(mw.getName(), "DefaultMicrowave");
	}

	/** test_microwave_tick:
	 * Tests that the Microwave tick method only increments its internal ticks or uses power when it's running.
	 * Tests that microwave uses 1500 W, or 1.5kW, per second for a period of time selected from a uniform distribution across 40-120 sec when activated.
	 */
	void test_microwave_tick() {
		// Test when run time is <60 seconds
		Microwave mw;
		// Check that microwave starts out at 0 ticks and not running.
		TS_ASSERT_EQUALS(mw.getTicks(), 0);
		TS_ASSERT_EQUALS(mw.isRunning(), false);
		TS_ASSERT_EQUALS(mw.getPower(), 0);
		// Check that ticks are incremented correctly.
		mw.tick();
		TS_ASSERT_EQUALS(mw.getTicks(), 0);
		mw.tick();
		TS_ASSERT_EQUALS(mw.getTicks(), 0);
		mw.activate("40");
		//mw.runTimeLeft_ = 40;
		TS_ASSERT_EQUALS(mw.runTimeLeft_, 40);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_EQUALS(mw.getTicks(), 0);
		TS_ASSERT_EQUALS(mw.getPower(), 0);

		// Test when run time is 60 seconds
		mw.activate("60");
		//mw.runTimeLeft_ = 60;
		TS_ASSERT_EQUALS(mw.runTimeLeft_, 60);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_EQUALS(mw.getTicks(), 0);
		TS_ASSERT_EQUALS(mw.getPower(), 0);

		// Test when run time is >60 seconds and <120 seconds
		mw.activate("70");
		//mw.runTimeLeft_ = 70;
		TS_ASSERT_EQUALS(mw.runTimeLeft_, 70);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_EQUALS(mw.getTicks(), 1);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_EQUALS(mw.getTicks(), 0);
		TS_ASSERT_EQUALS(mw.getPower(), 0);

		// Test when run time is 120 seconds
		mw.activate("120");
		//mw.runTimeLeft_ = 120;
		TS_ASSERT_EQUALS(mw.runTimeLeft_, 120);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_EQUALS(mw.getTicks(), 1);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_EQUALS(mw.getTicks(), 0);
		TS_ASSERT_EQUALS(mw.getPower(), 0);
}

	/** test_microwave_getEnergy:
	 * Tests that the Microwave energy is correctly updated.
	 */
	void test_microwave_getEnergy() {
		// Test when run time is <60 seconds
		Microwave mw;
		mw.activate("40");
		//mw.runTimeLeft_ = 40;
		TS_ASSERT_EQUALS(mw.runTimeLeft_, 40);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_DELTA(mw.getEnergy(), (1.5*40)/60, 1e-6);
		TS_ASSERT_EQUALS(mw.getPower(), 0);

		mw.activate("60");
		//mw.runTimeLeft_ = 60;
		TS_ASSERT_EQUALS(mw.runTimeLeft_, 60);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_DELTA(mw.getEnergy(), (1.5*60+1.5*40)/60, 1e-6);
		mw.tick();
		TS_ASSERT_DELTA(mw.getEnergy(), (1.5*60+1.5*40)/60, 1e-6);
		TS_ASSERT_EQUALS(mw.getPower(), 0);

		mw.activate("70");
		//mw.runTimeLeft_ = 70;
		TS_ASSERT_EQUALS(mw.runTimeLeft_, 70);
		mw.tick();
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		TS_ASSERT_DELTA(mw.getEnergy(), (2*1.5*60+1.5*40)/60, 1e-6);
		mw.tick();
		TS_ASSERT_DELTA(mw.getEnergy(), (2*1.5*60+1.5*10+1.5*40)/60, 1e-6);
		mw.tick();
		TS_ASSERT_DELTA(mw.getEnergy(), (2*1.5*60+1.5*10+1.5*40)/60, 1e-6);
		TS_ASSERT_EQUALS(mw.getPower(), 0);

		mw.activate("120");
		//mw.runTimeLeft_ = 120;
		TS_ASSERT_EQUALS(mw.runTimeLeft_, 120);
		TS_ASSERT_EQUALS(mw.getPower(), 1.5);
		mw.tick();
		TS_ASSERT_DELTA(mw.getEnergy(), (3*1.5*60+1.5*10+1.5*40)/60, 1e-6);
		mw.tick();
		TS_ASSERT_DELTA(mw.getEnergy(), (4*1.5*60+1.5*10+1.5*40)/60, 1e-6);
		mw.tick();
		TS_ASSERT_DELTA(mw.getEnergy(), (4*1.5*60+1.5*10+1.5*40)/60, 1e-6);
		TS_ASSERT_EQUALS(mw.getPower(), 0);
	}

	/** test_microwave_configure:
	 *  Tests that configuring the Microwave from a property table works correctly.
	 */
	void test_microwave_configure() {
		Microwave mw;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "MW"));
		props->insert(make_pair("powerOn", "1.1"));
		props->insert(make_pair("powerOff", "0.001"));
		mw.configure(props);
		TS_ASSERT(mw.getName() == "MW");
		TS_ASSERT_EQUALS(mw.powerOn_, 1.1);
		TS_ASSERT_EQUALS(mw.powerOff_, 0.001);
	}

	/** test_microwave_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_microwave_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Microwave mw;
		cout << endl << "test_microwave_shift(): " << mw;
	}

	/** test_microwave_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_microwave_cast() {
		Microwave mw;
		cout << endl << "test_microwave_cast(): int = " << (int) mw
				<< ", double = " << (double) mw << endl;
		// Compare (int)mw with 0
		TS_ASSERT_EQUALS((int)mw, 0);
	}

};

#endif /*MICROWAVETEST_H_*/
