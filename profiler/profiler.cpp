#ifndef PROFILER_CPP
#define PROFILER_CPP
#include <iostream>
#include "profiler.hpp"
timing::timing()
{
	best=1000000;
	worst=0;
	//So we know it's the first run
	average=-1;
	total=0;
	executionCount=0;
}
void timing::start()
{
	personalTimer.start();
}
void timing::stop()
{
	float result=personalTimer.getTime();
	if (result<best && result!=0) best=result;
	if (result>worst) worst=result;
	
	if (average==-1) average=result;
	else average=(average+result)/2; //Simple averaging function

	total+=result;

	++executionCount;
}

profiler::profiler()
{
	
}
profiler::~profiler()
{
	clearTimings();
}
timing* profiler::getTiming(const std::string& timingName)
{
	std::map<std::string, timing*>::iterator it=timers.find(timingName);
	if (it==timers.end())
	{
		return NULL;
	}
	else return it->second;
}
//Creates a new timing or updates an existing one
void profiler::startTiming(const std::string& timingName)
{
	std::map<std::string, timing*>::iterator it=timers.find(timingName);
	if (it==timers.end())
	{
		timers[timingName]=new timing;
		timers[timingName]->start();
	}
	else it->second->start();
}
//Stops an existing timing and increments its call counter
void profiler::stopTiming(const std::string& timingName)
{
	std::map<std::string, timing*>::iterator it=timers.find(timingName);
	if (it==timers.end())
	{
		std::cout << "ERROR: Timer " << timingName << " does not exist!\n";
	}
	it->second->stop();
}
//Displays the results of a specific timer
void profiler::outputTimingResults(const std::string& timingName)
{
	std::map<std::string, timing*>::iterator it=timers.find(timingName);
	if (it==timers.end())
	{
		std::cout << "ERROR: Timer " << timingName << " does not exist!\n";
	}
	timing *ti=it->second;
	std::cout << timingName << " bst: " << ti->best << " wst: " << ti->worst << "\navg: "
	<< ti->average << " ttl: " << ti->total << " calls: " << ti->executionCount;
}

//Deletes all timings
void profiler::clearTimings()
{
	for (std::map<std::string, timing*>::iterator it=timers.begin(); it!=timers.end(); ++it)
	{
		delete it->second;
	}
	timers.clear();
}

//Shows results from all timers
void profiler::outputAllResults()
{
	for (std::map<std::string, timing*>::iterator it=timers.begin(); it!=timers.end(); ++it)
	{
		timing *ti=it->second;
		std::cout << it->first << " bst: " << ti->best << " wst: " << ti->worst << "\n	avg: "
		<< ti->average << " ttl: " << ti->total << " calls: " << ti->executionCount << "\n";
	}
}

//Shows results from all timers with percentages of the time given
void profiler::outputAllResultsPercentages(float frameTime)
{
	for (std::map<std::string, timing*>::iterator it=timers.begin(); it!=timers.end(); ++it)
	{
		timing *ti=it->second;
		std::cout << it->first << " bst: " << (ti->best/frameTime) * 100 << "% wst: " << (ti->worst/frameTime) * 100 << "%\n	avg: "
		 << (ti->average/frameTime) * 100 << "% ttl: " << (ti->total/frameTime) * 100 << "% calls: " << ti->executionCount << "\n";
	}
}
#endif
