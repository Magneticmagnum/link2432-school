/**
 * Oven.h
 *
 *  Created on: Nov 12, 2010
 *      Author: Maya Pevzner
 */

#ifndef OVEN_H_
#define OVEN_H_

#include "Model.h"

class Oven: public Model {
public:
	Oven();
	virtual ~Oven();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	void configure(PropertyTable* props);

	/** tick:
	 * Logs current instantaneous power consumption and adds it to cumulative energy use.
	 * If running state is set to true, it logs that it is on.
	 * If ticks cooking time (duration_ + 10) hasn't passed yet, increment ticks.
	 * If ticks = 10, it just changed from preheat to bake, so set stateChanged to true.
	 * If ticks cooking time (duration_ + 10) has passed, turns off, resets ticks to 0,
	 * sets stateChanged to true, and logs that it is off.
	 * If running state is set to false, it logs that it is off.
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
	 * Returns boolean indicating if the Oven is running or not.
	 */
	bool isRunning();

	/** activate:
	 * Activates the Oven.
	 * Preheats for 10 min, then switches to baking for duration specified in args.
	 */
	void activate(std::string args);

	friend class OvenTest;

private:
	double powerPreheat_;
	double powerOn_;
	double powerOff_;
	bool running_;
	int ticks_;
	int duration_;
};

#endif /* OVEN_H_ */
