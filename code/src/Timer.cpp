#include "Timer.h"

#include <iostream>
#include <mutex>
#include <chrono>

#include "Video.h"
#define TICK 3
using namespace Noovo;
static Timer* instance;

Timer::Timer(){

}
Timer::~Timer(){

}
Timer* Timer::GetInstance(){
    if(instance == nullptr){
        instance = new Timer();
    }
    return instance;
}
void Timer::Create_timerthread(int loops){
   time_thread = std::thread(&Timer::_process,this,loops);
}
void Timer::_process(int loops){
    int i(0);
    while(i < loops){
        ++i;
        int frames_before = Video::ReturnAmount();
        std::this_thread::sleep_for(std::chrono::seconds(TICK));
        int frames_after = Video::ReturnAmount();
        std::cout <<"\033[1;31mSpeeds : "<< (frames_after - frames_before)/(TICK) <<"frames/s\033[0m"<<std::endl;
    }
    Video::ResetAmount();
}
