#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED


#include <ctime>


class Timer
{
    private:
        clock_t _start;

    public:
        void start()
        {
            _start = clock();
        }

        double stop()
        {
            return (clock() - _start) / (double)CLOCKS_PER_SEC;
        }
};


#endif // TIMER_H_INCLUDED
