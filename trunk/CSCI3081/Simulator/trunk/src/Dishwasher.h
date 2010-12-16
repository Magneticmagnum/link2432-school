/*
 * Dishwasher.h
 *
 *  Created on: Nov 12, 2010
 *      Author: nguyen
 */

#ifndef DISHWASHER_H_
#define DISHWASHER_H_

#include "Model.h"

class Dishwasher: public Model {
public:
	Dishwasher();
	virtual ~Dishwasher();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	void configure(PropertyTable* props);

	/** tick:
	 * Logs current instantaneous power consumption.
	 * If running state is set to true and ticks <20, it finds a WaterHeater and draws (22/20 = 1.1)
	 * liters of water, at the current temperature of the water in the WaterHeater, from the WaterHeater.
	 * If running is set to true and ticks <45, it stays on, logs that it is on, and increments the ticks.
	 * If running is set to true and ticks >=45, it turns off, logs that it is off, resets ticks to 0, and
	 * sets stateChanged_ state to true.
	 * Finally, adds instantaneous power consumption to cumulative energy use.
	 */
	void tick();

	/** getPower:
	 * Returns instantaneous power usage of this Model.
	 */
	double getPower();

	/** getTicks:
	 * Returns the number of ticks it's on.
	 */
	int getTicks();

	/** isRunning:
	 * Returns boolean indicating if the Dishwasher is running or not.
	 */
	bool isRunning();

	// returns days since last ran
	int getData();

	/** activate:
	 * Activates the Dishwasher.
	 */
	void activate(std::string args);

	friend class DishwasherTest;

private:
	double powerOff_;
	double powerOn_;
	bool running_;
	int ticks_;
	int daysSinceLast_;
	int currentDay_;
};

#endif /* DISHWASHER_H_ */
