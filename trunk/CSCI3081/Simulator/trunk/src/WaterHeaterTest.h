/** WaterHeaterTest.h:
 * Unit tests for the WaterHeater class.
 */

#ifndef WHTEST_H_
#define WHTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "WaterHeater.h"

using namespace std;

class WaterHeaterTest: public CxxTest::TestSuite {
public:

	/** test_waterHeater_constructor:
	 * Tests that the WaterHeater constructor creates a new instance, and
	 * that the instance starts out at 0 ticks, off, and w/water temperature 50 deg C.
	 */
	void test_waterHeater_constructor() {
		WaterHeater wh;
		TS_ASSERT_EQUALS(wh.getTicks(), 0);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getTemperature(), 50.0);
		TS_ASSERT_EQUALS(wh.powerOn_, 4.5);
		TS_ASSERT_EQUALS(wh.capacity_, 200.0);
		TS_ASSERT_EQUALS(wh.getName(), "DefaultWaterHeater");
	}

	/** test_waterHeater_drawWater:
	 * Tests that the WaterHeater drawWater method correctly updates the water heater's temperature.
	 */
	void test_waterHeater_drawWater() {
		WaterHeater wh;
		// Check that wh starts out at 0 ticks, off, temperature 50 deg C, & consuming no power.
		TS_ASSERT_EQUALS(wh.getTicks(), 0);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);
		TS_ASSERT_EQUALS(wh.getTemperature(), 50);
		// Draw 100 L of water at 42 deg C, then check that water temperature is updated correctly.
		wh.drawWater(100, 12);
		double water = 100.0 * (12.0 - 7) / (50.0 - 7);
		double temp = ((50.0 * (200.0 - water)) + (7 * water))/ 200.0;
		TS_ASSERT_DELTA(wh.getTemperature(), temp, 1e-4);
	}

	/** test_waterHeater_tick:
	 * Tests that the WaterHeater tick method increments the ticks,
	 * and that the water heater loses heat due to insulation defect,
	 * gains heat when it's on, turns on & off as needed based on
	 * water temperature, and consumes 3000 watts of power when on.
	 */
	void test_waterHeater_tick() {
		WaterHeater wh;

		// Check that wh starts out at 0 ticks, off, temperature 50 deg C, & consuming no power.
		TS_ASSERT_EQUALS(wh.getTicks(), 0);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);
		TS_ASSERT_EQUALS(wh.getTemperature(), 50);

		// Check that ticks are incremented, and that temperature, on/off state, & power usage are  updated correctly.
		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 1);
		TS_ASSERT_EQUALS(wh.getTemperature(), 50-0.00288);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);

		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 2);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-2*0.00288), 1e-4);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);

		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 3);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-3*0.00288), 1e-4);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);

		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 4);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-4*0.00288), 1e-4);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);

		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 5);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-5*0.00288), 1e-4);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);
		wh.tick();
		wh.tick();
		wh.tick();
		wh.tick();
		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 10);
		// Repeat for 334 more ticks (to get temp to just above 49 deg C by using ticks)
		for (int i = 0; i < 337; i++) {
			wh.tick();
		}
		TS_ASSERT_EQUALS(wh.getTicks(), 347);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-347*0.00288), 1e-4);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);

		// When temperature falls below 49 deg C, water heater should turn on (at 345 ticks)
		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 348);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-348*0.00288+0.3267182962), 1e-4);
		TS_ASSERT_EQUALS(wh.isRunning(), true);
		TS_ASSERT_EQUALS(wh.getPower(), 4.5);

		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 349);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-349*0.00288+2*0.3267182962), 1e-4);

		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 350);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-350*0.00288+3*0.3267182962), 1e-4);
		TS_ASSERT_EQUALS(wh.isRunning(), true);
		TS_ASSERT_EQUALS(wh.getPower(), 4.5);

		// When temperature rises above 50 deg C, water heater should turn off (after 351 ticks)
		wh.tick();
		TS_ASSERT_EQUALS(wh.getTicks(), 351);
		TS_ASSERT_DELTA(wh.getTemperature(), (50-351*0.00288+4*0.3267182962), 1e-4);
		TS_ASSERT_EQUALS(wh.isRunning(), false);
		TS_ASSERT_EQUALS(wh.getPower(), 0);

	}

	/** test_waterHeater_getEnergy:
	 * Tests that the WaterHeater energy is correctly updated.
	 */
	void test_waterHeater_getEnergy() {
		WaterHeater wh;
		wh.tick();
		wh.drawWater(100, 42);
		wh.tick();
		wh.tick();
		//wh.tick();
		TS_ASSERT_DELTA(wh.getEnergy(), (4.5*2)/60.0, 1e-6);
	}

	/** test_waterHeater_configure:
	 *  Tests that configuring the WaterHeater from a property table works correctly.
	 */
	void test_waterHeater_configure() {
		WaterHeater wh;
		PropertyTable *props = new PropertyTable;
		props->insert(make_pair("name", "WH"));
		props->insert(make_pair("powerOn", "5.0"));
		props->insert(make_pair("powerOff", "0.001"));
		wh.configure(props);
		TS_ASSERT(wh.getName() == "WH");
		TS_ASSERT_EQUALS(wh.powerOn_, 5);
		TS_ASSERT_EQUALS(wh.powerOff_, 0.001);
	}

	/** test_waterHeater_activate:
	 * Tests that the WaterHeater activate method correctly draws water.
	 */
	void test_waterHeater_activate() {
		WaterHeater wh;
		wh.activate("on,100, 12");
		double water = 100.0 * (12.0 - 7) / (50.0 - 7);
		double temp = ((50.0 * (200.0 - water)) + (7 * water))/ 200.0;
		TS_ASSERT_DELTA(wh.getTemperature(), temp, 1e-4);
	}

	/** test_waterHeater_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_waterHeater_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		WaterHeater wh;
		cout << endl << "test_waterHeater_shift(): " << wh;
	}

	/** test_waterHeater_subtractionAssignment:
	 *  Tests that "-=" operator is overloaded to withdraw hot water from a WaterHeater.
	 */
	void test_waterHeater_subtractionAssignment() {
		WaterHeater wh;
		wh -= 100;
		TS_ASSERT_DELTA(wh.getTemperature(), 28.5, 0.1);
	}

	/** test_waterHeater_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_waterHeater_cast() {
		WaterHeater wh;
		cout << endl << "test_waterHeater_cast(): int = " << (int)wh << ", double = " << (double)wh << endl;
		// Compare (double)wh with 0.0 using delta=0.1
		TS_ASSERT_DELTA((double)wh, 0.0, 0.1);
	}

};

#endif /*WHTEST_H_*/
