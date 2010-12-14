/**
 * Person.h
 *
 *  Created on: Nov 12, 2010
 *      Author: Maya Pevzner
 */

#ifndef PERSON_H_
#define PERSON_H_

#include "Model.h"

class Person: public Model {
public:
	Person();
	Person(std::string name);
	virtual ~Person();

	/** tick:
	 * Notifies the model that one tick of time has passed since last call to tick();
	 */
	void tick();

	/** getTicks:
	 * Returns the number of ticks it's on.
	 */
	int getTicks();

	/** getRandomNum:
	 * Returns a random number, chosen from uniform distribution in range lower to upper.
	 */
	int getRandomNum(int lower, int upper);

	/** wakeUp:
	 * Sets awake state to false, sets ticks to 0, and logs that Person woke up.
	 */
	void wakeUp();

	/** goToBed:
	 * Sets awake state to false, turns off the Television, and logs that Person went to bed.
	 */
	void goToBed();

	/** cook:
	 * Sets cooking state to true, sets cookTimeLeft to duration, and logs that Person started cooking.
	 */
	void cook(int duration);

	/** finishCooking:
	 * Sets cooking state to false and logs that Person stopped cooking.
	 */
	void finishCooking();

	/** takeShower:
	 * Sets showering state to true, sets showerTimeLeft to duration, and logs that Person started showering.
	 */
	void takeShower(int duration);

	/** finishShower:
	 * Sets showering state to false, sets showerDoneTime to current minutes, and logs that Person finished showering.
	 */
	void finishShower();

	/** takeBath:
	 * Sets bathing state to true, sets bathTimeLeft to duration, and logs that Person started bathing.
	 */
	void takeBath(int duration);

	/** finishBath:
	 * Sets bathing state to false and logs that Person finished bathing.
	 */
	void finishBath();

	/** watchTelevision:
	 * Sets watchingTv state to true and logs that Person started watching TV.
	 */
	void watchTelevision();

	/** leaveHouse:
	 * Sets atHome state to false and logs that Person left the house.
	 */
	void leaveHouse();

	/** comeHome:
	 * Sets atHome state to true and logs that Person arrived back at the house.
	 */
	void comeHome();

	friend class PersonTest;

private:
	int ticks_;
	int bedTime_;
	int cookTimeLeft_;
	int daysSinceDishwasher_;
	int getHomeTime_;
	int showerTimeLeft_;
	int wakeTime_;
	bool atBreakfast_;
	bool atDinner_;
	bool atHome_;
	bool awake_;
	bool bathing_;
	bool cooking_;
	bool fastDinner_;
	bool justGotHome_;
	bool justFinishedDinner_;
	bool justFinishedShower_;
	bool justWokeUp_;
	bool showering_;
	bool vegOut_;
	bool watchingTv_;

	int showerDecide_;
	int breakfastDecide_;
	int ovenDecide_;
	int bathDecide_;
	int bathTime_;
};

#endif /* PERSON_H_ */
