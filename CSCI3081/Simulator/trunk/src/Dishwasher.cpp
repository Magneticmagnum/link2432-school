/*
 * Dishwasher.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: Nguyen, Maya Pevzner
 */

#include "Dishwasher.h"
#include "WaterHeater.h"
#include "House.h"
#include "Stringutil.h"
#include "Clock.h"

Dishwasher::Dishwasher() {
	running_ = false;
	powerOn_ = 2.0;
	powerOff_ = 0.004;
	stateChanged_ = true;
	daysSinceLast_ = 0;
	currentDay_ = 0;
	scheduler_ = NULL;
	ticks_ = 0;
	std::string name = "DefaultDishwasher";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Dishwasher instance created.");
}

Dishwasher::~Dishwasher() {
}

void Dishwasher::configure(PropertyTable* props) {
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	setName((*props)["name"]);
}

void Dishwasher::tick() {
	Model::tick();
	House* inst = House::getInstance();
	Clock* clk = dynamic_cast<Clock *> (inst->getModel("Clock"));
	logPower();
	if (running_) {
		if (ticks_ < 20) {
			House *ptr = House::getInstance();
			WaterHeater *wh = dynamic_cast<WaterHeater *> (ptr->getModel(
					"WaterHeater"));
			if (wh != NULL) {
				// Draw (22/20 = 1.1) liters of water, at the current temperature of the water in the WaterHeater, from the WaterHeater.
				wh->drawWater(1.1, wh->getTemperature());
			} else {
				LoggerPtr log(Logger::getLogger("Dishwasher"));
				LOG4CXX_ERROR(
						log,
						"Dishwasher :: Dishwasher unable to locate a WaterHeater instance to get the water it needs.");
			}
			ticks_++;
			logOn();
		} else if (ticks_ < 45) {
			ticks_++;
			logOn();
		} else {
			running_ = false;
			ticks_ = 0;
			stateChanged_ = true;
			notifyStateChanged_ = true;
			logOff();
		}
	} else {
		logOff();
		if (clk->getDay() > currentDay_) {
			daysSinceLast_++;
			currentDay_ = clk->getDay();
		}
	}
	energy_ += getPower();
}

double Dishwasher::getPower() {
	// When running, it uses 2 kW when on for 45 minutes. When off, it uses 4 W, or 0.004 kW.
	if (running_) {
		power_ = powerOn_;
	} else {
		power_ = powerOff_;
	}
	return power_;
}

int Dishwasher::getTicks() {
	return ticks_;
}

int Dishwasher::getData() {
	return daysSinceLast_;
}

bool Dishwasher::isRunning() {
	return running_;
}

void Dishwasher::activate(std::string args) {
	running_ = true;
	ticks_ = 0;
	stateChanged_ = true;
	notifyStateChanged_ = true;
	daysSinceLast_ = 0;
}
