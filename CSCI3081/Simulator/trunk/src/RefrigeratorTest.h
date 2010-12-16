/** RefrigeratorTest.h:
 * Unit tests for the Refrigerator class.
 */
#ifndef REFRIGERATORTEST_H_
#define REFRIGERATORTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Refrigerator.h"

class RefrigeratorTest: public CxxTest::TestSuite {
public:

	/** test_refrigerator_constructor:
	 * Tests that the Refrigerator constructor creates a new instance.
	 */
	void test_refrigerator_constructor() {
		Refrigerator fridge;
		TS_ASSERT_EQUALS(fridge.getCookingState(), false);
		TS_ASSERT_EQUALS(fridge.powerOn_, 0.3);
		TS_ASSERT_EQUALS(fridge.onTime_, 3);
		TS_ASSERT_EQUALS(fridge.dutyCycleCooking_, 0.5);
		TS_ASSERT_EQUALS(fridge.dutyCycleNotCooking_, 0.3);
		TS_ASSERT_EQUALS(fridge.getName(), "DefaultRefrigerator");
	}

	/** test_refrigerator_tick:
	 * Tests that the Refrigerator tick method increments the ticks,
	 * that the method turns the Refrigerator on and off as needed,
	 * and that the tick cycle accounts for whether or not a Person is cooking.
	 */
	void test_refrigerator_tick() {
		Refrigerator fridge;
		// Check that fridge starts out correctly.
		TS_ASSERT_EQUALS(fridge.getCookingState(), false);
		bool initRunning = fridge.isRunning();
		int initTicks = fridge.getTicks();
		TS_ASSERT_EQUALS(fridge.isRunning(), initRunning);
		TS_ASSERT_EQUALS(fridge.getTicks(), initTicks);
		// Check that ticks are incremented correctly.
		fridge.tick();
		if (initTicks < 9) {
			TS_ASSERT_EQUALS(fridge.getTicks(), initTicks+1);
		} else {
			TS_ASSERT_EQUALS(fridge.getTicks(), 0);
		}
		// Check that cycle updates correctly when a Person is cooking.
		fridge.activate("on");
		TS_ASSERT_EQUALS(fridge.getCookingState(), true);
		TS_ASSERT_EQUALS(fridge.getTicks(), 0);
		TS_ASSERT_EQUALS(fridge.isRunning(), true);
		TS_ASSERT_EQUALS(fridge.getPower(), 0.3);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 1);
		TS_ASSERT_EQUALS(fridge.isRunning(), true);
		TS_ASSERT_EQUALS(fridge.getPower(), 0.3);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 2);
		TS_ASSERT_EQUALS(fridge.isRunning(), true);
		TS_ASSERT_EQUALS(fridge.getPower(), 0.3);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 3);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 4);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 5);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 0);
		TS_ASSERT_EQUALS(fridge.isRunning(), true);
		TS_ASSERT_EQUALS(fridge.getPower(), 0.3);
		// Check that cycle updates correctly when nobody is cooking.
		fridge.activate("off");
		TS_ASSERT_EQUALS(fridge.getCookingState(), false);
		TS_ASSERT_EQUALS(fridge.getTicks(), 0);
		TS_ASSERT_EQUALS(fridge.isRunning(), true);
		TS_ASSERT_EQUALS(fridge.getPower(), 0.3);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 1);
		TS_ASSERT_EQUALS(fridge.isRunning(), true);
		TS_ASSERT_EQUALS(fridge.getPower(), 0.3);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 2);
		TS_ASSERT_EQUALS(fridge.isRunning(), true);
		TS_ASSERT_EQUALS(fridge.getPower(), 0.3);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 3);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 4);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 5);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 6);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 7);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 8);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 9);
		TS_ASSERT_EQUALS(fridge.isRunning(), false);
		TS_ASSERT_EQUALS(fridge.getPower(), 0);
		fridge.tick();
		TS_ASSERT_EQUALS(fridge.getTicks(), 0);
		TS_ASSERT_EQUALS(fridge.isRunning(), true);
		TS_ASSERT_EQUALS(fridge.getPower(), 0.3);
	}

	/** test_refrigerator_getEnergy:
	 * Tests that the WaterHeater energy is correctly updated.
	 */
	void test_refrigerator_getEnergy() {
		Refrigerator fridge;
		fridge.activate("on");
		fridge.tick(); // on
		fridge.tick();
		fridge.tick();
		fridge.tick(); // off
		fridge.tick();
		fridge.tick();
		fridge.activate("off");
		fridge.tick(); // on
		fridge.tick();
		fridge.tick();
		fridge.tick(); // off
		fridge.tick();
		fridge.tick();
		fridge.tick();
		fridge.tick();
		fridge.tick();
		fridge.tick();
		TS_ASSERT_DELTA(fridge.getEnergy(), (6*0.3)/60, 1e-6);
	}

	/** test_refrigerator_configure:
	 *  Tests that configuring the Refrigerator from a property table works correctly.
	 */
	void test_refrigerator_configure() {
		Refrigerator r;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "Fridge"));
		props->insert(make_pair("powerOn", "3.0"));
		props->insert(make_pair("powerOff", "0.001"));
		props->insert(make_pair("onTime", "4"));
		props->insert(make_pair("dutyCycleCooking", "0.6"));
		props->insert(make_pair("dutyCycleNotCooking", "0.4"));
		r.configure(props);
		TS_ASSERT(r.getName() == "Fridge");
		TS_ASSERT(r.powerOn_ == 3.0);
		TS_ASSERT(r.powerOff_ == 0.001);
		TS_ASSERT(r.onTime_ == 4);
		TS_ASSERT(r.dutyCycleCooking_ == 0.6);
		TS_ASSERT(r.dutyCycleNotCooking_ == 0.4);
	}

	/** test_refrigerator_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_refrigerator_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Refrigerator fridge;
		cout << endl << "test_refrigerator_shift(): " << fridge;
	}

	/** test_refrigerator_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_refrigerator_cast() {
		Refrigerator fridge;
		cout << endl << "test_refrigerator_cast(): int = " << (int) fridge
				<< ", double = " << (double) fridge << endl;
		// Compare (int)fridge with 0
		TS_ASSERT_EQUALS((int)fridge, 0);
	}

};

#endif /*REFRIGERATORTEST_H_*/
