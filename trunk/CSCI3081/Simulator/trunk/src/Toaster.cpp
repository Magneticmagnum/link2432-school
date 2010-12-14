/*
 * Toaster.cpp
 *
 *  Created on: Nov 11, 2010
 *      Author: Maya Pevzner
 */
#include "Toaster.h"

Toaster::Toaster() {
	powerOff_ = 0;
	powerOn_ = 0.9;
	running_ = false;
	scheduler_ = NULL;
	stateChanged_ = true;
	ticks_ = 0;
	std::string name = "DefaultToaster";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Toaster instance created.");
}

Toaster::~Toaster() {
}

void Toaster::configure(PropertyTable* props) {
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	setName((*props)["name"]);
}

void Toaster::tick() {
	logPower();
	if (running_) {
		logOn();
		if (ticks_ < 2) {
			ticks_++;
		} else {
			running_ = false;
			stateChanged_ = true;
			logOff();
			ticks_ = 0;
		}
	} else {
		logOff();
	}
	energy_ += getPower();
}

double Toaster::getPower() {
	if (running_) {
		power_ = powerOn_;
	}
	else {
		power_ = powerOff_;
	}
	return power_;
}

int Toaster::getTicks() {
	return ticks_;
}

bool Toaster::isRunning() {
	return running_;
}

void Toaster::activate() {
	running_ = true;
	stateChanged_ = true;
}
