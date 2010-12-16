/** HouseTest.h:
 * Unit tests for the House class.
 */
#ifndef HOUSETEST_H_
#define HOUSETEST_H_

#include <cxxtest/TestSuite.h>
#include "House.h"
#include "Model.h"
#include "Scheduler.h"
#include "Xbox.h"
#include "ConfigFile.h"

class HouseTest: public CxxTest::TestSuite {
public:

	/** test_house_constructor:
	 * Tests the House constructor.
	 */
	void test_house_constructor() {
		Model model;
		House *house = House::getInstance();
		TS_ASSERT_EQUALS(house->getInstance(), house);
		house->putModel("Model", &model);
		Model *modelPtr = house->getModel("Model");
		TS_ASSERT_EQUALS(&*modelPtr, &model);
		house->clear();
	}

	/** test_house_loadConfig:
	 * Tests that config files are loaded correctly, Models are created correctly, & Models get registered with Scheduler.
	 */
	void test_house_loadConfig() {
		House *house = House::getInstance();
		house->clear();
		house->registerModelFactory("Xbox", &construct<Xbox> );
		Scheduler sched;
		house->loadConfig("model-spec-test.txt", sched);
		TS_ASSERT(sched.getModels().size() != 0);
		TS_ASSERT_EQUALS(house->createModel("Xbox","Xbox")->getName(), "DefaultXbox");
		house->clear();
	}
};

#endif /*HOUSETEST_H_*/
