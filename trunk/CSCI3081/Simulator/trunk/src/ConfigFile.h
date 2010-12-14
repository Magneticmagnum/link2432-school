/*
 * ConfigFile.h
 *
 *  Created on: Nov 25, 2010
 *      Author: Joe Houlton
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <string>
#include <map>
#include <log4cxx/logger.h>

using log4cxx::Logger;
using log4cxx::LoggerPtr;

typedef std::map<std::string, std::string> PropertyTable;

class ConfigFile {
public:
	ConfigFile(std::string);
	virtual ~ConfigFile();

	/** getProps:
	 * Returns a PropertyTable containing the properties for the Model called modelName.
	 */
	PropertyTable* getProps(std::string modelName);

	/** getPropsMap:
	 * Returns the map of PropertyTables for all Models.
	 */
	std::map<std::string, PropertyTable *> getPropsMap();
	std::map<std::string, PropertyTable *>::iterator begin();
	std::map<std::string, PropertyTable *>::iterator end();

	friend class ConfigFileTest;

private:
	std::map<std::string, PropertyTable *> modelprops_;
};

#endif /* CONFIGFILE_H_ */
