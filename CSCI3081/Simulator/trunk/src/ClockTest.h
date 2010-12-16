/** ClockTest.h:
 * Unit tests for the Clock class.
 */

#ifndef CLOCKTEST_H_
#define CLOCKTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "Clock.h"

class ClockTest : public CxxTest::TestSuite {
public:

	/** test_clock_constructor:
	 * Tests that Clock objects are created correctly.
	 * Also tests that time getters work.
     */
	void test_clock_constructor() {
		Clock clk;
		Scheduler *sched = new Scheduler;
		clk.registerScheduler(sched);
		TS_ASSERT_EQUALS(clk.scheduler_->getTime(), sched->getTime());
		TS_ASSERT_EQUALS(clk.getTime(), clk.scheduler_->getTime());
		TS_ASSERT_EQUALS(clk.getMinutesSinceMidnight(), clk.scheduler_->getMinutesSinceMidnight());
		TS_ASSERT_EQUALS(clk.getMinute(), clk.scheduler_->getMinute());
		TS_ASSERT_EQUALS(clk.getHours(), clk.scheduler_->getHours());
		TS_ASSERT_EQUALS(clk.getDay(), clk.scheduler_->getDay());
		TS_ASSERT(clk.getFormattedTime() == clk.scheduler_->getFormattedTime());
	}

	/** test_clock_overloadedMethods:
	 * Tests that methods that have been inherited from Model and overloaded by Clock, do what they should.
     */
	void test_clock_overloadedMethods() {
		Scheduler sched;
		Clock clk;
		clk.tick();
		clk.activate("");
		PropertyTable *props = new PropertyTable;
		clk.configure(props);
	}
};

#endif /*CLOCKTEST_H_*/
