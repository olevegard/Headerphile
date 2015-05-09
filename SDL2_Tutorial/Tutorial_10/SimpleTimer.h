#pragma once

#include <chrono>

using namespace std::chrono; // Adding this means we don't have to add std::chrono in front of everything

class Timer
{
	public:
		Timer()
			// Init timePrev to the current point in time
			:	timePrev( high_resolution_clock::now() )
		{
		}

		// Returns time since last time this function was called in seconds with nanosecond precision
		double GetDelta()
		{
			// 1. Gett current time as a std::chrono::time_point
			auto timeCurrent = high_resolution_clock::now();

			// 2. Get the time difference as seconds represented as a double
			duration< double > delta( timeCurrent - timePrev );

			// 3. Reset the timePrev to the current point in time
			timePrev = high_resolution_clock::now();

			// 4. Returns the number of ticks in delta
			return delta.count();
		}
	private:

		// For delta time calculation, updated every frame
		// We use high_resolution_clock because we want the highest possible accuracy
		time_point< high_resolution_clock > timePrev;
};

