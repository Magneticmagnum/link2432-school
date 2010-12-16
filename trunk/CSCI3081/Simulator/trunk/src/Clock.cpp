/*
 * Clock.cpp
 *
 *  Created on: Dec 15, 2010
 *
 *  This class forwards all time related get-events to scheduler.  This is wrapped so we don't have
 *  to expose the scheduler's mutator methods to models
 *      Author: Joe
 */

#include "Clock.h"
#include "Scheduler.h"

Clock::Clock() {
	// TODO Auto-generated constructor stub
	std::string name = "DefaultClock";
	setName(name);
	scheduler_ = NULL;
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Clock created.");
}

Clock::~Clock() {
	// TODO Auto-generated destructor stub
}

void Clock::tick() {
	Model::tick();

}

int Clock::getTime() {
	return scheduler_->getTime();
}

int Clock::getMinutesSinceMidnight() {
	return scheduler_->getMinutesSinceMidnight();
}

int Clock::getMinute() {
	return scheduler_->getMinute();
}

int Clock::getHours() {
	return scheduler_->getHours();
}

int Clock::getDay() {
	return scheduler_->getDay();
}

std::string Clock::getFormattedTime() {
	return scheduler_->getFormattedTime();
}

void Clock::activate(std::string args) {
	// nothing to activate
	notifyStateChanged_ = true;
}

void Clock::configure(PropertyTable* props) {
	// nothing to configure
}
void Clock::registerScheduler(Scheduler *sched) {
	scheduler_ = sched;
}
