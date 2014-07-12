#include "Timer.h"

Timer::Timer()
	:	timePrev( high_resolution_clock::now() )
	,	timeStart( steady_clock::now() )
	,	frameCount( 0 )
	,	nextFPSUpdate( 1000 )
		,	fpsUpdateInterval( 500 )
{
}
// Returns time since last time this function was called in seconds with nanosecond precision
double Timer::GetDelta()
{
	// Gett current time as a std::chrono::time_point
	// which basically contains info about the current point in time
	auto timeCurrent = high_resolution_clock::now();

	// Compare the two to create time_point containing delta time in nanosecnds
	nanoseconds timeDiff = duration_cast< nanoseconds >( timeCurrent - timePrev );

	// Get the tics as a variable
	double delta = timeDiff.count();

	// Turn nanoseconds into seconds
	// Note : we can't use duration_cast to seconds here because then we'd
	//	loose the precisioon and be left with whole seconds
	//	( which would be 0 every frame, which means no movement can occur at all )
	delta /= 1000000000;

	timePrev = timeCurrent;
	return delta;
}
int32_t Timer::GetAverageFPS()
{
	++frameCount;
	auto timeCurrent = steady_clock::now();

	// Calculate total running time
	nanoseconds runningTime = timeCurrent - timeStart;

	if ( IsTimeForFPSUpdate( runningTime ) )
		RecalculateFPS( runningTime );

	return currentFPS;
}
// Do the actual average FPS calcuations ( updates currentFPS )
void Timer::RecalculateFPS( const nanoseconds &runningTime ) 
{
	nextFPSUpdate += fpsUpdateInterval;
	int64_t runningTimeSec = runningTime.count() / 1000000000;
	currentFPS = frameCount / runningTimeSec;
}

// We need to reduce the interval at which we update the average FPS display because
//	otherwise we'd be wastng resources on recreating the text texture thousands of time every sevcond
//
// Note : we use a duration of nanoseconds here because then we can pass any duration
//	as long as it operates on nanoseconds or something less precise( microsecc, millisec, secounds, ..)
//	duration_cast<> cast is needed if we loose precision
//
// It's possible ( though unlikely ) that we need to use something more precise than
//	nanoseconds here. In that case, we'll have to use duration_cast< nanoseconds > ()
bool Timer::IsTimeForFPSUpdate( const nanoseconds &runningTime ) const
{
	// This if check has three steos
	// 1. Cast runningTime ( which is a duration in milliseconds ) to a duration in milliseocnds;
	// 2. Get the acruall number of milliseconds in tick for the duration returned in the cast above.
	// 3. Compare the number of milliseconds with nextFPSUpdate to see if it's time for another update
	return ( duration_cast< milliseconds > ( runningTime ).count() >= nextFPSUpdate );
}
