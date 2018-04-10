#ifndef __OPTION_H__
#define __OPTION_H__
#include <thread>


namespace Noovo{

    class Option 
    {
    private:
        Option();
        ~Option();
        
    public :
        static Timer* GetInstance();
        void Create_timerthread(int );
        
}
#endif