#include <chrono>
#include <ratio>
#include <thread>
#include <iostream>

using namespace std::chrono;

int main()
{
	std::ratio< 1, 1000> ratioMilliseconds;
	std::ratio< 1, 1000> ratioMinute;
	milliseconds dur( 1000 );

	std::cout << "Sleeping for 1 seconds\n";
	std::cout << high_resolution_clock::now() << std::end;
    std::this_thread::sleep_for( dur );
	std::cout << "Done sleeping\n";

	//time_point< high_resolution_clock, duration< double, std::ratio< 1, 1 > > >  t = high_resolution_clock::now();
	
	return 0;
}
