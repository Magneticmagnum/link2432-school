/*
 * House.h
 *
 *  Created on: Nov 9, 2010
 *      Author: Joe Houlton
 */

#ifndef HOUSE_H_
#define HOUSE_H_
#include <map>
#include <string>
#include "Model.h"
#include "ConfigFile.h"
#include "Scheduler.h"
#include "RandomInt.h"
#include <log4cxx/logger.h>

using log4cxx::Logger;
using log4cxx::LoggerPtr;

// Constructs new Model
template<class T> Model* construct() {
	return new T;
}

// Constructs new Model factory
typedef Model* (*factory)();

class House {
public:

	/** loadConfig:
	 * Loads configuration file config abd configures models.
	 */
	void loadConfig(string config, Scheduler &sched);

	/** getInstance:
	 * Returns an instance of the House class.
	 * Creates new instance if none exists, otherwise returns existing instance.
	 */
	static House* getInstance();

	/** getModel:
	 * Gets a Model named key.
	 */
	Model* getModel(std::string key);

	/** putModel:
	 * Puts a Model named key, of Model type model, into the Model locator.
	 */
	void putModel(std::string key, Model *model);

	/* createModel:
	 * Uses a factory to construct a Model and registers it with the House.
	 */
	Model* createModel(std::string type, std::string name);

	/* registerModelFactory:
	 * Registers a factory with the House.
	 */
	void registerModelFactory(std::string type, factory f);

	/** clear:
	 * Clears the contents of the internal map.
	 */
	void clear();

private:
	static House* instance_;
	std::map<std::string, Model *> modelMap_;
	std::map<std::string, factory> factories_;
	ConfigFile* cfg;
	House() {}
	~House() {}
	House(const House &);
	House & operator=(const House &);

};
#endif //HOUSE_H_
