/*
 * Microwave.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: Maya Pevzner
 */
#include "Microwave.h"
#include "RandomInt.h"
#include "Stringutil.h"

Microwave::Microwave() {
	powerOff_ = 0;
	powerOn_ = 1.5;
	r_ = RandomInt(40,120);
	running_ = false;
	stateChanged_ = true;
	scheduler_ = NULL;
	ticks_ = 0;
	std::string name = "DefaultMicrowave";
	setName(name);
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Microwave instance created.");
}

Microwave::~Microwave() {
}

void Microwave::configure(PropertyTable* props) {
	powerOn_ = atof((*props)["powerOn"].c_str());
	powerOff_ = atof((*props)["powerOff"].c_str());
	setName((*props)["name"]);
}

void Microwave::tick() {
	logPower();
	energy_ += min(60, runTimeLeft_) * getPower();
	if (running_ == true) {
		logRunTimeLeft();
		logOn();
		if (runTimeLeft_ > 60) {
			ticks_++;
			runTimeLeft_ -= 60;
		} else {
			ticks_ = 0;
			runTimeLeft_ = 0;
			running_ = false;
			stateChanged_ = true;
			logOff();
		}
	} else {
		ticks_ = 0;
		logOff();
	}
}

double Microwave::getPower() {
	if ((running_ == true) & (runTimeLeft_ != 0)) {
		power_ = powerOn_;
	}
	else {
		power_ = powerOff_;
	}
	return power_;
}

int Microwave::getTicks() {
	return ticks_;
}

bool Microwave::isRunning() {
	return running_;
}

void Microwave::activate(std::string args) {
	running_ = true;
	std::vector<std::string> *parse = parseString2List(args, ",");
	if ((*parse)[0].c_str() == "") {
		runTimeLeft_ = r_();
	} else {
		runTimeLeft_ = atoi((*parse)[0].c_str());
	}
	stateChanged_ = true;
	delete parse;
}

void Microwave::logRunTimeLeft() {
	if (stateChanged_ == true) {
		LoggerPtr log(Logger::getLogger(getName()));
		LOG4CXX_INFO(log, getName() << " :: Run time set to: " << runTimeLeft_ << " seconds");
	}
}
