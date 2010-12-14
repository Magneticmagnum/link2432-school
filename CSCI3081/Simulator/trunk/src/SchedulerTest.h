/** SchedulerTest.h:
 * Unit tests for the Scheduler class.
 */

#ifndef SCHEDULERTEST_H_
#define SCHEDULERTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Refrigerator.h"

class SchedulerTest : public CxxTest::TestSuite {
public:

	/** test_scheduler_constructor:
	 * Checks that new Scheduler objects are initialized correctly.
	 */
	void test_scheduler_constructor() {
		Scheduler sched = Scheduler();
		TS_ASSERT_EQUALS(sched.getTime(), 0);
	}

	/** test_scheduler_registerModel:
	 * Checks that models are registered correctly in the Scheduler.
     */
	void test_scheduler_registerModel() {
		Scheduler sched = Scheduler();
		Refrigerator fridge1 = Refrigerator();
		Refrigerator fridge2 = Refrigerator();
		// Check that registering models returns true
		TS_ASSERT(sched.registerModel(fridge1) == true);
		TS_ASSERT(sched.registerModel(fridge2) == true);
		// Check that fridge1 and fridge2 have been added to models
		TS_ASSERT(sched.getModels().at(0) == &fridge1);
		TS_ASSERT(sched.getModels().at(1) == &fridge2);
		// Check that scheduler is correctly set for fridge1 and fridge2
		TS_ASSERT(fridge1.getScheduler() == &sched);
		TS_ASSERT(fridge2.getScheduler() == &sched);
	}

	/** test_scheduler_run:
	 * Tests that running the Scheduler works correctly.
	 * Checks that it loops through ticks until maxCount is reached,
	 * and loops through all the Models, adding a tick for each Model.
     */
	void test_scheduler_run() {
		Scheduler sched = Scheduler();
		// Check that Scheduler's time is initially 0.
		TS_ASSERT_EQUALS(sched.getTime(), 0);
		TS_ASSERT_EQUALS(sched.getMinute(), 0);
		TS_ASSERT_EQUALS(sched.getHours(), 0);
		TS_ASSERT_EQUALS(sched.getDay(), 0);
		// Call run function which will be tested.
		sched.run(10);
		// Check that the Scheduler's time is 10 after run(10) is called.
		TS_ASSERT_EQUALS(sched.getTime(), 10);
		TS_ASSERT_EQUALS(sched.getMinute(), 10);
		TS_ASSERT_EQUALS(sched.getHours(), 0);
		TS_ASSERT_EQUALS(sched.getDay(), 0);
	}

};

#endif /*SCHEDULERTEST_H_*/
