#include "LogSystem.h"
#include <fstream>
#include <iostream>
#include <ctime>

LogSystem* LogSystem::instance_ = nullptr;

// when the log system is created, it destroys the last .log file
inline LogSystem::LogSystem() : logRoute("..\\exes\\P3.log") { remove(logRoute); }

inline LogSystem::~LogSystem() { logRoute = nullptr; }

LogSystem* LogSystem::getSingleton() {
	if (instance_ == nullptr) {
		instance_ = new LogSystem();
	}

	return instance_;
}

void LogSystem::shutdownSingleton() {
	if (instance_ == nullptr) return;

	delete instance_;
	instance_ = nullptr;
}

// it writes the message msg in the .log file and in the console
// with the time when the message was sent. The .log file will be in "exes\"
void LogSystem::Log(const string& msg) {

	// time stuff
	time_t now = time(0);
	char str[26];
	ctime_s(str, sizeof str, &now);
	string time = {str[11], str[12], str[13] , str[14] , 
		str[15] , str[16] , str[17], str[18], ' ' };

	// file stuff. ios::app -> every output operation will be performed at the end of the file
	ofstream o(logRoute, ios::app);
	if (o.is_open()) o << time << msg << endl;

	// console stuff
	cout << time << msg << endl;

	o.close();
}
