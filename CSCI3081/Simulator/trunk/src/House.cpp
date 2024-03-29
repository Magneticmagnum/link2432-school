/*
 * House.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: Joe Houlton
 */

#include "ConfigFile.h"
#include "House.h"
#include "Scheduler.h"
#include <string>
#include <log4cxx/logger.h>

using log4cxx::Logger;
using log4cxx::LoggerPtr;

House * House::instance_ = (House *) NULL;

void House::loadConfig(string config, Scheduler &sched) {
	cfg = new ConfigFile(config);
	// create all new models
	map<string, PropertyTable*>::iterator begin = cfg->begin();
	map<string, PropertyTable*>::iterator end = cfg->end();
	while(begin != end) {
		PropertyTable* tbl = begin->second;
		std::string type = (*tbl)["type"];
		std::string name = (*tbl)["name"];
		Model* mdl = createModel(type, name);
		if (mdl != NULL) {
			// now each model can be configured correctly
			mdl->configure(tbl);
			sched.registerModel(mdl);
		} else {
			LoggerPtr log(Logger::getLogger("House"));
			LOG4CXX_ERROR(log, " House :: createModel(" << type << ", " << name << ") returned NULL!");
		}
		begin++;
	}
}

House* House::getInstance() {
	if (instance_ == NULL) {
		instance_ = new House();
	}
	return instance_;
}

Model* House::getModel(std::string key) {
	return modelMap_[key];
}

void House::putModel(std::string key, Model *model) {
	modelMap_.insert(std::make_pair(key, model));
}

Model* House::createModel(std::string type, std::string name) {
	factory f = factories_[type];
	if (f != NULL) {
		Model* m = f();
		House::putModel(name, m);
		return m;
	}
	return NULL;
}

void House::registerModelFactory(std::string type, factory f) {
	factories_.insert(std::make_pair(type, f));
}

void House::clear() {
	modelMap_.clear();
}
