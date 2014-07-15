// Timer.h - headerphile.blogspot.no
// A class for timing functioanlity
// Contains delta time calculation and average fps calculation
// Heavily commented for learning purposes
// One of the major purposes of this class is to teach how std::chrono works

#pragma once

#include <chrono>

using namespace std::chrono; // Adding this means we don't have to add std::chrono in front of everything

class Timer
{
	public:
		Timer();

		// Returns time since last time this function was called in seconds with nanosecond precision
		double GetDelta();

		// Get average FPS. Spans the time from when the Timer object was created so the vale will stabilize over time.
		int32_t GetAverageFPS();
	private:
		// Do the actual average FPS calcuations. Called every frame.Updates currentFPS.
		void RecalculateFPS( const nanoseconds &runningTime );

		// Check if enough time ( fpsUpdateInterval milliseconds ) has passed since last recalculation
		bool IsTimeForFPSUpdate( const nanoseconds &runningTime ) const;

		// Vairables
		// ================================================================================================
		time_point< high_resolution_clock > timePrev;	// For delta time calculation, updated every frame
		time_point< steady_clock > timeStart;			// For average FPS calculation, never updated

		int64_t frameCount;			// Total number of frames so far
		int64_t nextFPSUpdate;		// When to recalculate FPS count, milliseconds
		int64_t fpsUpdateInterval;	// How often to update FPS count, milliseconds
		int32_t currentFPS;			// FPS for this second. Updated when IsTimeForFPSUpdated is true, otherwise it's just returned
};
