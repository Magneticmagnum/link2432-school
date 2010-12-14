/**
 * Microwave.h
 *
 *  Created on: Nov 12, 2010
 *      Author: Maya Pevzner
 */

#ifndef MICROWAVE_H_
#define MICROWAVE_H_

#include "Model.h"
#include "RandomInt.h"

class Microwave: public Model {
public:
	Microwave();
	virtual ~Microwave();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	void configure(PropertyTable* props);

	/** tick:
	 * Logs current instantaneous power consumption and adds it to cumulative energy use.
	 * If running state is set to true, it logs that it is on and logs the run time left.
	 * If run time left is > 60 (seconds), it increments ticks and decrements run time left by 60.
	 * Otherwise it sets ticks to 0, run time left to 0, turns off, sets state changed to true,
	 * and logs that it has turned off
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
	 * Returns boolean indicating if the Microwave is running or not.
	 */
	bool isRunning();

	/** activate:
	 * Activates the Microwave.
	 * If args is empty string, selects session run time, in seconds, from a uniform distribution across 40-120 sec.
	 * If args is valid integer string, uses that as run time.
	 */
	void activate(std::string args);

	friend class MicrowaveTest;

private:
	double powerOff_;
	double powerOn_;
	RandomInt r_;
	bool running_;
	int runTimeLeft_;
	int ticks_;

	/** logRunTimeLeft:
	 * Write INFO message to log giving the run time.
	 */
	void logRunTimeLeft();
};

#endif /* MICROWAVE_H_ */
