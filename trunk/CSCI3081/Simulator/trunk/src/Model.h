/** Model.h
 *
 *  Created on: Oct 8, 2010
 *      Author: pevz0003
 */

#ifndef MODEL_H_
#define MODEL_H_
#include "Scheduler.h"
#include "ConfigFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <log4cxx/logger.h>

using log4cxx::Logger;
using log4cxx::LoggerPtr;
using namespace std;

class Model {
public:
	Model();
	virtual ~Model();

	/** Shift (<<) operator:
	 * Stream put out the name (using std:ostream&), current power draw, and total energy use.
	 */
	friend ostream& operator<<(ostream& os, Model& m);

	/** Cast operator:
	 * Returns total energy consumption in a specified type -- int or double.
	 */
	operator int();
	operator double();

	/** configure:
	 * Passed a PropertyTable*, uses table to configure model settings
	 * using the associated name-value pairs for variables.
	 */
	virtual void configure(PropertyTable*);

	/** setName:
	 * Set name of the Model.
	 */
	void setName(std::string &other);

	/** getName:
	 * Return the (immutable) name of the Model.
	 */
	const std::string getName();

	/** tick:
	 * Notifies the Model that one tick of time has passed since last call to tick().
	 * (Subclasses must implement this function to create their behaviors.)
	 */
	virtual void tick();

	/** getPower:
	 * Returns instantaneous power usage of this Model.
	 */
	double getPower();

	virtual int getData();

	/** getEnergy:
	 * Returns total energy usage of this Model since it was created.
	 */
	double getEnergy();

	/** setScheduler:
	 * Sets the Scheduler for the Model.
	 */
	void setScheduler(Scheduler* sched);

	/** getScheduler:
	 * Returns the Scheduler for the Model.
	 */
	Scheduler* getScheduler();

	/** getStateChanged:
	 * Returns value of stateChanged_ variable.
	 */
	bool getStateChanged();

	bool getNotifyState();

	/** activate():
	 * Activates the model based on arguments.
	 */
	virtual void activate(std::string args);

	friend class ModelTest;

protected:
	Scheduler *scheduler_;
	double energy_;
	double power_;
	std::string name_;
	bool stateChanged_;
	bool notifyStateChanged_;

	/** logOn:
	 * Write INFO message to log saying Model is turned on and giving the time.
	 */
	void logOn();

	/** logOff:
	 * Write INFO message to log saying Model is turned off and giving the time & energy use.
	 */
	void logOff();

	/** logPower:
	 * Write log message to log stating Model's instantaneous power usage.
	 * If Model state has changed, logging is at INFO level, otherwise at DEBUG level.
	 */
	void logPower();
};
#endif /* MODEL_H_ */
