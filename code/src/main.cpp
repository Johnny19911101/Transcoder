#include <iostream>
#include <unistd.h>
#include "Transcoder.h"
#include <thread>
using namespace Noovo;
char ch;
/*Transcoder test;
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
        sleep(3);
        test.StopProcess();
    
    
}
*/
int main(){
/*    std::thread t1(test1);
    std::thread t2(test2);
    t1.join();
    t2.join();
    std::cout << "hi"<<std::endl;
*/
    std::unordered_map<int,std::shared_ptr<Stream>> testmpa;
    std::vector<AVFormatContext*> test2;
    std::pair<int,int> t{0x100,0};
    std::vector<std::pair<int,int>> test4;
    test4.push_back(t);
    Transcoder test;
    int i=0;
   // while(1){
        test.SetConfig("/home/johnny/abc.ts","test2",test4);
        test.Process();
        test.CleanUp();

       
   // }
    return 0;

}