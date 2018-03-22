#include <iostream>
#include <unistd.h>
#include "Transcoder.h"
#include "TransState.h"
#include "Video.h"
#include <thread>
using namespace Noovo;
char ch;
Transcoder test;
static void test1(){
    int i=0;
    
    test.InputFile("/home/johnny/frc.ts");
    test.OutputFile("test2.ts",0x140,0x14a);
    test.OutputAmount();
    test.Flow();
    test.CleanUp();
    i++;
    std::cout << i <<"times" <<std ::endl;
    
}
static void test2(){
    while(1){
        std::cout <<test.ReturnStateName()<<std::endl;
        sleep(1);
    }
}

int main(){
    std::thread t1(test1);
    std::thread t2(test2);
    t1.join();
    t2.join();
    std::cout << "hi"<<std::endl;
    return 0;

}