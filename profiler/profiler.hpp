#ifndef PROFILER_HPP
#define PROFILER_HPP
#include <map>
#include "../timer/timer.hpp"


class timing
{
	private:
		timer personalTimer;
	public:
		int executionCount;
		float best, worst, average, total;

		timing();
		void start();
		void stop();
};
class profiler
{
	private:
		std::map<std::string, timing*> timers;
	public:
		profiler();
		~profiler();

		//Returns NULL if the timing doesn't exist
		timing* getTiming(const std::string&);
		//Creates a new timing or updates an existing one
		void startTiming(const std::string&);
		//Stops an existing timing and increments its call counter
		void stopTiming(const std::string&);
		//Displays the results of a specific timer
		void outputTimingResults(const std::string&);

		//Deletes all timings
		void clearTimings();

		//Shows results from all timers
		void outputAllResults();
		//Shows results from all timers with percentages of the time given
		void outputAllResultsPercentages(float);
};
#endif
