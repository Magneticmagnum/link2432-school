/*
 * HairDryer.cpp
 *
 *  Created on: Nov 23, 2010
 *      Author: Maya Pevzner
 */

#include "HairDryer.h"

HairDryer::HairDryer() {
	powerHi_ = 1.0;
	powerLo_ = 0.5;
	running_ = false;
	scheduler_ = NULL;
	stateChanged_ = true;
	std::string name = "DefaultHairDryer";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: HairDryer instance created.");
}

HairDryer::~HairDryer() {
}

void HairDryer::configure(PropertyTable* props) {
	powerHi_ = atof((*props)["powerHi"].c_str());
	powerLo_ = atof((*props)["powerLo"].c_str());
	setName((*props)["name"]);
}

void HairDryer::tick() {
	logPower();
	energy_ += getPower();
	if (running_) {
		logOn();
	} else {
		logOff();
	}
}

double HairDryer::getPower() {
	// When running, it uses 1 kW on high, 0.5 kW on low.
	if (running_) {
		if (high_ == true) {
			power_ = powerHi_;
		} else {
			power_ = powerLo_;
		}
	} else {
		power_ = 0.0;
	}
	return power_;
}

bool HairDryer::isRunning() {
	return running_;
}

void HairDryer::activate(bool high) {
	running_ = true;
	stateChanged_ = true;
	high_ = high;
}

void HairDryer::deactivate() {
	running_ = false;
	stateChanged_ = true;
}
