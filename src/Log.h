/*
 * Locator programming pattern for logging:
 *
 * There is a log interface, concrete classes, and a locator class.
 *
 * The code that uses logging first uses the locator to get the concrete object. This avoids
 * coupling between the concrete class and the user. Outside code is responsible for giving the locator
 * a concrete object, so the locator isn't coupled either.
 *
 * Example usage:
 * 	 Outside code:
 * 		ConsoleLog *log = new ConsoleLog();
 * 		Locator::provide(log);
 *
 * 	 Dependent code:
 * 	 	Log& log = Locator::getLog();
 * 	 	log.logMsg("Log Message");
 */

#pragma once

#include <iostream>
#include <string>

class Logger {
public:
	virtual ~Logger() {}
	virtual void logMsg(std::string msg) = 0;
};

class NullLogger : public Logger
{
public:
	void logMsg(std::string msg) override
	{
		// Do nothing
	}
};

class ConsoleLogger : public Logger
{
public:
	void logMsg(std::string msg) override
	{
		// Log to console
		std::cout << msg << "\n";
	}

};


class Locator
{
public:

	static void init() { service_ = &nullService_; }

	static void disable() { provide(nullptr); }

	static Logger& getLogger() { return *service_; }

	static void provide(Logger* service)
	{
		// Check for null in provide to save cycles when accessing service
		if (service == nullptr)
		{
			// Revert to null service
			service_ = &nullService_;
		}
		else {
			service_ = service;
		}
	}

private:
	static Logger* service_;
	static NullLogger nullService_;
};
