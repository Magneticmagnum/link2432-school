/** ConfigFileTest.h:
 * Unit tests for the ConfigFile class.
 */

#ifndef CONFIGFILETEST_H_
#define CONFIGFILETEST_H_

#include <cxxtest/TestSuite.h>
#include "ConfigFile.h"

class ConfigFileTest : public CxxTest::TestSuite {
public:

	/** test_configFile_constructor:
	 * Tests that ConfigFile objects are created correctly.
     */
	void test_configFile_constructor() {
		ConfigFile cfg = ConfigFile("model-spec.txt");
		TS_ASSERT(cfg.getProps("Dishwasher")->size() != 0);
		TS_ASSERT(cfg.getPropsMap().size() != 0);
		TS_ASSERT(cfg.begin() == cfg.modelprops_.begin());
		TS_ASSERT(cfg.end() == cfg.modelprops_.end());
	}

};

#endif /*CONFIGFILETEST_H_*/
