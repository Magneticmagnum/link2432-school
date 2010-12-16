/*
 * PersonSM.cpp
 *
 * A re done person that acts like a state machine
 *  Created on: Nov 18, 2010
 *      Author: Joe Houlton
 */

#include "PersonSM.h"
#include "ConfigFile.h"
#include <log4cxx/logger.h>
#include "Stringutil.h"
#include "House.h"
#include "RandomInt.h"
#include "Clock.h"
#include <iostream>
#include <sstream>
#include <iomanip>

PersonSM::PersonSM() {
	funcargs_.insert(std::make_pair("activate", &PersonSM::activate));
	funcargs_.insert(std::make_pair("waitOnTarget", &PersonSM::waitOnTarget));
	funcargs_["choice"] = &PersonSM::choice;
	funcargs_.insert(std::make_pair("randomTime", &PersonSM::randomTime));
	funcargs_.insert(std::make_pair("upperTime", &PersonSM::upperTime));
	funcargs_.insert(std::make_pair("lowerTime", &PersonSM::lowerTime));
	funcargs_.insert(std::make_pair("delay", &PersonSM::delay));
	funcargs_.insert(std::make_pair("end", &PersonSM::end));
	funcargs_.insert(std::make_pair("timedWait", &PersonSM::timedWait));
	funcargs_.insert(std::make_pair("dataChoice", &PersonSM::dataChoice));
	funcargs_.insert(std::make_pair("earlierTimeChoice",
			&PersonSM::earlierTimeChoice));
	funcargs_.insert(std::make_pair("laterTimeChoice",
			&PersonSM::laterTimeChoice));
	std::string name = "DefaultPersonSM";
	setName(name);
	currentstate = NULL;
}

PersonSM::~PersonSM() {
	if (currentstate != NULL) {
		delete currentstate;
	}
}
int PersonSM::loadStateFile(const char *path) {
	ConfigFile cfg(path);
	std::map<std::string, PropertyTable*>::iterator begin = cfg.begin();
	std::map<std::string, PropertyTable*>::iterator end = cfg.end();
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
		begin++;
	}
	stateargs_t *endstate = new stateargs_t;
	endstate->name = "END";
	endstate->type = "end";
	endstate->stateentry = true;
	endstate->statefinished = false;
	addState(endstate);
	currentstate = (argmap_.find("BEGIN"))->second;
	return 0;
}

int PersonSM::addState(stateargs_t *state) {
	// find it in the map
	if (state == NULL) {
		return -1;
	}
	std::map<std::string, stateargs_t*>::iterator find = argmap_.find(
			state->name);
	if (find == argmap_.end()) {
		argmap_[state->name] = state;
		LoggerPtr personLog(Logger::getLogger("PersonSM"));
		LOG4CXX_DEBUG(personLog, "PersonSM :: addState :: " << state->name);
		if (currentstate == NULL) {
			LOG4CXX_DEBUG(personLog, "PersonSM :: addState :: null "
					<< state->name);
			currentstate = state;
		}
		return 0;
	} else {
		// error, state already exists
		return -1;
	}
}

void PersonSM::tick() {
	Model::tick();
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	LOG4CXX_INFO(personLog, getName() << " :: tick " << currentstate->name);
	if (currentstate->statefinished) {
		// just finished state, advance to next
		std::string prevname = currentstate->name;
		std::string prevnext = currentstate->next;
		currentstate = argmap_[currentstate->next];
		if (currentstate != NULL) {
			LOG4CXX_INFO(personLog, getName() << " :: Change state from "
					<< currentstate->name << " to " << currentstate->next);
		} else {
			LOG4CXX_ERROR(personLog, getName()
					<< " :: Tried to change state from " << prevname << " to "
					<< prevnext << " but couldn't find it! ENDING");
			currentstate = argmap_.find("END")->second;
		}
	}
	if (currentstate != NULL) {
		// call the appropriate state function with the params
		statetemplate_f f = funcargs_[currentstate->type];
		if (f == NULL) {
			LOG4CXX_ERROR(personLog, getName() << " :: NULL event pointer! "
					<< currentstate->type);
			currentstate = argmap_.find("END")->second;
		}
		f(currentstate);
	}
}

int PersonSM::activate(stateargs_t* currentstate) {
	// log just entered state
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	LOG4CXX_INFO(personLog, "PersonSM :: static :: Activate :: "
			<< currentstate->name);
	currentstate->stateentry = false;
	std::vector<std::string> *parsed = parseString(currentstate->params, "|");
	std::vector<std::string>::iterator begin = parsed->begin();
	std::vector<std::string>::iterator end = parsed->end();
	while (begin != end) {
		House* inst = House::getInstance();
		std::vector<std::string> *subparse = parseString(*begin, ":");
		Model *mod;
		LOG4CXX_INFO(personLog, "PersonSM :: Parsed strings");
		if (subparse->size() == 2) {
			// model needs to take a comma seperated argument list
			LOG4CXX_INFO(personLog, "mod = inst->getModel((*subparse)[0]);");
			mod = inst->getModel((*subparse)[0]);
			LOG4CXX_INFO(personLog, "mod->activate((*subparse)[1]);");
			mod->activate((*subparse)[1]);
			LOG4CXX_INFO(personLog, "PersonSM :: Activated model "
					<< mod->getName());
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
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	// parse everything, add model string list to pointer array
	if (currentstate->stateentry) {
		LOG4CXX_INFO(personLog, "PersonSM :: static :: Wait on target :: "
				<< currentstate->name);
		House* inst = House::getInstance();
		std::vector<std::string>* parse =
				parseString(currentstate->params, "|");
		if (parse == NULL) {
			LOG4CXX_ERROR(personLog, "PersonSM :: static :: Wait on target :: "
					<< currentstate->name
					<< " attempting to parse NULL string.  Skipping!");
			currentstate->statefinished = true;
			currentstate->nummodels = 0;
		} else {
			currentstate->nummodels = parse->size();
			currentstate->models = new Model*[currentstate->nummodels];
			std::vector<std::string>::iterator begin = parse->begin();
			std::vector<std::string>::iterator end = parse->end();
			currentstate->counter = 0;
			int i = 0;
			while (begin != end) {
				currentstate->models[i] = inst->getModel(*begin);
				begin++;
				i++;
			}
			delete parse;
			currentstate->stateentry = false;
		}
	}

	int i;
	for (i = 0; i < currentstate->nummodels; i++) {
		// if it's null we dont count it
		Model *m = currentstate->models[i];
		if (m != NULL) {
			if (m->getNotifyState()) {
				// model state changed, increment how many have changed
				currentstate->counter++;
				LOG4CXX_DEBUG(personLog,
						"PersonSM :: static :: Wait on target :: waiting on "
								<< currentstate->nummodels
								<< " models, so far have "
								<< currentstate->counter);
			}
		} else {
			LOG4CXX_ERROR(personLog, "PersonSM :: static :: Wait on target :: "
					<< currentstate->name
					<< " attempting to wait on NULL model.  Skipping!");
			currentstate->counter++;
		}
		// could be in trouble if we skipped over nummodels, but that wouldn't happen
		if (currentstate->counter >= currentstate->nummodels) {
			currentstate->statefinished = true;
			delete currentstate->models;
		}
	}
}

int PersonSM::randomTime(stateargs_t* currentstate) {
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	if (currentstate->stateentry) {
		LOG4CXX_INFO(personLog, "PersonSM :: static :: Random time :: "
				<< currentstate->name);
		std::vector<std::string>* parse =
				parseString(currentstate->params, "|");
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
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	if (currentstate->stateentry) {
		LOG4CXX_INFO(personLog, "PersonSM :: static :: Upper time :: "
				<< currentstate->name);
		std::vector<std::string>* parse =
				parseString(currentstate->params, "|");
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
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	if (currentstate->stateentry) {
		LOG4CXX_INFO(personLog, "PersonSM :: static :: Lower time :: "
				<< currentstate->name);
		std::vector<std::string>* parse =
				parseString(currentstate->params, "|");
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
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	if (currentstate->stateentry) {
		LOG4CXX_INFO(personLog, "PersonSM :: static :: Delay :: "
				<< currentstate->name);
		currentstate->time = atoi(currentstate->params.c_str());
		currentstate->counter = 0;
		currentstate->stateentry = false;
	}
	currentstate->counter++;
	if (currentstate->counter > currentstate->time) {
		currentstate->statefinished = true;
	}
}

// given a %, roll a random between 0 and 100.  If less than %, advance to given state 1
// otherwise state 2
int PersonSM::choice(stateargs_t* currentstate) {
	if (currentstate->stateentry) {
		LoggerPtr personLog(Logger::getLogger("PersonSM"));
		LOG4CXX_INFO(personLog, "PersonSM :: static :: Choice "
				<< currentstate->name);
		std::vector<std::string>* parse =
				parseString(currentstate->params, "|");
		int pct = atoi((*parse)[0].c_str());
		// choose random between 0 and 100
		RandomInt r(0, 100);
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

int PersonSM::end(stateargs_t* currentstate) {
	if (currentstate->stateentry) {
		LoggerPtr personLog(Logger::getLogger("PersonSM"));
		LOG4CXX_INFO(personLog, "PersonSM :: static :: END ");
		currentstate->stateentry = false;
	}
	// do nothing
}

int PersonSM::timedWait(stateargs_t* currentstate) {
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	if (currentstate->stateentry) {
		LOG4CXX_INFO(personLog, "PersonSM :: static :: timed wait :: "
				<< currentstate->name);
		currentstate->time = atoi(currentstate->params.c_str());
		currentstate->counter = 0;
		currentstate->stateentry = false;
	}
	House* house = House::getInstance();
	Clock* clock = dynamic_cast<Clock *> (house->getModel("Clock"));
	if (clock != NULL) {
		if (clock->getMinutesSinceMidnight() > currentstate->time) {
			currentstate->statefinished = true;
		}
	} else {
		// no clock!
		currentstate->statefinished = true;
		LOG4CXX_ERROR(personLog,
				"PersonSM :: static :: timed wait :: No clock model!");
	}
}

int PersonSM::dataChoice(stateargs_t* currentstate) {
	if (currentstate->stateentry) {
		LoggerPtr personLog(Logger::getLogger("PersonSM"));
		LOG4CXX_INFO(personLog, "PersonSM :: static :: Choice "
				<< currentstate->name);
		std::vector<std::string>* parse =
				parseString(currentstate->params, "|");
		House* inst = House::getInstance();
		std::string mdlN = (*parse)[0];
		int dat = atoi((*parse)[1].c_str());
		Model *mdlPtr = inst->getModel(mdlN);
		if (mdlPtr == NULL) {
			// wtf couldn't find model

		} else {
			int mdlDat = mdlPtr->getData();
			// choose random between 0 and 100
			if (dat == mdlDat) {
				currentstate->next = (*parse)[2];
			} else {
				currentstate->next = (*parse)[3];
			}
		}
		delete parse;
	}
	currentstate->statefinished = true;
}

int PersonSM::earlierTimeChoice(stateargs_t* currentstate) {
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	if (currentstate->stateentry) {
		LOG4CXX_INFO(personLog, "PersonSM :: time choice :: "
				<< currentstate->name);
		std::vector<std::string>* parse =
				parseString(currentstate->params, "|");
		House* house = House::getInstance();
		int time = atoi((*parse)[0].c_str());
		Clock* clock = (Clock*) (house->getModel("Clock"));
		if (clock == NULL) {
			// wtf couldn't find model

		} else {
			int clockTime = clock->getMinutesSinceMidnight();
			// choose random between 0 and 100
			if (clockTime < time) {
				currentstate->next = (*parse)[1];
			} else {
				currentstate->next = (*parse)[2];
			}
		}
		delete parse;
	}
	currentstate->statefinished = true;
}

int PersonSM::laterTimeChoice(stateargs_t* currentstate) {
	LoggerPtr personLog(Logger::getLogger("PersonSM"));
	if (currentstate->stateentry) {
		LOG4CXX_INFO(personLog, "PersonSM :: time choice :: "
				<< currentstate->name);
		std::vector<std::string>* parse =
				parseString(currentstate->params, "|");
		House* house = House::getInstance();
		int time = atoi((*parse)[0].c_str());
		Clock* clock = (Clock*) (house->getModel("Clock"));
		if (clock == NULL) {
			// wtf couldn't find model

		} else {
			int clockTime = clock->getMinutesSinceMidnight();
			// choose random between 0 and 100
			if (clockTime > time) {
				currentstate->next = (*parse)[1];
			} else {
				currentstate->next = (*parse)[2];
			}
		}
		delete parse;
	}
	currentstate->statefinished = true;
}
