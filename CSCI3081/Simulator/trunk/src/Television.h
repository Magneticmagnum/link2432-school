/*
 * Television.h
 *
 *  Created on: Nov 12, 2010
 *      Author: nguyen, Maya Pevzner
 */

#ifndef TELEVISION_H_
#define TELEVISION_H_

#include "Model.h"

class Television: public Model {
public:
	Television();
	virtual ~Television();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	void configure(PropertyTable* props);

	/** tick:
	 * Logs current instantaneous power consumption and adds it to cumulative energy use.
	 * If running state is set to true, it logs that it is on.
	 * If running state is set to false, it logs that it is off.
	 */
	void tick();

	/** getPower:
	 * Returns instantaneous power usage of this Model.
	 */
	double getPower();

	/** isRunning:
	 * Returns boolean indicating if the Television is running or not.
	 */
	bool isRunning();

	/** activate:
	 * Turns Television on if args is "on", and off if args is "off".
	 */
	void activate(std::string args);

	friend class TelevisionTest;

private:
	double powerOff_;
	double powerOn_;
	bool running_;
};

#endif /* TELEVISION_H_ */
