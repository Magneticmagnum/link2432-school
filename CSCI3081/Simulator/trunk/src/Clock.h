/*
 * Clock.h
 *
 *  Created on: Dec 15, 2010
 *      Author: Joe
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "Model.h"
#include "ConfigFile.h"
#include "Scheduler.h"
#include <string>

class Clock: public Model {
public:
	Clock();
	virtual ~Clock();

	/** configure(PropertyTable* props):
	 * Inherited method from superclass. Does nothing for the Clock Model.
	 */
	void configure(PropertyTable* props);

	/** registerScheduler(Scheduler *sched):
	 * Registers a Scheduler pointer with the Clock.
	 */
	void registerScheduler(Scheduler *sched);

	/** tick():
	 * Inherited method from superclass. Does nothing for the Clock Model.
	 */
	void tick();

	/** getTime:
	 * Returns number of ticks (Minutes) that have passed since Scheduler was started.
	 */
	int getTime();

	/** getMinutesSinceMidnight():
	 * Returns number of ticks (Minutes) that have passed since midnight.
	 */
	int getMinutesSinceMidnight();

	/** getMinute:
	 * Returns number of Minutes in the Time that has passed since Scheduler was started.
	 */
	int getMinute();

	/** getHours:
	 * Returns number of Hours in the Time that has passed since Scheduler was started.
	 */
	int getHours();

	/** getDay:
	 * Returns number of Days in the Time that has passed since Scheduler was started.
	 */
	int getDay();

	/** getFormattedTime():
	 * Returns formatted time (used for logging).
	 */
	std::string getFormattedTime();

	/** activate(std::string args):
	 * Inherited method from superclass. Does nothing for the Clock Model.
	 */
	void activate(std::string args);

	friend class ClockTest;

private:
	Scheduler *scheduler_;
};

#endif /* CLOCK_H_ */
