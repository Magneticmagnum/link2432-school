/** DishwasherTest.h:
 * Unit tests for the Dishwasher class.
 */

#ifndef DISHWASHERTEST_H_
#define DISHWASHERTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Dishwasher.h"
#include "Clock.h"
#include "WaterHeater.h"

using namespace std;

class DishwasherTest: public CxxTest::TestSuite {
public:

	/** test_dishwasher_constructor:
	 * Tests that the Dishwasher constructor creates a new instance, and
	 * that the instance starts out at 0 ticks and not running.
	 */
	void test_dishwasher_constructor() {
		Dishwasher dw;
		TS_ASSERT_EQUALS(dw.getTicks(), 0);
		TS_ASSERT_EQUALS(dw.isRunning(), false);
		TS_ASSERT_EQUALS(dw.getPower(), 0.004);
		TS_ASSERT_EQUALS(dw.getName(), "DefaultDishwasher");
		TS_ASSERT_EQUALS(dw.stateChanged_, true);
		TS_ASSERT_EQUALS(dw.getData(), 0);
	}

	/** test_dishwasher_tick:
	 * Tests that the Dishwasher tick method increments the ticks.
	 * Check that the Dishwasher uses 4 W of power continuously when not running.
	 * Check that when activated, it uses 2 kW continuously during
	 * its 45-minute cycle, and it draws 22 L of hot water over the
	 * first 20 minutes of that cycle.
	 */
	void test_dishwasher_tick() {
		Scheduler sched;
		Dishwasher dw;
		WaterHeater wh;
		House *ptr = House::getInstance();
		Clock *c = new Clock();
		ptr->putModel(std::string("Clock"), c);
		ptr->putModel("WaterHeater", &wh);
		Clock* clk = (Clock*) (ptr->getModel("Clock"));
		clk->registerScheduler(&sched);
		WaterHeater *wh1 = dynamic_cast<WaterHeater *>(ptr->getModel("WaterHeater"));

		// Check that dishwasher starts out at 0 ticks and off.
		TS_ASSERT_EQUALS(dw.getTicks(), 0);
		TS_ASSERT_EQUALS(dw.isRunning(), false);
		TS_ASSERT_EQUALS(dw.getPower(), 0.004);

		// Check that ticks are not incremented when Dishwasher is not running,
		// and that power is still updated when not running.
		dw.tick();
		TS_ASSERT_EQUALS(dw.getTicks(), 0);
		TS_ASSERT_EQUALS(dw.getPower(), 0.004);
		dw.tick();
		TS_ASSERT_EQUALS(dw.getTicks(), 0);
		TS_ASSERT_EQUALS(dw.getPower(), 0.004);

		TS_ASSERT_EQUALS(wh1->getTemperature(), 50);

		// Check that activating Dishwasher adjusts state & power usage.
		dw.activate("");

		// Check that mock WaterHeater temperature starts at 50
		TS_ASSERT_EQUALS(wh1->getTemperature(), 50);

		dw.tick();
		TS_ASSERT_EQUALS(dw.isRunning(), true);
		TS_ASSERT_EQUALS(dw.getPower(), 2);

		// Check that 22 liters of hot water is drawn from WaterHeater over first 20 minutes of Dishwasher cycle,
		// by checking that 22/20 liters of hot water is drawn from WaterHeater per minute:
		// After 1 Dishwasher tick, WaterHeater temperature should = ((50.0 * (200.0 - 1.1)) + (7 * 1.1))/ 200.0 = 49.7635.
		TS_ASSERT_DELTA(wh1->getTemperature(), 49.7635, 1e-5);
		// After 2 Dishwasher ticks, WaterHeater temperature should = ((49.7635 * (200.0 - 1.1)) + (7 * 1.1))/ 200.0 = 49.52830075.
		dw.tick();
		TS_ASSERT_DELTA(wh1->getTemperature(), 49.5283, 1e-5);
		// After 3 Dishwasher ticks, WaterHeater temperature should = ((49.52830075 * (200.0 - 1.1)) + (7 * 1.1))/ 200.0 = 49.294395095875.
		dw.tick();
		TS_ASSERT_DELTA(wh1->getTemperature(), 49.2944, 1e-5);

		for (int i = 0; i < 17; i++) {
			dw.tick();
		}

		TS_ASSERT_EQUALS(dw.getTicks(), 20);
		TS_ASSERT_EQUALS(dw.isRunning(), true);
		TS_ASSERT_EQUALS(dw.getPower(), 2);

		// Check that power usage is 2 kW for entire Dishwasher cycle.
		for (int i = 0; i < 25; i++) {
			dw.tick();
		}
		TS_ASSERT_EQUALS(dw.getTicks(), 45);
		TS_ASSERT_EQUALS(dw.isRunning(), true);
		TS_ASSERT_EQUALS(dw.getPower(), 2);

		// Check that Dishwasher turns off after its 45-minute cycle is complete.
		dw.tick();
		TS_ASSERT_EQUALS(dw.getTicks(), 0);
		TS_ASSERT_EQUALS(dw.isRunning(), false);
		TS_ASSERT_EQUALS(dw.getPower(), 0.004);
		ptr->clear();
	}

	/** test_dishwasher_getEnergy:
	 * Tests that the Dishwasher energy is correctly updated.
	 */
	void test_dishwasher_getEnergy() {
		Scheduler sched;
		Dishwasher dw;
		WaterHeater wh;
		House *ptr = House::getInstance();
		Clock *c = new Clock();
		ptr->putModel(std::string("Clock"), c);
		ptr->putModel("WaterHeater", &wh);
		Clock* clk = (Clock*) (ptr->getModel("Clock"));
		clk->registerScheduler(&sched);
		dw.tick();
		dw.tick();
		TS_ASSERT_EQUALS(dw.getEnergy(), (2*0.004)/60);
		dw.activate("");
		for (int i = 0; i < 45; i++) { // run dishwasher's 45 minute cycle
			dw.tick();
		}
		TS_ASSERT_DELTA(dw.getEnergy(), ((2*0.004 + 45*2)/60), 1e-6);
		ptr->clear();
	}

	/** test_dishwasher_configure:
	 *  Tests that configuring the Dishwasher from a property table works correctly.
	 */
	void test_dishwasher_configure() {
		Dishwasher dw;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "DW"));
		props->insert(make_pair("powerOn", "3"));
		props->insert(make_pair("powerOff", "0.001"));
		dw.configure(props);
		TS_ASSERT(dw.getName() == "DW");
		TS_ASSERT_EQUALS(dw.powerOn_, 3);
		TS_ASSERT_EQUALS(dw.powerOff_, 0.001);
	}

	/** test_dishwasher_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_dishwasher_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Dishwasher dw;
		cout << endl << "test_dishwasher_shift(): " << dw;
	}

	/** test_dishwasher_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_dishwasher_cast() {
		Dishwasher dw;
		cout << endl << "test_dishwasher_cast(): int = " << (int)dw << ", double = " << (double)dw << endl;
		// Compare (double)dw with 0.0 using delta=0.1
		TS_ASSERT_DELTA((double)dw, 0.0, 0.1);
	}

};

#endif /* DISHWASHERTEST_H_ */
