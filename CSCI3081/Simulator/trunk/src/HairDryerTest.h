/** HairDryerTest.h:
 * Unit test for the HairDryer class.
 */
#ifndef HAIRDRYERTEST_H_
#define HAIRDRYERTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "ConfigFile.h"
#include "HairDryer.h"

class HairDryerTest: public CxxTest::TestSuite {
public:

	/** test_hairDryer_constructor:
	 * Tests that the HairDryer constructor creates a new instance, and
	 * that the instance starts out at 0 ticks and running.
	 */
	void test_hairDryer_constructor() {
		HairDryer hd;
		TS_ASSERT_EQUALS(hd.isRunning(), false);
		TS_ASSERT_EQUALS(hd.stateChanged_, true);
		TS_ASSERT_EQUALS(hd.getName(), "DefaultHairDryer");
	}

	/** test_hairDryer_tick:
	 * Tests that the HairDryer tick method correctly updates the power depending on if it's running or not.
	 */
	void test_hairDryer_tick() {
		HairDryer hd;
		// Check that TV starts out not running.
		TS_ASSERT_EQUALS(hd.isRunning(), false);
		TS_ASSERT_EQUALS(hd.getPower(), 0);
		hd.tick();
		TS_ASSERT_EQUALS(hd.getPower(), 0);
		// Check that turning HairDryer on updates power usage & running state
		hd.activate("high");
		hd.tick();
		TS_ASSERT_EQUALS(hd.isRunning(), true);
		TS_ASSERT_EQUALS(hd.getPower(), 1.0);
		hd.tick();
		TS_ASSERT_EQUALS(hd.getPower(), 1.0);
		hd.activate("low");
		hd.tick();
		TS_ASSERT_EQUALS(hd.isRunning(), true);
		TS_ASSERT_EQUALS(hd.getPower(), 0.5);
		hd.tick();
		TS_ASSERT_EQUALS(hd.getPower(), 0.5);
		// Check that turning HairDryer off updates power usage & running state
		hd.deactivate();
		hd.tick();
		TS_ASSERT_EQUALS(hd.isRunning(), false);
		TS_ASSERT_EQUALS(hd.getPower(), 0);
	}

	/** test_hairDryer_getEnergy:
	 * Tests that the HairDryer energy is correctly updated.
	 */
	void test_hairDryer_getEnergy() {
		HairDryer hd;
		hd.tick();
		hd.activate("high");
		hd.tick();
		hd.activate("low");
		hd.tick();
		hd.deactivate();
		hd.tick();
		TS_ASSERT_DELTA(hd.getEnergy(), (1.0+0.5)/60, 1e-6);
	}

	/** test_hairDryer_configure:
	 *  Tests that configuring the HairDryer from a property table works correctly.
	 */
	void test_hairDryer_configure() {
		HairDryer hd;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "HD"));
		props->insert(make_pair("powerHi", "1.3"));
		props->insert(make_pair("powerLo", "0.3"));
		hd.configure(props);
		TS_ASSERT(hd.getName() == "HD");
		TS_ASSERT(hd.powerHi_ == 1.3);
		TS_ASSERT(hd.powerLo_ == 0.3);
	}

	/** test_hairDryer_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_hairDryer_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		HairDryer hd;
		cout << endl << "test_hairDryer_shift(): "  << hd;
	}

	/** test_hairDryer_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_hairDryer_cast() {
		HairDryer hd;
		hd.tick();
		cout << endl << "test_hairDryer_cast(): int = " << (int)hd << ", double = " << (double)hd << endl;
		// Compare with 0.01/60
		TS_ASSERT_EQUALS((int)hd, 0);
		TS_ASSERT_EQUALS((double)hd, 0.0);
	}
};

#endif /*HAIRDRYERTEST_H_*/
