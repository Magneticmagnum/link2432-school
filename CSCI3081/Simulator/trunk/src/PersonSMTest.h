/** PersonSMTest.h:
 * Unit tests for the PersonSM class.
 */

#ifndef SMTEST_H_
#define SMTEST_H_

#include <cxxtest/TestSuite.h>
#include "Scheduler.h"
#include "Model.h"
#include "PersonSM.h"

class PersonSMTest: public CxxTest::TestSuite {
public:

	/** test_personSM_constructor:
	 * Tests that the PersonSM constructor creates a new instance, and
	 * that the instance starts out at 0 ticks, off, and w/water temperature 50 deg C.
	 */
	void test_personSM_constructor() {
		PersonSM p;
		TS_ASSERT_EQUALS(p.funcargs_["activate"], PersonSM::activate);
		TS_ASSERT_EQUALS(p.funcargs_["waitOnTarget"], PersonSM::waitOnTarget);
		TS_ASSERT_EQUALS(p.funcargs_["randomTime"], PersonSM::randomTime);
		TS_ASSERT_EQUALS(p.funcargs_["upperTime"], PersonSM::upperTime);
		TS_ASSERT_EQUALS(p.funcargs_["lowerTime"], PersonSM::lowerTime);
		TS_ASSERT_EQUALS(p.funcargs_["delay"], PersonSM::delay);
		TS_ASSERT_EQUALS(p.funcargs_["end"], PersonSM::end);
	}

	/** test_personSM_loadStateFile:
	 * Tests that the state file is loaded correctly.
	 */
	void test_personSM_loadStateFile() {
		PersonSM p;
		p.loadStateFile("person-spec.txt");
		TS_ASSERT(p.argmap_.size() != 0);
	}

	/** test_personSM_tick:
	 * Tests that the PersonSM tick method increments states when necessary.
	 * Also tests that states are added correctly.
	 */
	void test_personSM_tick() {
		PersonSM p;
		stateargs_t* s = new stateargs_t;
		s->counter = 0;
		s->name = "BEGIN";
		s->next = "sleeping";
		s->nummodels = 0;
		s->params = "0";
		s->statefinished = true;
		s->time = 1;
		s->type = "delay";
		stateargs_t* s1 = new stateargs_t;
		s1->counter = 0;
		s1->name = "sleeping";
		s1->next = "wake";
		s1->nummodels = 0;
		s1->params = "";
		s1->statefinished = false;
		s1->time = 2;
		s1->type = "delay";
		p.addState(s);
		p.addState(s1);
		TS_ASSERT(p.argmap_["BEGIN"] == s);
		TS_ASSERT(p.argmap_["sleeping"] == s1);
		p.currentstate = s;
		p.tick();
		TS_ASSERT(p.currentstate->name == "sleeping");
	}

	/** test_personSM_stateFunctions:
	 * Tests that PersonSM class state functions work.
	 */
	void test_personSM_stateFunctions() {
		// We were unable to find a way to test the static functions in PersonSM.
	}

};

#endif /*SMTEST_H_*/
