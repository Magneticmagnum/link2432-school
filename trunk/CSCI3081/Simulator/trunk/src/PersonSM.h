/*
 * PersonSM.h
 *
 *  Created on: Nov 18, 2010
 *      Author: Joe Houlton
 */

#ifndef PERSONSM_H_
#define PERSONSM_H_

#include <map>
#include <string>
#include "Model.h"

// name-<string> next-<string> type-<(activate, waitontarget, randomtime, uppertime, lowertime, delay)>
// activate <target1:parameters1,parameter2|target2|target3:param1>
// waitOnTarget <target1>
// randomTime <begin> <end>
// upperTime <begin> <end>
// lowerTime <begin> <end>
// delay <amount>
typedef struct {
	std::string name;
	std::string next;
	std::string type;
	std::string params;
	bool statefinished;
	bool stateentry;
	int counter;
	int time;
	Model** models;
	int nummodels;
} stateargs_t;

typedef int (*statetemplate_f)(stateargs_t*);

class PersonSM: public Model {

public:
	PersonSM();
	~PersonSM();

	/** loadStatefile(const char* path):
	 * Loads states from a config file, parses config data, stores it in a state, and adds the state to the state map.
	 * Uses config file class so file needs to be of the format:
	 * name-<[A-Za-z]*> next-<[A-Za-z]*> type-<[statetypes]> parameters-<[[A-Za-z0-9]*|]*>
	 * example: name-sleeping next-wake type-delay parameters-90
	 */
	int loadStateFile(const char* path);

	/** tick():
	 * Tick the Person, performing whatever actions are necessary in their current state.
	 */
	void tick();

	friend class PersonSMTest;

private:
	// Map to the stateargs_t entries in the config file
	std::map<std::string, stateargs_t *> argmap_;
	std::map<std::string, int(*)(stateargs_t *)> funcargs_;

	/** addState(stateargs_t *state):
	 * Adds a state to the state map.
	 */
	int addState(stateargs_t *state);

	// State-type functions

	/** activate(std::string args):
	 * Activate a state.
	 */
	static int activate(stateargs_t*);

	/** waitOnTarget(std::string args):
	 * Wait for one or more models for their state to change.
	 */
	static int waitOnTarget(stateargs_t*);

	/** randomTime(PersonSM::stateargs_t**):
	 * Expects two args, min and max. Chooses random between them.
	 */
	static int randomTime(stateargs_t*);

	/** upperTime(PersonSM::stateargs_t**):
	 * Expects two args, min and max. Always returns upper time.
	 */
	static int upperTime(stateargs_t*);

	/** lowerTime(PersonSM::stateargs_t**):
	 * Expects two args, min and max. Always returns lower time.
	 */
	static int lowerTime(stateargs_t*);

	/** delay(PersonSM::stateargs_t**):
	 * Delay for constant time.  Increment internal counter and set statefinished when done.
	 */
	static int delay(stateargs_t*);

	/** choice(stateargs_t*):
	 * Determine if a random number condition is satisfied.
	 * syntax: chance%|<success state>|<fail state>
	 * Advances to success state if chance succeeds.  Otherwise advances to fail state
	 */
	static int choice(stateargs_t*);

	/** end(stateargs_t*):
	 * Represents the end of the state machine. Does nothing - prevents segfault.
	 */
	static int end(stateargs_t*);

	/** timedWait(stateargs_t*):
	 * Wait until a time of day.
	 */
	static int timedWait(stateargs_t*);

	/** dataChoice(stateargs_t *):
	 * If model data (getData) == arg, choice 1.  Otherwise choice 2.
	 * This is basically for checking dishwasher days since ran...
	 * Syntax: Model|expectedValue|SuccessState|FailureState
	 */
	static int dataChoice(stateargs_t *);

	/** earlierTimeChoice(stateargs_t *):
	 *  Checks the clock. If the current time is less than the given time, advance to choice 1, otherwise choice 2.
	 */
	static int earlierTimeChoice(stateargs_t *);

	/** laterTimeChoice(stateargs_t *):
	 *  Checks the clock. If the current time is greater than the given time, advance to choice 1, otherwise choice 2.
	 */
	static int laterTimeChoice(stateargs_t *);
	// Pointer to current state argument map
	stateargs_t* currentstate;
};

#endif /* PERSONSM_H_ */
