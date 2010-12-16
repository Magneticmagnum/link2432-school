/*
 * HairDryer.h
 *
 *  Created on: Nov 23, 2010
 *      Author: Maya Pevzner
 */

#ifndef HAIRDRYER_H_
#define HAIRDRYER_H_

#include "Model.h"

class HairDryer: public Model {
public:
	HairDryer();
	virtual ~HairDryer();

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
	 * Returns boolean indicating if the HairDryer is running or not.
	 */
	bool isRunning();

	/** activate:
	 * Activates the HairDryer on level ("high" or "low") specified in args.
	 * If args is empty string, defaults to level "high".
	 */
	void activate(std::string args);

	/** deactivate:
	 * Deactivates the HairDryer.
	 */
	void deactivate();

	friend class HairDryerTest;

private:
	bool high_;
	double powerHi_;
	double powerLo_;
	bool running_;
};

#endif /* HAIRDRYER_H_ */
