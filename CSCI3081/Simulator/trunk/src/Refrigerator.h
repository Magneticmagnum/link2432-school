/**
 * Refrigerator.h
 *
 *  Created on: Oct 8, 2010
 *      Author: Maya Pevzner
 */

#ifndef REFRIGERATOR_H_
#define REFRIGERATOR_H_

#include "Model.h"

class Refrigerator: public Model {
public:
	Refrigerator();
	virtual ~Refrigerator();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	void configure(PropertyTable *props);

	/** tick:
	 * Increments the ticks. Logs current instantaneous power consumption and
	 * adds it to cumulative energy use.
	 * IF it's running:
	 * If ticks have not reached onTime, it logs that it is on.
	 * If ticks have reached onTime, it turns off, sets stateChanged to true,
	 * and logs that it is off.
	 * IF nobody is cooking and it's not running:
	 * If ticks have not reached (int)(onTime_/dutyCycleNotCooking_,
	 * it logs that it is off.
	 * If ticks have reached (int)(onTime_/dutyCycleNotCooking_,
	 * it turns on, sets stateChanged to true, resets ticks to 0,
	 * and logs that it is on.
	 * IF a Person is cooking and it's not running:
	 * If ticks have not reached (int)(onTime_/dutyCycleCooking_,
	 * it logs that it is off.
	 * If ticks have reached (int)(onTime_/dutyCycleCooking_,
	 * it turns on, sets stateChanged to true, resets ticks to 0,
	 * and logs that it is on.
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
	 * Returns boolean indicating if the Refrigerator is running or not.
	 */
	bool isRunning();

	/** getCookingState:
	 * Returns boolean indicating if the Refrigerator thinks someone is cooking or not.
	 */
	bool getCookingState();

	/** activate:
	 * Updates the Refrigerator's duty cycle state to cooking or not cooking based on args.
	 * In order to start "cooking" duty cycle, args must be "1".
	 * In order to start "not cooking" duty cycle, args must be "0".
	 */
	void activate(std::string args);

	friend class RefrigeratorTest;

private:
	bool cooking_;
	double dutyCycleCooking_;
	double dutyCycleNotCooking_;
	double onTime_;
	double powerOn_;
	double powerOff_;
	bool running_;
	int ticks_;
};

#endif /* REFRIGERATOR_H_ */
