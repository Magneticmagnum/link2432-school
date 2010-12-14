/*
 * Xbox.h
 *
 *  Created on: Nov 23, 2010
 *      Author: Maya Pevzner
 */

#ifndef XBOX_H_
#define XBOX_H_

#include "Model.h"

class Xbox: public Model {
public:
	Xbox();
	virtual ~Xbox();

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
	 * Returns boolean indicating if the Xbox is running or not.
	 */
	bool isRunning();

	/** activate:
	 * Activates the Xbox.
	 */
	void activate();

	/** deactivate:
	 * Deactivates the Xbox.
	 */
	void deactivate();

	friend class XboxTest;

private:
	double powerOff_;
	double powerOn_;
	bool running_;
};

#endif /* XBOX_H_ */
