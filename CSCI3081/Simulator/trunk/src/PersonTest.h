/** PersonTest.h:
 * Unit tests for Person class.
 */
#ifndef PERSONTEST_H_
#define PERSONTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
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

class PersonTest: public CxxTest::TestSuite {
public:

	/** test_person_constructor:
	 * Tests that the Person constructor creates a new instance, and
	 * that the instance initializes variables correctly.
	 */
	void test_person_constructor() {
		Person person;
		TS_ASSERT_EQUALS(person.getTicks(), 0);
		TS_ASSERT_EQUALS(person.getName(), "Person");
		TS_ASSERT_EQUALS(person.bathTime_, 0);
		TS_ASSERT_EQUALS(person.bedTime_, 0);
		TS_ASSERT_EQUALS(person.cookTimeLeft_, 0);
		TS_ASSERT_EQUALS(person.daysSinceDishwasher_, 0);
		TS_ASSERT_EQUALS(person.getHomeTime_, 0);
		TS_ASSERT_EQUALS(person.showerTimeLeft_, 0);
		TS_ASSERT_EQUALS(person.wakeTime_, 0);
		TS_ASSERT_EQUALS(person.atBreakfast_, false);
		TS_ASSERT_EQUALS(person.atDinner_, false);
		TS_ASSERT_EQUALS(person.atHome_, true);
		TS_ASSERT_EQUALS(person.awake_, false);
		TS_ASSERT_EQUALS(person.bathing_, false);
		TS_ASSERT_EQUALS(person.cooking_, false);
		TS_ASSERT_EQUALS(person.fastDinner_, false);
		TS_ASSERT_EQUALS(person.justGotHome_, false);
		TS_ASSERT_EQUALS(person.justFinishedDinner_, false);
		TS_ASSERT_EQUALS(person.justFinishedShower_, false);
		TS_ASSERT_EQUALS(person.justWokeUp_, false);
		TS_ASSERT_EQUALS(person.showering_, false);
		TS_ASSERT_EQUALS(person.vegOut_, false);
		TS_ASSERT_EQUALS(person.watchingTv_, false);
		std::string name = "Maya";
		Person p1(name);
		TS_ASSERT_EQUALS(p1.getName(), "Maya");
	}

	/** test_person_tick_upper:
	 * Tests that the Person tick method does everything that it should, increments ticks, activates other Models as needed, etc.
	 * These tests are using upper bounds of random generators.
	 */
	void test_person_tick_upper() {
		RandomInt r;
		Person mp;
		WaterHeater wh;
		Refrigerator fridge;
		Stove sv;
		Oven ov;
		Microwave mw;
		Dishwasher dw;
		Toaster ts;
		Television tv;
		Scheduler sched;
		sched.registerModel(mp);
		House *inst = House::getInstance();
		inst->clear();
		inst->putModel("WaterHeater", &wh);
		inst->putModel("Stove", &sv);
		inst->putModel("Oven", &ov);
		inst->putModel("Microwave", &mw);
		inst->putModel("Dishwasher", &dw);
		inst->putModel("Toaster", &ts);
		inst->putModel("Refrigerator", &fridge);
		inst->putModel("Television", &tv);
		TS_ASSERT_EQUALS(mp.awake_, false);
		// person wakes at 7:00 (in ticks, that's 7*60 = 420)
		sched.run(1);
		TS_ASSERT_EQUALS(mp.ticks_, 1);
		mp.wakeTime_ = 30;
		TS_ASSERT_EQUALS(mp.wakeTime_, 30);
		mp.bedTime_ = 30;
		TS_ASSERT_EQUALS(mp.bedTime_, 30);
		mp.getHomeTime_ = 60;
		TS_ASSERT_EQUALS(mp.getHomeTime_, 60);
		mp.breakfastDecide_ = 1;
		TS_ASSERT_EQUALS(mp.breakfastDecide_, 1);
		mp.showerDecide_ = 1;
		TS_ASSERT_EQUALS(mp.breakfastDecide_, 1);
		mp.ovenDecide_ = 0;
		TS_ASSERT_EQUALS(mp.ovenDecide_, 0);
		mp.bathDecide_ = 1;
		TS_ASSERT_EQUALS(mp.bathDecide_, 1);
		mp.bathTime_ = 20;
		TS_ASSERT_EQUALS(mp.bathTime_, 20);
		sched.run(419);
		TS_ASSERT_EQUALS(mp.awake_, true);
		TS_ASSERT_EQUALS(mp.ticks_, 0);
		TS_ASSERT_EQUALS(mp.justWokeUp_, true);
		// does nothing for 5 minutes
		sched.run(5);
		TS_ASSERT_EQUALS(mp.justWokeUp_, false);
		// person takes a shower
		mp.takeShower(10);
		TS_ASSERT_EQUALS(mp.showering_, true);
		TS_ASSERT_EQUALS(mp.showerTimeLeft_, 10);
		// tick for 10
		sched.run(10);
		TS_ASSERT_EQUALS(mp.justFinishedShower_, true);
		// tick for 15
		sched.run(15);
		// person cooks breakfast for 10 minutes
		TS_ASSERT_EQUALS(mp.atBreakfast_, true);
		sched.run(10);
		// finished with breakfast, now leaves
		TS_ASSERT_EQUALS(mp.atBreakfast_, false);
		TS_ASSERT_EQUALS(mp.atHome_, false);
		// person gets back at 18:30 pm, it is now 7:38 so tick for 10 hrs 52 min or 652 ticks
		sched.run(652);
		TS_ASSERT_EQUALS(mp.atHome_, true);
		// fast dinner
		TS_ASSERT_EQUALS(mp.fastDinner_, true);
		TS_ASSERT_EQUALS(mp.atDinner_, true);
		sched.run(10);
		TS_ASSERT_EQUALS(mp.justFinishedDinner_, true);
		// does nothing for 20 ticks
		sched.run(20);
		// days since dishwasher is 1
		TS_ASSERT_EQUALS(mp.daysSinceDishwasher_, 1);
		// person takes bath (4 minutes to fill + 20 minutes bath time)
		TS_ASSERT_EQUALS(mp.bathTime_, 24);
		TS_ASSERT_EQUALS(mp.bathing_, true);
		sched.run(24);
		// person now veges out from 19:20 to 23:30 (4 hrs 10 min or 250 ticks)
		TS_ASSERT_EQUALS(mp.watchingTv_, true);
		sched.run(250);
		TS_ASSERT_EQUALS(mp.watchingTv_, false);
		TS_ASSERT_EQUALS(mp.awake_, false);
		// check that dishwasher turns on when dishes are not washed for 3 days
		sched.run(31);
		TS_ASSERT_EQUALS(mp.scheduler_->getHours(), 0);
		mp.getHomeTime_ = 60;
		mp.bedTime_ = 30;
		sched.run(1440);
		// days since dishwasher is 2
		TS_ASSERT_EQUALS(mp.daysSinceDishwasher_, 2);
		TS_ASSERT_EQUALS(mp.scheduler_->getHours(), 0);
		mp.getHomeTime_ = 60;
		mp.bedTime_ = 30;
		sched.run(1440);
		// days since dishwasher is 3
		TS_ASSERT_EQUALS(mp.daysSinceDishwasher_, 3);
		TS_ASSERT_EQUALS(mp.scheduler_->getHours(), 0);
		mp.getHomeTime_ = 60;
		mp.bedTime_ = 30;
		sched.run(1440);
		// days since dishwasher is 0
		TS_ASSERT_EQUALS(mp.daysSinceDishwasher_, 0);
		inst->clear();
	}

	/** test_person_tick_lower:
	 * Tests that the Person tick method does everything that it should, increments ticks, activates other Models as needed, etc.
	 * These tests are using lower bounds of random generators.
	 */
	void test_person_tick_lower() {
		RandomInt r;
		Person mp;
		WaterHeater wh;
		Refrigerator fridge;
		Stove sv;
		Oven ov;
		Microwave mw;
		Dishwasher dw;
		Toaster ts;
		Television tv;
		Scheduler sched;
		sched.registerModel(mp);
		House *inst = House::getInstance();
		inst->clear();
		inst->putModel("WaterHeater", &wh);
		inst->putModel("Stove", &sv);
		inst->putModel("Oven", &ov);
		inst->putModel("Microwave", &mw);
		inst->putModel("Dishwasher", &dw);
		inst->putModel("Toaster", &ts);
		inst->putModel("Refrigerator", &fridge);
		inst->putModel("Television", &tv);
		TS_ASSERT_EQUALS(mp.awake_, false);
		// person wakes at 6:30 (in ticks, that's 6.5*60 = 390)
		sched.run(1);
		TS_ASSERT_EQUALS(mp.ticks_, 1);
		mp.wakeTime_ = 0;
		TS_ASSERT_EQUALS(mp.wakeTime_, 0);
		mp.bedTime_ = 0;
		TS_ASSERT_EQUALS(mp.bedTime_, 0);
		mp.getHomeTime_ = 0;
		TS_ASSERT_EQUALS(mp.getHomeTime_, 0);
		mp.breakfastDecide_ = 0;
		TS_ASSERT_EQUALS(mp.breakfastDecide_, 0);
		mp.showerDecide_ = 0;
		TS_ASSERT_EQUALS(mp.breakfastDecide_, 0);
		mp.ovenDecide_ = 1;
		TS_ASSERT_EQUALS(mp.ovenDecide_, 1);
		mp.bathDecide_ = 0;
		TS_ASSERT_EQUALS(mp.bathDecide_, 0);
		mp.bathTime_ = 0;
		TS_ASSERT_EQUALS(mp.bathTime_, 0);
		sched.run(389);
		TS_ASSERT_EQUALS(mp.awake_, true);
		TS_ASSERT_EQUALS(mp.ticks_, 0);
		TS_ASSERT_EQUALS(mp.justWokeUp_, true);
		// does nothing for 5 minutes
		sched.run(5);
		TS_ASSERT_EQUALS(mp.justWokeUp_, false);
		// person decides not to take shower
		TS_ASSERT_EQUALS(mp.justFinishedShower_, true);
		// tick for 15
		sched.run(15);
		// person cooks breakfast for 10 minutes
		TS_ASSERT_EQUALS(mp.atBreakfast_, true);
		sched.run(10);
		// finished with breakfast, now leaves
		TS_ASSERT_EQUALS(mp.atBreakfast_, false);
		TS_ASSERT_EQUALS(mp.atHome_, false);
		// person gets back at 17:30 pm, it is now 7:00 so tick for 10 hrs 30 min or 630 ticks
		sched.run(630);
		TS_ASSERT_EQUALS(mp.atHome_, true);
		// fast dinner
		TS_ASSERT_EQUALS(mp.fastDinner_, false);
		TS_ASSERT_EQUALS(mp.atDinner_, true);
		sched.run(40);
		TS_ASSERT_EQUALS(mp.justFinishedDinner_, true);
		// does nothing for 20 ticks
		sched.run(20);
		// days since dishwasher is 0
		TS_ASSERT_EQUALS(mp.daysSinceDishwasher_, 0);
		// person decides not to take bath
		TS_ASSERT_EQUALS(mp.bathing_, false);
		// person now veges out from 18:28 to 23:00 (4 hrs 32 min or 272 ticks)
		TS_ASSERT_EQUALS(mp.watchingTv_, true);
		sched.run(272);
		TS_ASSERT_EQUALS(mp.watchingTv_, false);
		TS_ASSERT_EQUALS(mp.awake_, false);
		inst->clear();
	}

	/** test_person_shift:
	 *  Tests that "<<" operator is overloaded to output current power & total energy use into the stream.
	 */
	void test_person_shift() {
		// Stream put out the name (using std:ostream&), current power draw, and total energy use.
		Person person;
		cout << endl << "test_person_shift(): " << person;
	}

	/** test_person_cast:
	 *  Tests that "()" operator is overloaded to return total energy consumption in a specified type (int or double).
	 */
	void test_person_cast() {
		Person person;
		cout << endl << "test_person_cast(): int = " << (int) person
				<< ", double = " << (double) person << endl;
		// Compare (int)person with 0
		TS_ASSERT_EQUALS((int)person, 0);
	}
};
#endif /*PERSONTEST_H_*/
