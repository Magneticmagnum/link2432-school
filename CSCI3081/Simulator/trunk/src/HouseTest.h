/** HouseTest.h:
 * Unit tests for the House class.
 */
#ifndef HOUSETEST_H_
#define HOUSETEST_H_

#include <cxxtest/TestSuite.h>
#include "House.h"
#include "Model.h"
#include "Scheduler.h"
#include "Refrigerator.h"
#include "WaterHeater.h"
#include "Dishwasher.h"
#include "Toaster.h"
#include "Microwave.h"
#include "Oven.h"
#include "Stove.h"
#include "Television.h"
#include "HairDryer.h"
#include "Xbox.h"
#include "Person.h"

class HouseTest: public CxxTest::TestSuite {
public:

	/** test_house_constructor:
	 * Tests the House constructor.
	 */
	void test_house_constructor() {
		Model model;
		House *house = House::getInstance();
		house->putModel("Model", &model);
		Model *modelPtr = house->getModel("Model");
		TS_ASSERT_EQUALS(&*modelPtr, &model);
		house->clear();
	}

	/** test_house_loadConfig:
	 * Tests that config files are loaded correctly & Models get registered with Scheduler.
	 */
	void test_house_loadConfig() {
		House *house = House::getInstance();
		house->clear();
		house->registerModelFactory("Dishwasher", &construct<Dishwasher>);
		house->registerModelFactory("HairDryer", &construct<HairDryer>);
		house->registerModelFactory("Microwave", &construct<Microwave>);
		house->registerModelFactory("Oven", &construct<Oven>);
		house->registerModelFactory("Refrigerator", &construct<Refrigerator>);
		house->registerModelFactory("Stove", &construct<Stove>);
		house->registerModelFactory("Television", &construct<Television>);
		house->registerModelFactory("Toaster", &construct<Toaster>);
		house->registerModelFactory("WaterHeater", &construct<WaterHeater>);
		house->registerModelFactory("Xbox", &construct<Xbox>);
		house->registerModelFactory("Person", &construct<Person>);
		Scheduler sched;
		house->loadConfig("model-spec.txt", sched);
		TS_ASSERT(sched.getModels().size() != 0);
	}
};

#endif /*HOUSETEST_H_*/
