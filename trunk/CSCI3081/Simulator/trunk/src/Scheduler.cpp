/*
 * Scheduler.cpp
 *
 *  Created on: Oct 8, 2010
 *      Author: Maya Pevzner
 */

#include "Scheduler.h"
#include "Model.h"
#include <log4cxx/logger.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

using log4cxx::Logger;
using log4cxx::LoggerPtr;

Scheduler::Scheduler() :
	models_() {
	time_ = 0;
	LoggerPtr schedulerLog(Logger::getLogger("Scheduler"));
	LOG4CXX_DEBUG(schedulerLog, "Scheduler :: Scheduler instance created.");
}

Scheduler::~Scheduler() {
}

bool Scheduler::registerModel(Model *thing) {
	models_.push_back(thing);
	thing->setScheduler(this);
	return true;
}

int Scheduler::getTime() {
	return time_;
}

int Scheduler::getMinutesSinceMidnight() {
	return time_ % 1440;
}

int Scheduler::getMinute() {
	return time_ % 60;
}

int Scheduler::getHours() {
	return time_ / 60 - getDay() * 24;
}

int Scheduler::getDay() {
	return time_ / 1440;
}

std::string Scheduler::getFormattedTime() {
	std::stringstream s_;
	if ((getMinute() < 10) && (getHours() < 10)) {
		s_ << "Day: " << getDay() << ", Time: 0" << getHours() << ":0"
				<< getMinute();
	} else if ((getMinute() < 10) && (getHours() >= 10)) {
		s_ << "Day: " << getDay() << ", Time: " << getHours() << ":0"
				<< getMinute();
	} else if ((getMinute() >= 10) && (getHours() < 10)) {
		s_ << "Day: " << getDay() << ", Time: 0" << getHours() << ":"
				<< getMinute();
	} else {
		s_ << "Day: " << getDay() << ", Time: " << getHours() << ":"
				<< getMinute();
	}
	return s_.str();
}

void Scheduler::run(int maxCount) {
	LoggerPtr schedulerLog(Logger::getLogger("Scheduler"));
	// Loop through ticks until maxCount is reached.
	LOG4CXX_INFO(schedulerLog, "Scheduler :: run :: " << maxCount << std::endl);
	for (int i = 0; i < maxCount; i++) {
		time_++;
		size_t nModels = models_.size();
		sumPower_ = 0;
		// Loop through registered Models, adding a tick for each Model and summing power & energy usage.
		for (size_t i = 0; i < nModels; i++) {
			Model* tmp = models_.at(i);
			tmp->tick();
			sumPower_ += tmp->getPower();
		}
		sumPower_ = sumPower_;
		sumEnergy_ += sumPower_;
		// Log current day/time, instantaneous power usage, and cumulative energy usage.
		LOG4CXX_INFO(schedulerLog, "Scheduler :: " << getFormattedTime());
		LOG4CXX_INFO(schedulerLog, "Scheduler :: Instantaneous power use: "
				<< fixed << setprecision(4) << getSumPower() << " kW");
		LOG4CXX_INFO(schedulerLog, "Scheduler :: Cumulative energy use: "
				<< fixed << setprecision(4) << getSumEnergy() << " kWh"
				<< std::endl);
	}
}

std::vector<Model*> Scheduler::getModels() {
	return models_;
}

double Scheduler::getSumPower() {
	return sumPower_;
}

double Scheduler::getSumEnergy() {
	return sumEnergy_;
}

