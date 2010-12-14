/*
 * Person.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: Maya Pevzner
 */
#include "Person.h"
#include "RandomInt.h"
#include "House.h"
#include "Refrigerator.h"
#include "WaterHeater.h"
#include "Dishwasher.h"
#include "Microwave.h"
#include "Oven.h"
#include "Stove.h"
#include "Television.h"
#include "Toaster.h"

Person::Person() {
	ticks_ = 0;
	bedTime_ = 0;
	cookTimeLeft_ = 0;
	daysSinceDishwasher_ = 0;
	getHomeTime_ = 0;
	showerTimeLeft_ = 0;
	wakeTime_ = 0;
	atBreakfast_ = false;
	atDinner_ = false;
	atHome_ = true;
	awake_ = false;
	bathing_ = false;
	cooking_ = false;
	fastDinner_ = false;
	justGotHome_ = false;
	justFinishedDinner_ = false;
	justFinishedShower_ = false;
	justWokeUp_ = false;
	showering_ = false;
	vegOut_ = false;
	watchingTv_ = false;
	breakfastDecide_ = 1;
	showerDecide_ = 1;
	ovenDecide_ = 1;
	bathDecide_ = 1;
	bathTime_ = 0;
	std::string name = "Person";
	setName(name);
	scheduler_ = NULL;
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Person instance created.");
}

Person::Person(std::string name) {
	ticks_ = 0;
	bedTime_ = 0;
	cookTimeLeft_ = 0;
	daysSinceDishwasher_ = 0;
	getHomeTime_ = 0;
	showerTimeLeft_ = 0;
	wakeTime_ = 0;
	atBreakfast_ = false;
	atDinner_ = false;
	atHome_ = true;
	awake_ = false;
	bathing_ = false;
	cooking_ = false;
	fastDinner_ = false;
	justGotHome_ = false;
	justFinishedDinner_ = false;
	justFinishedShower_ = false;
	justWokeUp_ = false;
	showering_ = false;
	vegOut_ = false;
	watchingTv_ = false;
	breakfastDecide_ = 1;
	showerDecide_ = 1;
	ovenDecide_ = 1;
	bathDecide_ = 1;
	bathTime_ = 0;
	setName(name);
	scheduler_ = NULL;
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Person instance created.");
}

Person::~Person() {
	LoggerPtr log(Logger::getLogger(getName()));
	LOG4CXX_DEBUG(log, getName() << " :: Person instance destroyed.");
}

void Person::tick() {
	LoggerPtr log(Logger::getLogger(getName()));
	if (scheduler_) {
		House *ptr = House::getInstance();

		// If Person is asleep...
		if (awake_ == false) {
			LOG4CXX_DEBUG(log, getName() << " :: Person is asleep.");
			// At midnight (when hours is no longer 11 and hence hours is less than 7), start counting ticks
			if (scheduler_->getHours() <= 7) {
				// At 0:00, calculate decisions for the day...
				if (ticks_ == 0) {
					// Calculate random wake time between 6:30 and 7:00
					wakeTime_ = getRandomNum(0, 30);
					LOG4CXX_DEBUG(log, getName() << " :: Person's wake time set to " << wakeTime_ << " minutes after 6:30.");
					// 50% probability of taking a shower on a given day
					showerDecide_ = getRandomNum(0,1);
					LOG4CXX_DEBUG(log, getName() << " :: Set decision of whether or not to shower today: getRandomNum(0,1) = " << showerDecide_);
					// 50% probability of using Stove only at breakfast vs. both Stove & Microwave
					breakfastDecide_ = getRandomNum(0,1);
					LOG4CXX_DEBUG(log, getName() << " :: Set decision of whether to use Stove only, or both Stove and Microwave, for breakfast: getRandomNum(0,1) = " << breakfastDecide_);
					// 20% probability of using Oven at dinner
					ovenDecide_ = getRandomNum(1,5);
					LOG4CXX_DEBUG(log, getName() << " :: Set decision of whether to use Oven for dinner: getRandomNum(1,5) = " << ovenDecide_);
					// 20% probability of taking a bath after dinner
					bathDecide_ = getRandomNum(1,5);
					LOG4CXX_DEBUG(log, getName() << " :: Set decision of whether to take a bath after dinner: getRandomNum(1,5) = " << bathDecide_);
					bedTime_ = getRandomNum(0, 30);
					LOG4CXX_DEBUG(log, getName() << " :: Person's bed time set to " << bedTime_ << " minutes after 23:00.");
					getHomeTime_ = getRandomNum(0, 60);
					LOG4CXX_DEBUG(log, getName() << " :: Person's get home time set to " << getHomeTime_ << " minutes after 17:30.");
					bathTime_ = getRandomNum(15, 20);
					LOG4CXX_DEBUG(log, getName() << " :: Person's bath time (if s/he will take bath on this day) set to " << bathTime_ << " minutes.");
				}
				ticks_++;
				LOG4CXX_DEBUG(log, getName() << " :: Person is asleep and ticks_ = " << ticks_);
			}
			// When the current ticks is the wake time, Person wakes up.
			if (ticks_ == wakeTime_+390) {
				ticks_ = 0;
				wakeUp();
				LOG4CXX_DEBUG(log, getName() << " :: Person woke up, awake_ = " << awake_ << ", justWokeUp_ = " << justWokeUp_ <<", and ticks_ = " << ticks_);
			}
		}

		// If Person is awake...
		else {

			// If Person just woke up...
			if (justWokeUp_ == true) {
				// For first 5 minutes after waking up, Person does nothing.
				if (ticks_ < 5) {
					ticks_++;
					LOG4CXX_DEBUG(log, getName() << " :: Person 'just woke up' and ticks_ = " << ticks_);
				}

				// 5 minutes after waking up, the Person takes a shower with a 50 percent probability.
				if (ticks_ == 5) {
					justWokeUp_ = false;
					LOG4CXX_DEBUG(log, getName() << " :: Person no longer 'just woke up', and justWokeUp_ = " << justWokeUp_);
					ticks_ = 0;
					if (showerDecide_ == 1) { // Shower today.
						LOG4CXX_INFO(log, getName() << " :: Person decided to take a shower today.");
						// Showers last 5-10 minutes.
						takeShower(getRandomNum(5, 10));
					} else { // No shower today.
						LOG4CXX_INFO(log, getName() << " :: Person decided not to take a shower today.");
						justFinishedShower_ = true;
						ticks_ = 0;
					}
				}
			}

			// If Person is showering...
			if (showering_ == true) {
				ticks_++;
				LOG4CXX_DEBUG(log, getName() << " :: Person is taking a shower and ticks_ = " << ticks_);
				// Flow rate of the shower head is 8 l/min of water at 40 C.
				WaterHeater *wh = dynamic_cast<WaterHeater *> (ptr->getModel("WaterHeater"));
				LOG4CXX_DEBUG(log, getName() << " :: WaterHeater pointer created.");
				if (wh != NULL) {
					wh->drawWater(8, 40);
					LOG4CXX_DEBUG(log, getName() << " :: Person's shower draws 8 liters of water at 40 C from a water heater.");
				} else {
					LOG4CXX_ERROR(log, getName() << " :: Person is trying to take a shower, but can't find a WaterHeater instance to get the water it needs.");
				}
				showerTimeLeft_--;
				LOG4CXX_DEBUG(log, getName() << " :: Person's showerTimeLeft_ = " << showerTimeLeft_);
				if (showerTimeLeft_ == 0) {
					finishShower();
					LOG4CXX_DEBUG(log, getName() << " :: Person finished taking a shower.");
				}
			}

			// If Person just finished shower (or decided not to)...
			if (justFinishedShower_ == true) {
				// For 15 minutes after finishing showering (or after deciding not to shower), Person does nothing
				if (ticks_ < 15) {
					ticks_++;
					LOG4CXX_DEBUG(log, getName() << " :: Person 'just finished shower' and ticks_ = " << ticks_);
				}
				// 15 minutes later, the person gets breakfast.
				if (ticks_ == 15) {
					justFinishedShower_ = false;
					atBreakfast_ = true;
					ticks_ = 0;
					LOG4CXX_DEBUG(log, getName() << " :: Person is 'at breakfast', atBreakfast_ = " << atBreakfast_ << ", and ticks_ = " << ticks_);
					// Spend 10 minutes "cooking" breakfast
					cook(10);

					// Use Toaster.
					Toaster *toaster = dynamic_cast<Toaster *> (ptr->getModel("Toaster"));
					if (toaster != NULL) {
						toaster->activate();
						LOG4CXX_INFO(log, getName() << " :: Person activated Toaster to make breakfast.");
					} else {
						LOG4CXX_ERROR(log, getName() << " :: Person is trying to make breakfast, but can't find a Toaster instance to use.");
					}

					// Make coffee using Stove, one burner on high for 4 minutes.
					// Use either Stove (one burner on "medium" for 5 minutes") or Microwave.
					Stove *stove = dynamic_cast<Stove *> (ptr->getModel("Stove"));
					if (breakfastDecide_ == 1) { // Use Stove only.
						if (stove != NULL) {
							stove->activate(3, 4, 2, 5, 0, 0, 0, 0);
							LOG4CXX_INFO(log, getName() << " :: Person activated Stove to make breakfast.");
						} else {
							LOG4CXX_ERROR(log, getName() << " :: Person is trying to make breakfast, but can't find a Stove instance to use.");
						}
					} else { // Use Microwave and Stove.
						Microwave *mw = dynamic_cast<Microwave *> (ptr->getModel("Microwave"));
						if (mw != NULL) {
							mw->activate();
							LOG4CXX_INFO(log, getName() << " :: Person activated Microwave to make breakfast.");
						} else {
							LOG4CXX_ERROR(log, getName() << " :: Person is trying to make breakfast, but can't find a Microwave instance to use.");
						}
						if (stove != NULL) {
							stove->activate(3, 4, 0, 0, 0, 0, 0, 0);
							LOG4CXX_INFO(log, getName() << " :: Person activated Stove to make breakfast.");
						} else {
							LOG4CXX_ERROR(log, getName() << " :: Person is trying to make breakfast, but can't find a Stove instance to use.");
						}
					}
					ticks_++;
				}
			}

			// If Person is at breakfast...
			if (atBreakfast_ == true) {
				// Finish breakfast
				if (ticks_ < 10) {
					ticks_++;
					cookTimeLeft_--;
					LOG4CXX_DEBUG(log, getName() << " :: Person is 'at breakfast', ticks_= " << ticks_ << ", and cookTimeLeft_ = " << cookTimeLeft_);
					if (cookTimeLeft_ == 0) {
						finishCooking();
					}
				}
				// Leave the house after breakfast
				else {
					leaveHouse();
				}
			}

			// If Person is not home, figure out if they should come home yet.
			// Person should arrive back between 17:30 and 18:30 (uniform distribution).
			if (atHome_ == false) {
				// At 17:30, start counting ticks
				if (((scheduler_->getHours() == 17) && (scheduler_->getMinute() >= 30)) ||
						((scheduler_->getHours() == 18) && (scheduler_->getMinute() <= 30))) {
					ticks_++;
					if ((scheduler_->getHours() == 17) && (scheduler_->getMinute() == 30)) {
						ticks_ = 0;
					}
					// When the current ticks is the get home time, Person gets home.
					if (ticks_ == getHomeTime_) {
						comeHome();
						LOG4CXX_DEBUG(log, getName() << " :: Person got home, atHome_ = " << atHome_ << ", justGotHome_ = " << justGotHome_ << ", and ticks_ = " << ticks_);
					}
					else {
						LOG4CXX_DEBUG(log, getName() << " :: Person is not home and ticks_ = " << ticks_);
					}
				}
			}

			// If Person just got home...
			if (justGotHome_ == true) {
				justGotHome_ = false;
				atDinner_ = true;
				// Decide on fast dinner (10 minutes cooking) or slow dinner (40 minutes cooking) based on time Person got home
				if (getHomeTime_ > 30) {
					fastDinner_ = true;
					LOG4CXX_INFO(log, getName() << " :: Person go home after 18:00 on this day, so cooking dinner will be fast (10 minutes).");
				} else {
					fastDinner_ = false;
					LOG4CXX_INFO(log, getName() << " :: Person got home before 18:00 on this day, so cooking dinner will be slow (40 minutes).");
				}
				if (fastDinner_ == false) { // Spend 40 minutes "cooking" dinner
					cook(40);
					LOG4CXX_DEBUG(log, getName() << " :: Person will spend 40 minutes cooking dinner.");

					// Use 3 burners on stove, one on "high" for 15 min, one on "medium" for 20 min, and one on "low" for 30 min.
					Stove *stove = dynamic_cast<Stove *> (ptr->getModel("Stove"));
					if (stove != NULL) {
						stove->activate(3, 15, 2, 20, 1, 30, 0, 0);
						LOG4CXX_INFO(log, getName() << " :: Person activated Stove to make dinner.");
					} else {
						LOG4CXX_ERROR(log, getName() << " :: Person is trying to make dinner, but can't find a Stove instance to use.");
					}

					// Possibly (20 percent probability) use the oven.
					if (ovenDecide_ == 1) {
						Oven *oven = dynamic_cast<Oven *> (ptr->getModel("Oven"));
						if (oven != NULL) {
							oven->activate(30);
							LOG4CXX_INFO(log, getName() << " :: Person activated Oven to make dinner.");
						} else {
							LOG4CXX_ERROR(log, getName() << " :: Person is trying to make dinner, but can't find an Oven instance to use.");
						}
					}
				} else { // Spend 10 minutes "cooking" dinner
					cook(10);
					LOG4CXX_DEBUG(log, getName() << " :: Person will spend 10 minutes cooking dinner.");
				}
			}

			// If Person is at dinner, finish dinner.
			if ((atDinner_ == true) && (ticks_ <= 40)) {
				ticks_++;
				cookTimeLeft_--;
				LOG4CXX_DEBUG(log, getName() << " :: Person is cooking dinner, ticks_ = " << ticks_ << ", and cookTimeLeft_ = " << cookTimeLeft_);
				// If fast dinner, use Microwave 3 times.
				if ((fastDinner_ == true) && ((cookTimeLeft_ == 9)
						|| (cookTimeLeft_ == 6) || (cookTimeLeft_ == 3))) {
					Microwave *mw = dynamic_cast<Microwave *> (ptr->getModel("Microwave"));
					if (mw != NULL) {
						mw->activate();
						LOG4CXX_INFO(log, getName() << " :: Person activated Microwave to make dinner.");
					} else {
						LOG4CXX_ERROR(log, getName() << " :: Person is trying to make dinner, but can't find a Microwave instance to use.");
					}
				}
				if (cookTimeLeft_ == 0) {
					ticks_ = 0;
					finishCooking();
					atDinner_ = false;
					justFinishedDinner_ = true;
					LOG4CXX_DEBUG(log, getName() << " :: Person finished cooking dinner, ticks_ = " << ticks_ << ", atDinner_ = " << atDinner_ << ", and justFinishedDinner_ = " << justFinishedDinner_);
				}
			}

			// After cooking dinner is finished...
			if (justFinishedDinner_ == true) {
				// Do nothing for 20 minutes (presumably eat dinner)
				if (ticks_ < 20) {
					ticks_++;
					LOG4CXX_DEBUG(log, getName() << " :: Person is eating (not cooking) dinner and ticks_ = " << ticks_);
				}

				// 20 minutes after cooking dinner is finished...
				if (ticks_ == 20) {
					// Dishwasher may have to be run. If Person cooked dinner using the stove (i.e. if it was a "slow dinner"),
					// or if it has been 3 days since the Dishwasher was run, then the Dishwasher will have to run.
					LOG4CXX_INFO(log, getName() << " :: Person observed that it has been " << daysSinceDishwasher_ << " days since the Dishwasher was run.");
					if ((fastDinner_ == false) || (daysSinceDishwasher_ == 3)) {
						Dishwasher *dw = dynamic_cast<Dishwasher *> (ptr->getModel("Dishwasher"));
						if (dw != NULL) {
							dw->activate();
							LOG4CXX_INFO(log, getName() << " :: Person activated Dishwasher after dinner.");
						} else {
							LOG4CXX_ERROR(log, getName() << " :: Person is trying to wash dishes after dinner, but can't find a Dishwasher instance to use.");
						}
						daysSinceDishwasher_ = 0;
						LOG4CXX_DEBUG(log, getName() << " :: Person observed that it has been " << daysSinceDishwasher_ << " days since the Dishwasher was run.");
					} else {
						daysSinceDishwasher_++;
						LOG4CXX_DEBUG(log, getName() << " :: Person observed that it has been " << daysSinceDishwasher_ << " days since the Dishwasher was run.");
					}

					// Person may decide to take a bath, with 20 percent probability (some days are more stressful than others).
					ticks_ = 0;
					justFinishedDinner_ = false;
					// Possibly (20 percent probability) take a bath.
					if (bathDecide_ == 1) {
						WaterHeater *wh = dynamic_cast<WaterHeater *> (ptr->getModel("WaterHeater"));
						if (wh != NULL) {
							// Baths require 100 l of water at 42 C
							wh->drawWater(100, 42);
							LOG4CXX_DEBUG(log, getName() << " :: Person's bath draws 100 liters of water at 42 C from a water heater.");
						} else {
							LOG4CXX_ERROR(log, getName() << " :: Person is trying to take a bath, but can't find a WaterHeater instance to use.");
						}
						// Tub takes 4 min to fill, and the bath takes 15-20 minutes after the tub is filled.
						takeBath(4+bathTime_);
					}
					// Otherwise, veg out for the rest of the night
					else {
						vegOut_ = true;
					}
				}
			}

			// If Person is taking a bath, finish taking a bath.
			if (bathing_ == true) {
				ticks_++;
				LOG4CXX_DEBUG(log, getName() << " :: Person is taking a bath and ticks_ = " << ticks_);
				if (bathTime_ == ticks_) {
					finishBath();
					LOG4CXX_DEBUG(log, getName() << " :: Person is finished bath and ticks_ = " << ticks_);
				}
			}

			// For the remainder of the evening, until Person goes to bed, it watches the Television to veg out.
			// (The television gets turned off before the person goes to bed.)
			if ((vegOut_ == true) && (watchingTv_ == false)) {
				watchTelevision();
				Television* tv = dynamic_cast<Television *> (ptr->getModel("Television"));
				if (tv != NULL) {
					tv->activate();
					LOG4CXX_INFO(log, getName() << " :: Person activated Television to veg out.");
				} else {
					LOG4CXX_ERROR(log, getName() << " :: Person is trying to veg out, but can't find a Television instance to use.");
				}
			}

			// Person goes to bed between 11:00 (i.e. 23:00) and 11:30 (i.e. 23:30)
			if (scheduler_->getHours() == 23) {
				// When the current ticks is the bed time, Person goes to bed.
				if (ticks_ == bedTime_) {
					goToBed();
				} else {
					// If it's not bedtime yet, increment ticks
					ticks_++;
				}
			}
		}
	} else {
		LOG4CXX_DEBUG(log, getName() << " :: tick(): Person has not been associated with a Scheduler. (This message will also appear for test case instances of Models.)");
	}
}

int Person::getTicks() {
	return ticks_;
}

int Person::getRandomNum(int lower, int upper) {
	RandomInt r(lower, upper);
	return r();
}

void Person::wakeUp() {
	awake_ = true;
	justWokeUp_ = true;
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it woke up and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Woke up at " << scheduler_->getFormattedTime());
}

void Person::goToBed() {
	awake_ = false;
	// Turn off Television
	watchingTv_ = false;
	House *ptr = House::getInstance();
	Television *tv = dynamic_cast<Television *> (ptr->getModel("Television"));
	if (tv != NULL) {
		tv->deactivate();
	} else {
		LoggerPtr log(Logger::getLogger(getName()));
		LOG4CXX_ERROR(log, getName() << " :: Person is trying to turn off the Television to go to bed, but can't find a Television instance to use.");
	}
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it went to sleep and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Went to bed at " << scheduler_->getFormattedTime());
}

void Person::cook(int duration) {
	cooking_ = true;
	cookTimeLeft_ = duration;
	// Notify Refrigerator that a Person is cooking.
	House *ptr = House::getInstance();
	Refrigerator *fridge = dynamic_cast<Refrigerator *> (ptr->getModel(
			"Refrigerator"));
	if (fridge != NULL) {
		fridge->setCookingState(true);
	} else {
		LoggerPtr log(Logger::getLogger(getName()));
		LOG4CXX_ERROR(log, getName() << " :: Person is trying to cook, but can't find a Refrigerator instance to use.");
	}
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it started cooking and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Started cooking at " << scheduler_->getFormattedTime());
}

void Person::finishCooking() {
	cooking_ = false;
	cookTimeLeft_ = 0;
	// Notify Refrigerator that no one is cooking.
	House *ptr = House::getInstance();
	Refrigerator *fridge = dynamic_cast<Refrigerator *> (ptr->getModel(
			"Refrigerator"));
	if (fridge != NULL) {
		fridge->setCookingState(false);
	} else {
		LoggerPtr log(Logger::getLogger(getName()));
		LOG4CXX_ERROR(log, getName() << " :: Person is trying to stop cooking, but can't find a Refrigerator instance to use.");
	}
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it stopped cooking and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Stopped cooking at " << scheduler_->getFormattedTime());
}

void Person::takeShower(int duration) {
	showering_ = true;
	showerTimeLeft_ = duration;
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it started showering and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Started showering at " << scheduler_->getFormattedTime());
}

void Person::finishShower() {
	showering_ = false;
	justFinishedShower_ = true;
	ticks_ = 0;
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it finished showering and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Finished showering at " << scheduler_->getFormattedTime());
}

void Person::takeBath(int duration) {
	bathing_ = true;
	bathTime_ = duration;
	LoggerPtr log(Logger::getLogger(getName()));
	if (scheduler_) {
		// Write INFO message to log saying it started taking bath and giving the time.
		LOG4CXX_INFO(log, getName() << " :: Started taking bath at " << scheduler_->getFormattedTime());
	} else {
		LOG4CXX_DEBUG(log, getName() << " :: Person has not been associated with a Scheduler. (This message will also appear for test case instances of Models.)");
	}
}

void Person::finishBath() {
	bathing_ = false;
	vegOut_ = true;
	ticks_ = 0;
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it finished taking bath and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Finished taking bath at " << scheduler_->getFormattedTime());
}

void Person::watchTelevision() {
	watchingTv_ = true;
	ticks_ = 0;
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it started watching TV and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Started watching TV at " << scheduler_->getFormattedTime());
}

void Person::leaveHouse() {
	atBreakfast_ = false;
	atHome_ = false;
	ticks_ = 0;
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it left the house and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Left the house at " << scheduler_->getFormattedTime());
}

void Person::comeHome() {
	atHome_ = true;
	justGotHome_ = true;
	ticks_ = 0;
	LoggerPtr log(Logger::getLogger(getName()));
	// Write INFO message to log saying it arrived at the house and giving the time.
	LOG4CXX_INFO(log, getName() << " :: Arrived at the house at " << scheduler_->getFormattedTime());
}
