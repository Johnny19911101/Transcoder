#ifndef __TIMER_H__
#define __TIMER_H__
#include <thread>


namespace Noovo{

    class Timer 
    {
    public:
        static Timer* GetInstance();
        void Create_timerthread(int );
    private:
        Timer();
        ~Timer();
        int64_t _finished_frame = 0;
        bool button = true;
        std::thread time_thread;     
        void _process(int );
    };
}
#endif