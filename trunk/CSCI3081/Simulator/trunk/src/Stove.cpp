/*
 * Stove.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: nguyen, Maya Pevzner
 */

#include "Stove.h"

Stove::Stove() {
	powerOn_ = 1.4;
	powerOff_ = 0;
	running_ = false;
	scheduler_ = NULL;
	stateChanged_ = true;
	energy_ += getPower();
	// Make sure burners are off
	for (int i = 0; i < 4; i++) {
		burners_[i] = 0;
		cookTime_[i] = 0;
	}
	std::string name = "DefaultStove";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Stove instance created.");
}

Stove::~Stove() {
}

void Stove::configure(PropertyTable* props) {
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	setName((*props)["name"]);
}

void Stove::tick() {
	logPower();
	energy_ += getPower();
	if (running_) {
		logBurners();
		logOn();
		int sum = 0;
		for (int i = 0; i < 4; i++) {
			if (burners_[i] != 0) {
				if (cookTime_[i] != 0) {
					cookTime_[i]--;
				} else {
					burners_[i] = 0;
					stateChanged_ = true;
					logBurners();
				}
			}
			sum += burners_[i];
		}
		if (sum = 0) {
			running_ = false;
			stateChanged_ = true;
			logOff();
		}
	} else {
		logOff();
	}
}

double Stove::getPower() {
	power_ = 0;
	if (running_) {
		// Go through each burner and increment their power based on power settings
		for (int i = 0; i < 4; i++) {
			// "High" uses full power
			if (burners_[i] == 3) {
				power_ += powerOn_;
			}
			// "Medium" uses 50% power
			if (burners_[i] == 2) {
				power_ += powerOn_ * 0.5;
			}
			// "Low" uses 25% power
			if (burners_[i] == 1) {
				power_ += powerOn_ * 0.25;
			}
		}
	} else {
		power_ = powerOff_;
	}
	return power_;
}

bool Stove::isRunning() {
	return running_;
}

void Stove::activate(int b1, int c1, int b2, int c2, int b3, int c3, int b4,int c4) {
	// Store each burner's power level in an int array
	burners_[0] = b1;
	burners_[1] = b2;
	burners_[2] = b3;
	burners_[3] = b4;
	cookTime_[0] = c1;
	cookTime_[1] = c2;
	cookTime_[2] = c3;
	cookTime_[3] = c4;
	running_ = true;
	stateChanged_ = true;
}

void Stove::deactivate() {
	burners_[0] = 0;
	burners_[1] = 0;
	burners_[2] = 0;
	burners_[3] = 0;
	cookTime_[0] = 0;
	cookTime_[1] = 0;
	cookTime_[2] = 0;
	cookTime_[3] = 0;
	running_ = false;
	stateChanged_ = true;
}

void Stove::logBurners() {
	if (stateChanged_ == true) {
		LoggerPtr log(Logger::getLogger("Stove"));
		LOG4CXX_INFO(log, getName() << " :: Burner 1: " << burners_[0] << ", Burner 2: " << burners_[1] << ", Burner 3: " << burners_[2] << ", Burner 4: " << burners_[3]);
	}
}
