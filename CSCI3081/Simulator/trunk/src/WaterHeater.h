/*
 * WaterHeater.h
 *
 *  Created on: Nov 5, 2010
 *      Author: Nguyen, Maya Pevzner
 */

#ifndef WATERHEATER_H_
#define WATERHEATER_H_

#include "Model.h"
#include <iostream>

using namespace std;

class WaterHeater: public Model {
public:
	WaterHeater();
	virtual ~WaterHeater();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	void configure(PropertyTable* props);

	/** Subtraction assignment (-=) operator:
	 * Withdraw hot water from a waterheater.
	 */
	friend WaterHeater& operator-=(WaterHeater& wh, double amt);

	/*
	 * drawWater:
	 * Method to determine update the new temperature_ after a person uses "quantity" amount of water
	 * at "temp" temperature.
	 */
	void drawWater(double quantity, double temp);

	/** tick:
	 * Decrements temperature to reflect heat loss due to imperfect thermal insulation.
	 * Logs current instantaneous power consumption.
	 * If running state is set to true, increases temperature to reflect heating and logs that it is on.
	 * If running state is set to true and temperature is >=50, it turns off, makes state changed true,
	 * logs the current temperature, and logs that it is off.
	 * If running state is set to false, it logs that it is off.
	 * If running state is set to false and temperature is <49, it turns on, makes state changed true,
	 * logs the current temperature, logs that it is on, and increases temperature to reflect heating.
	 * Finally, adds instantaneous power consumption to cumulative energy use.
	 */
	void tick();

	/** getPower:
	 * Returns instantaneous power usage of this Model.
	 */
	double getPower();

	/*
	 * getTemperature:
	 * Returns the temperature of the water.
	 */
	double getTemperature();

	/** getTicks:
	 * Returns the number of ticks it's on.
	 */
	int getTicks();

	/** isRunning:
	 * Returns true if WaterHeater is running. Otherwise, returns false.
	 */
	bool isRunning();

	/** activate:
	 * Can be used by PersonSM to draw water.
	 * Comma-delimited args are parsed into quantity and temperature.
	 */
	void activate(std::string args);

	friend class WaterHeaterTest;

private:
	double capacity_;
	double powerOff_;
	double powerOn_;
	bool running_;
	bool drawing_;
	double drawTemp_;
	double drawAmt_;
	double temperature_;
	int ticks_;

	/** logTemp:
	 * Write INFO message to log giving water temperature.
	 */
	void logTemp();
};

#endif /* WATERHEATER_H_ */
