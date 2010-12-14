/*
 * Oven.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: Maya Pevzner
 */
#include "Oven.h"

Oven::Oven() {
	powerPreheat_ = 7.5;
	powerOn_ = 2.5;
	powerOff_ = 0;
	running_ = false;
	scheduler_ = NULL;
	stateChanged_ = true;
	ticks_ = 0;
	std::string name = "DefaultOven";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Oven instance created.");
}

Oven::~Oven() {
}

void Oven::configure(PropertyTable* props) {
	powerPreheat_ = atof((*props)["powerPreheat"].c_str());
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	setName((*props)["name"]);
}

void Oven::tick() {
	logPower();
	energy_ += getPower();
	if (running_) {
		logOn();
		if (ticks_ < (duration_ + 10)) {
			if (ticks_ == 10) {
				stateChanged_ = true;
			}
			ticks_++;
		} else {
			running_ = false;
			ticks_ = 0;
			stateChanged_ = true;
			logOff();
		}
	} else {
		logOff();
	}
}

double Oven::getPower() {
	// When running, it uses 7.5 kW for 10 min, and then switches to 2.5 kW for duration_.
	if ((running_) && (ticks_ < 10)) {
		power_ = powerPreheat_;
	} else if ((running_) && (ticks_ < (duration_ + 10))) {
		power_ = powerOn_;
	}
	// When not running, Oven uses no power.
	else {
		power_ = powerOff_;
	}
	return power_;
}

int Oven::getTicks() {
	return ticks_;
}

bool Oven::isRunning() {
	return running_;
}

void Oven::activate(int duration) {
	running_ = true;
	stateChanged_ = true;
	duration_ = duration;
}
