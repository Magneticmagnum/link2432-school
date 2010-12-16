/*
 * Stove.h
 *
 *  Created on: Nov 12, 2010
 *      Author: nguyen
 */

#ifndef STOVE_H_
#define STOVE_H_

#include "Model.h"

class Stove: public Model {
public:
	Stove();
	virtual ~Stove();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	void configure(PropertyTable* props);

	/** tick:
	 * Logs current instantaneous power consumption and adds it to cumulative energy use.
	 * If running state is set to true, it logs that it is on and logs the state of each burner.
	 * Keeps track of stove's instantaneous power (across all 4 burners) using "sum" variable.
	 * Loops through burners array and corresponding cook time array, decrementing each burner's
	 * cook time. If a burner's cook time is 0, that burner is turned off, state changed is set
	 * to true, and the state of each burner is logged again.
	 * If sum is 0 (i.e. all burners are off), running state gets set to false, state changed
	 * gets set to true, and it logs that it is off.
	 * If running state is set to false, it logs that it is off.
	 */
	void tick();

	/** getPower:
	 * Returns instantaneous power usage of this Model.
	 */
	double getPower();

	/** isRunning:
	 * Returns boolean indicating if the Stove is running or not.
	 */
	bool isRunning();

	/** activate:
	 * Activates the Stove by processing args.
	 * @param args : comma-delim. string burner,heat,duration,burner,heat,duration...
	 * where bx is heat level for burner x (	 * In order to start cooking, args must be "1".
	 * ), and cx is time to use burner x at levent bx.
	 */
	void activate(std::string args);

	/** logBurners:
	 * When burners are turned on or off, write INFO message to log giving the heat level of each burner.
	 */
	void logBurners();

	friend class StoveTest;

private:
	int burners_[4];
	int cookTime_[4];
	double powerOn_;
	double powerOff_;
	bool running_;
};

#endif /* STOVE_H_ */
