/*
 * Model.cpp
 *
 *  Created on: Oct 8, 2010
 *      Author: Maya Pevzner
 */

#include <iostream>
#include "Model.h"
#include "ConfigFile.h"
using namespace std;

Model::Model() {
	power_ = 0;
	energy_ = 0;
	stateChanged_ = true;
	notifyStateChanged_ = true;
	scheduler_ = NULL;

}

Model::~Model() {
}

ostream& operator<<(ostream& os, Model& m) {
	os << m.getName() << ": power usage = " << m.getPower()
			<< " kW, Energy usage = " << m.getEnergy() << " kWh." << endl;
	return os;
}

Model::operator int() {
	return (int) getEnergy();
}

Model::operator double() {
	return (double) getEnergy();
}

void Model::configure(PropertyTable* table) {
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_ERROR(
			log,
			getName()
					<< " :: configure() method was called by Model instead of by Model subclass.");
}

void Model::setName(std::string &other) {
	name_ = other;
}

const std::string Model::getName() {
	return name_;
}

double Model::getPower() {
	return power_;
}

double Model::getEnergy() {
	// Since power (kW) is added to energy at each minute, need to divide by 60 to make energy units kW hours (instead of kW minutes).
	return energy_ / 60.0;
}

void Model::tick() {
	if (notifyStateChanged_)
		notifyStateChanged_ = false;
}

int Model::getData() {
	return 0;
}
void Model::setScheduler(Scheduler *sched) {
	scheduler_ = sched;
}

Scheduler* Model::getScheduler() {
	return scheduler_;
}

bool Model::getStateChanged() {
	return stateChanged_;
}

bool Model::getNotifyState() {
	return notifyStateChanged_;
}

void Model::activate(std::string args) {
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_ERROR(
			log,
			getName()
					<< " :: activate() method was called by Model instead of by Model subclass.");
}

void Model::logOn() {
	if (stateChanged_ == true) {
		LoggerPtr log(Logger::getLogger(getName()));
		if (scheduler_) {
			// Write INFO message to log saying it is turned on and giving the time & temperature.
			LOG4CXX_INFO(log, getName() << " :: ON at "
					<< scheduler_->getFormattedTime());
		} else {
			LOG4CXX_DEBUG(
					log,
					getName() << " :: logOn: " << getName()
							<< " has not been associated with a Scheduler. (This message will also appear for test case instances of Models.)");
		}
	}
	stateChanged_ = false;
}

void Model::logOff() {
	if (stateChanged_ == true) {
		LoggerPtr log(Logger::getLogger("WaterHeater"));
		if (scheduler_) {
			LOG4CXX_INFO(log, getName() << " :: OFF at "
					<< scheduler_->getFormattedTime());
		} else {
			LOG4CXX_DEBUG(
					log,
					getName() << " :: logOff: " << getName()
							<< " has not been associated with a Scheduler. (This message will also appear for test case instances of Models.)");
		}
		LOG4CXX_INFO(log, getName() << " :: Energy use: " << getEnergy()
				<< " kWh");
	}
	stateChanged_ = false;
}

void Model::logPower() {
	LoggerPtr log(Logger::getLogger(getName()));
	if (scheduler_) {
		if (stateChanged_ == true) {
			LOG4CXX_INFO(log, getName() << " :: Instantaneous power use: "
					<< getPower() << " kW");
		} else {
			LOG4CXX_DEBUG(log, getName() << " :: Instantaneous power use: "
					<< getPower() << " kW");
		}
	} else {
		LOG4CXX_DEBUG(
				log,
				getName() << " :: logPower: " << getName()
						<< " has not been associated with a Scheduler. (This message will also appear for test case instances of Models.)");
	}
}
