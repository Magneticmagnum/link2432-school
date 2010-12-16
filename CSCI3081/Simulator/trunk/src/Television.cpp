/*
 * Television.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: nguyen, Maya Pevzner
 */

#include "Television.h"

Television::Television() {
	// Initialize running state.
	powerOff_ = 0.01; // Default: When off, it uses 10 W, or 0.01 kW.
	powerOn_ = 0.18; // Default: When running, it uses 180 W, or 0.18 kW.
	running_ = false;
	scheduler_ = NULL;
	stateChanged_ = true;
	std::string name = "DefaultTelevision";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Television instance created.");
}

Television::~Television() {
}

void Television::configure(PropertyTable* props) {
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	setName((*props)["name"]);
}

void Television::tick() {
	Model::tick();
	logPower();
	energy_ += getPower();
	if (running_) {
		logOn();
	} else {
		logOff();
	}
}

double Television::getPower() {
	if (running_) {
		power_ = powerOn_;
	} else {
		power_ = powerOff_;
	}
	return power_;
}

bool Television::isRunning() {
	return running_;
}

void Television::activate(std::string args) {
	if (args == "on") {
		running_ = true;
	} else if (args == "off") {
		running_ = false;
	}
	stateChanged_ = true;
}
