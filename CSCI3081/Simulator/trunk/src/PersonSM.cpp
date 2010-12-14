/*
 * PersonSM.cpp
 *
 * A re done person that acts like a state machine
 *  Created on: Nov 18, 2010
 *      Author: Joe Houlton
 */

#include "PersonSM.h"
#include "ConfigFile.h"
#include "Stringutil.h"
#include "House.h"
#include "RandomInt.h"

/**
 * load the entire state file
 * uses config file class so file needs to be of the format
 * name-<[A-Za-z]*> next-<[A-Za-z]*> type-<[statetypes]> parameters-<[[A-Za-z0-9]*|]*>
 *
 * example: name-sleeping next-wake type-delay parameters-90
 */

PersonSM::PersonSM() {
	funcargs_.insert(std::make_pair("activate", &PersonSM::activate));
	funcargs_.insert(std::make_pair("waitOnTarget", &PersonSM::waitOnTarget));
	funcargs_.insert(std::make_pair("randomTime", &PersonSM::randomTime));
	funcargs_.insert(std::make_pair("upperTime", &PersonSM::upperTime));
	funcargs_.insert(std::make_pair("lowerTime", &PersonSM::lowerTime));
	funcargs_.insert(std::make_pair("delay", &PersonSM::delay));
}

PersonSM::~PersonSM() {
	delete currentstate;
}
int PersonSM::loadStateFile(const char *path) {
	ConfigFile cfg(path);
	std::map<std::string, PropertyTable*>::iterator begin =
			cfg.getPropsMap().begin();
	std::map<std::string, PropertyTable*>::iterator end =
			cfg.getPropsMap().end();
	while (begin != end) {
		stateargs_t *state = new stateargs_t;
		state->name = begin->first;
		PropertyTable *tbl = begin->second;
		state->next = (*tbl)["next"];
		state->type = (*tbl)["type"];
		state->params = (*tbl)["parameters"];
		state->stateentry = true;
		state->statefinished = false;
		addState(state);
	}
	std::map<std::string, stateargs_t*>::iterator begin1 = argmap_.begin();
	currentstate = begin1->second;
	return 0;
}

int PersonSM::addState(stateargs_t *state) {
	stateargs_t* exists = argmap_[state->name];
	if (exists == NULL) {
		argmap_.insert(std::make_pair(state->name, state));
		return 0;
	} else {
		// error, state already exists
		return -1;
	}
}

void PersonSM::tick() {
	if (currentstate->statefinished) {
		// just finished state, advance to next
		currentstate = argmap_[currentstate->next];
	}
	if (currentstate != NULL) {
		// call the appropriate state function with the params
		statetemplate_f f = funcargs_[currentstate->type];
		f(currentstate);
	}
}

int PersonSM::activate(stateargs_t* currentstate) {
	// log just entered state
	currentstate->stateentry = false;
	std::vector<std::string> *parsed = Stringutil::parseString2List(currentstate->params,
			"|");
	std::vector<std::string>::iterator begin = parsed->begin();
	std::vector<std::string>::iterator end = parsed->end();
	while (begin != end) {
		House* inst = House::getInstance();
		Model *mod;
		std::vector<std::string> *subparse = Stringutil::parseString2List(
				currentstate->params, ":");
		if (subparse->size() == 2) {
			// model needs to take a comma seperated argument list
			mod = inst->getModel((*subparse)[0]);
			mod->activate((*subparse)[1]);
		} else if (subparse->size() == 1) {
			mod = inst->getModel((*subparse)[0]);
			mod->activate("");
		}
		delete subparse;

		begin++;
	}
	currentstate->statefinished = true;
	delete parsed;
}

int PersonSM::waitOnTarget(stateargs_t* currentstate) {
	// parse everything, add model string list to pointer array
	if (currentstate->stateentry) {
		House* inst = House::getInstance();
		std::vector<std::string>* parse = Stringutil::parseString2List(
				currentstate->params, "|");
		currentstate->models = new Model*[parse->size()];
		std::vector<std::string>::iterator begin = parse->begin();
		std::vector<std::string>::iterator end = parse->end();
		int i = 0;
		while (begin != end) {
			currentstate->models[i] = inst->getModel(*begin);
			begin++;
			i++;
		}
		currentstate->stateentry = true;
	}
	int i;
	for (i = 0; i < currentstate->nummodels; i++) {
		if (currentstate->models[i]->getStateChanged()) {
			// model state changed, increment how many have changed
			currentstate->counter++;
		}
	}
	// could be in trouble if we skipped over nummodels, but that wouldn't happen
	if (currentstate->counter >= currentstate->nummodels) {
		currentstate->statefinished = true;
		delete[] currentstate->models;
	}
}

int PersonSM::randomTime(stateargs_t* currentstate) {
	if (currentstate->stateentry) {
		std::vector<std::string>* parse = Stringutil::parseString2List(
				currentstate->params, "|");
		int min = atoi((*parse)[0].c_str());
		int max = atoi((*parse)[1].c_str());
		// choose random between min and max
		RandomInt r(min, max);
		currentstate->time = r();
		currentstate->counter = 0;
		currentstate->stateentry = false;
		delete parse;
	}
	currentstate->counter++;
	if (currentstate->counter > currentstate->time) {
		currentstate->statefinished = true;
	}
}

int PersonSM::upperTime(stateargs_t* currentstate) {
	if (currentstate->stateentry) {
		std::vector<std::string>* parse = Stringutil::parseString2List(
				currentstate->params, "|");
		currentstate->time = atoi((*parse)[1].c_str());
		currentstate->counter = 0;
		currentstate->stateentry = false;
		delete parse;
	}
	currentstate->counter++;
	if (currentstate->counter > currentstate->time) {
		currentstate->statefinished = true;
	}
}

int PersonSM::lowerTime(stateargs_t* currentstate) {
	if (currentstate->stateentry) {
		std::vector<std::string>* parse = Stringutil::parseString2List(
				currentstate->params, "|");
		currentstate->time = atoi((*parse)[0].c_str());
		currentstate->counter = 0;
		currentstate->stateentry = false;
		delete parse;
	}
	currentstate->counter++;
	if (currentstate->counter > currentstate->time) {
		currentstate->statefinished = true;
	}
}

int PersonSM::delay(stateargs_t* currentstate) {
	if (currentstate->stateentry) {
		currentstate->time = atoi(currentstate->params.c_str());
		currentstate->counter = 0;
		currentstate->stateentry = false;
	}
	currentstate->counter++;
	if (currentstate->counter > currentstate->time) {
		currentstate->statefinished = true;
	}
}

/**
 * Determine if a random number condition is satisfied
 * syntax: chance%|<success state>|<fail state>
 *
 * advances to success state if chance succeeds.  Otherwise advances to fail state
 */
int PersonSM::choice(stateargs_t* currentstate) {
	if (currentstate->stateentry) {
		if (currentstate->stateentry) {
			std::vector<std::string>* parse = Stringutil::parseString2List(
					currentstate->params, "|");
			int pct = atoi((*parse)[0].c_str());
			// choose random between 0 and 100
			RandomInt r(0,100);
			int random = r();
			if (random < pct) {
				currentstate->next = (*parse)[1];
			} else {
				currentstate->next = (*parse)[2];
			}
			currentstate->statefinished = true;
			delete parse;
		}
	}
}
