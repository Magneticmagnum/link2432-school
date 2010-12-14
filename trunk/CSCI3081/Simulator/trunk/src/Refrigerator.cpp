/*
 * Refrigerator.cpp
 *
 *  Created on: Oct 8, 2010
 *      Author: Maya Pevzner
 */
#include "Refrigerator.h"
#include <boost/random.hpp>

Refrigerator::Refrigerator() {
	// Initialize running state and ticks.
	cooking_ = false;
	dutyCycleCooking_ = 0.5;
	dutyCycleNotCooking_ = 0.3;
	onTime_ = 3;
	powerOn_ = 0.3;
	powerOff_ = 0;
	scheduler_ = NULL;
	stateChanged_ = true;
	// Select random point in cycle
	boost::mt19937 generator(time(NULL));
	double rand = boost::uniform_real<double>(0, 1)(generator);
	if (rand < 0.5) {
		running_ = true;
		ticks_ = (int) boost::uniform_real<double>(0, onTime_)(generator);
	} else {
		running_ = false;
		ticks_ = (int) boost::uniform_real<double>(onTime_+1, onTime_/dutyCycleNotCooking_)(generator);
	}
	std::string name = "DefaultRefrigerator";
	setName(name);
	LoggerPtr refrigeratorLog(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(refrigeratorLog, getName() << " :: Refrigerator instance created.");
}

Refrigerator::~Refrigerator() {
}

void Refrigerator::configure(PropertyTable* props) {
	onTime_ = atoi((*props)["onTime"].c_str());
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	dutyCycleCooking_ = atof((*props)["dutyCycleCooking"].c_str());
	dutyCycleNotCooking_ = atof((*props)["dutyCycleNotCooking"].c_str());
	setName((*props)["name"]);
}

void Refrigerator::tick() {
	ticks_++;
	energy_ += getPower();
	logPower();
	// When undisturbed (nobody cooking), it has a duty cycle (ratio of on time to total time) of dutyCycleNotCooking_.
	if (cooking_ == false) {
		if (running_ == true) {
			if (ticks_ != (int)onTime_) {
				logOn();
			}
			// At onTime_ ticks, refrigerator turns off.
			else {
				running_ = false;
				stateChanged_ = true;
				logOff();
			}
		} else {
			if (ticks_ != (int)(onTime_/dutyCycleNotCooking_)) {
				logOff();
			}
			// At onTime_/dutyCycleNotCooking_ ticks, refrigerator turns on.
			else {
				running_ = true;
				stateChanged_ = true;
				// Start new cycle, reset internal ticks to 0.
				ticks_ = 0;
				logOn();
			}
		}
	}
	// When a Person is cooking, it has a duty cycle of dutyCycleCooking_. Each on cycle is onTime_ minutes.
	else {
		if (running_ == true) {
			if (ticks_ != (int)onTime_) {
				logOn();
			}
			// At onTime_ ticks, refrigerator turns off.
			else {
				running_ = false;
				stateChanged_ = true;
				logOff();
			}
		} else {
			if (ticks_ != (int)(onTime_/dutyCycleCooking_)) {
				logOff();
			}
			// At onTime_/dutyCycleCooking_ ticks, refrigerator turns on.
			else {
				running_ = true;
				stateChanged_ = true;
				// Start new cycle, reset internal ticks to 0.
				ticks_ = 0;
				logOn();
			}
		}
	}
}

double Refrigerator::getPower() {
	// When running, refrigerator uses powerOn_ kW.
	if (running_) {
		power_ = powerOn_;
	}
	// When not running, refrigerator uses no power.
	else {
		power_ = 0;
	}
	return power_;
}

int Refrigerator::getTicks() {
	return ticks_;
}

bool Refrigerator::isRunning() {
	return running_;
}

bool Refrigerator::getCookingState() {
	return cooking_;
}

bool Refrigerator::setCookingState(bool state) {
	cooking_ = state;
	ticks_ = 0;
	running_ = true;
	stateChanged_ = true;
}
