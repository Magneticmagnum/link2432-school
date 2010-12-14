/** Scheduler.h
 *
 *  Created on: Oct 8, 2010
 *      Author: pevz0003
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <vector>
#include <string>

class Model;

class Scheduler {
public:
	Scheduler();
	virtual ~Scheduler();

	/** registerModel:
	 * Adds Model instance thing to list of models that will be notified of ticks.
	 */
	bool registerModel(Model &thing);

	/** getTime:
	 * Returns number of ticks (Minutes) that have passed since Scheduler was started.
	 */
	int getTime();

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

	/** run:
	 * Increments tick counter, writes an INFO message to log giving current tick count,
	 * and notifies each of its registered Models by calling their tick() methods until maxCount ticks have been generated.
	 */
	void run(int maxCount);

	/** getModels:
	 * Returns models_ vector for the Scheduler.
	 */
	std::vector<Model*> getModels();

	/** incrementTime:
	 * Increments current time by 1 minute.
	 */
	void incrementTime();

	/** getSumPower:
	 * Returns instantaneous power usage in kW, summed across all active Model instances.
	 */
	double getSumPower();

	/** getSumEnergy:
	 * Returns cumulative energy usage in kWh since the beginning of the simulation, at the end of each tick cycle.
	 */
	double getSumEnergy();

	friend class PersonTest;

private:
	int time_;
	std::vector<Model*> models_;
	double sumPower_;
	double sumEnergy_;

};

#endif /* SCHEDULER_H_ */
