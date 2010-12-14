/*
 * simulator.cpp
 *
 *  Created on: Oct 8, 2010
 *      Author: Joe Houlton, Maya Pevzner
 */

#include <iostream>
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
#include "House.h"
#include "PersonSM.h"
#include "ConfigFile.h"

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

int main(int argc, char *argv[]) {

	// Set up the logger.
	LoggerPtr log(Logger::getLogger("Main"));
	PropertyConfigurator::configure(File("log.config"));

	// Run the simulator.
	LOG4CXX_INFO(log, "Simulator started. Log file opened.");

	// Create Models & register Models with House.
	House *house = House::getInstance();
	house->registerModelFactory("Dishwasher", &construct<Dishwasher> );
	house->registerModelFactory("HairDryer", &construct<HairDryer> );
	house->registerModelFactory("Microwave", &construct<Microwave> );
	house->registerModelFactory("Oven", &construct<Oven> );
	house->registerModelFactory("Refrigerator", &construct<Refrigerator> );
	house->registerModelFactory("Stove", &construct<Stove> );
	house->registerModelFactory("Television", &construct<Television> );
	house->registerModelFactory("Toaster", &construct<Toaster> );
	house->registerModelFactory("WaterHeater", &construct<WaterHeater> );
	house->registerModelFactory("Xbox", &construct<Xbox> );
	house->registerModelFactory("PersonSM", &construct<PersonSM> );

	// Load config file, configure Models, and register Models with Scheduler.
	Scheduler sched;
	house->loadConfig(argv[1], sched);
	PersonSM* johndoe = (PersonSM*) (house->getModel("JohnDoe"));
	johndoe->loadStateFile(argv[2]);

	// Call the Scheduler's run() method, wait for it to complete.
	sched.run(4320);

	// Exit simulator.
	LOG4CXX_INFO(log, "Simulator exited. Log file closed.");
	std::cout << "Simulation completed." << std::endl;
	return 0;
}
