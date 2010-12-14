/*
 * WaterHeater.cpp
 *
 *  Created on: Nov 5, 2010
 *      Author: Nguyen, Maya Pevzner
 */
#include <iostream>
#include <math.h>
#include "WaterHeater.h"
using namespace std;

WaterHeater::WaterHeater() {
	capacity_ = 200;
	powerOn_ = 4.5;
	powerOff_ = 0;
	running_ = false;
	scheduler_ = NULL;
	stateChanged_ = true;
	temperature_ = 50;
	ticks_ = 0;
	std::string name = "DefaultWaterHeater";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: WaterHeater instance created.");
}

WaterHeater::~WaterHeater() {
}

void WaterHeater::configure(PropertyTable* props) {
	capacity_ = atof((*props)["capacity"].c_str());
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	setName((*props)["name"]);
}

WaterHeater& operator-=(WaterHeater& wh, double amt) {
	wh.drawWater(amt, wh.getTemperature());
	return wh;
}

void WaterHeater::drawWater(double quantity, double temp) {
	double water = quantity * (temp - 7) / (temperature_ - 7);
	// Update water heater's temperature, taking into account the 7-deg C water entering.
	temperature_ = ((temperature_ * (capacity_ - water)) + (7 * water)) / capacity_;
}

void WaterHeater::tick() {
	ticks_++;
	// Water is always losing heat due to imperfect thermal insulation around the water heater.
	temperature_ -= ((0.576 * pow((capacity_/200),(2.0/3)))/ capacity_);
	logPower();
	if (running_) {
		temperature_ += ((powerOn_ * 60) / (4.132 * capacity_));
		logOn();
		// If temperature reaches 50 deg C, turn water heater off
		if (temperature_ >= 50) {
			running_ = false;
			stateChanged_ = true;
			logTemp();
			logOff();
		}
	} else {
		logOff();
		// If temperature falls below 49 deg C, turn water heater on.
		if (temperature_ < 49) {
			running_ = true;
			stateChanged_ = true;
			logTemp();
			logOn();
			temperature_ += ((powerOn_ * 60.0) / (4.132 * capacity_));

		}
	}
	energy_ += getPower();
}

double WaterHeater::getPower() {
	if (running_) {
		//power_ = powerOn_;
		power_ = 4.5;
	}
	else {
		power_ = powerOff_;
	}
	return power_;
}

double WaterHeater::getTemperature() {
	return temperature_;
}

int WaterHeater::getTicks() {
	return ticks_;
}

bool WaterHeater::isRunning() {
	return running_;
}

void WaterHeater::logTemp() {
	if (stateChanged_ == true) {
		LoggerPtr log(Logger::getLogger("WaterHeater"));
		LOG4CXX_INFO(log, getName() << " :: Water temperature: " << getTemperature() << " C");
	}
}
