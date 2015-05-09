#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
const int32_t numberOfItems = 10;
struct Tester
{
	public:
	Tester( int32_t ID_, std::string name_ )
    :    ID( ID_ )
    ,    name( name_ )
    {
    	
    }
    
	Tester( int32_t ID_ )
    :    ID( ID_ )
    {
    }
    bool operator<( const Tester &other ) 
    {
         return ID == other.ID;
    }
    bool operator=( const int32_t &val ) 
    {
         return ( ID = val );
    }

	int32_t ID;
	std::string name;
};
int main() {
	// your code goes here

    std::vector< Tester > vec( numberOfItems );
    std::generate_n( std::begin( vec ), numberOfItems, [](){ return rand() % 100; } );

    //for ( const auto &p : vec ) std:: cout << p << std::endl;
    
    std::sort( std::begin( vec ), std::end( vec ));
    std::cout << "Sorted : \n";
    //for ( const auto &p : vec ) std:: cout << p << std::endl;
        
	return 0;
}
