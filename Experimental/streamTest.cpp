
#include <iterator>
#include <iostream>

int main()
{
    // ...
    std::cout << "Write something " << std::endl;
    std::istream_iterator< int > intReader( std::cin );

    // reader refers to the end element of the stream
    // This is similar to std::end()
    std::istream_iterator < int > reader;

    while ( intReader != reader )
    {
        std::cout<< "Write "<< *intReader <<std::endl;         
        std::cout<< "You wrote " << *intReader << std::endl; 
        ++intReader;
    }
}