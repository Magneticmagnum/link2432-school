/*
 * Xbox.cpp
 *
 *  Created on: Nov 23, 2010
 *      Author: Maya Pevzner
 */

#include "Xbox.h"

Xbox::Xbox() {
	powerOff_ = 0.002;
	powerOn_ = 0.165;
	running_ = false;
	scheduler_ = NULL;
	stateChanged_ = true;
	std::string name = "DefaultXbox";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Xbox instance created.");
}

Xbox::~Xbox() {
}

void Xbox::configure(PropertyTable* props) {
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	setName((*props)["name"]);
}

void Xbox::tick() {
	logPower();
	energy_ += getPower();
	if (running_) {
		logOn();
	} else {
		logOff();
	}
}

double Xbox::getPower() {
	if (running_) {
		power_ = powerOn_;
	} else {
		power_ = powerOff_;
	}
	return power_;
}

bool Xbox::isRunning() {
	return running_;
}

void Xbox::activate() {
	running_ = true;
	stateChanged_ = true;
}

void Xbox::deactivate() {
	running_ = false;
	stateChanged_ = true;
}
