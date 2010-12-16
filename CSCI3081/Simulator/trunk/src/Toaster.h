/**
 * Toaster.h
 *
 *  Created on: Nov 11, 2010
 *      Author: Maya Pevzner
 */

#ifndef TOASTER_H_
#define TOASTER_H_

#include "Model.h"

class Toaster: public Model {
public:
	Toaster();
	virtual ~Toaster();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	void configure(PropertyTable* props);

	/** tick:
	 * Logs current instantaneous power consumption.
	 * If running state is set to true, it logs that it is on.
	 * If running state is set to true and it has been running for <2 minutes, increments ticks.
	 * If running state is set to true and it has been running for >=2 minutes, it turns off,
	 * makes state changed true, logs that it is off, and resets ticks to 0.
	 * If running state is set to false, it logs that it is off.
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
	 * Returns boolean indicating if the Toaster is running or not.
	 */
	bool isRunning();

	/** activate:
	 * Activates the Toaster.
	 * Parameter args is inherited from superclass. Unused for Toaster.
	 */
	void activate(std::string args);

	friend class ToasterTest;

private:
	double powerOff_;
	double powerOn_;
	bool running_;
	int ticks_;
};

#endif /* TOASTER_H_ */
