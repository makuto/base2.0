#ifndef TIMER_HPP
#define TIMER_HPP
#include <SFML/System.hpp>

//Note that if the timer is started after updating the window the measurements might be too 
//small to be used
class timer
{
    private: 
        sf::Clock myClock;
    public:
        //Reset the timer to zero and start it (there's no pausing; when startTimer is called the timer
        //is reset and started again)
        void start();
        //Get the current time since last restart
        float getTime();
};
#endif
