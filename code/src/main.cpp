
#include "Transcoder.h"
#include "Video.h"
#include <iostream>
using namespace Noovo;


int main(){
    Transcoder test;
    test.InputFile("frc.ts");
    test.OutputFile("test2.aac",0,0x14a);
    test.OutputAmount();
    test.Flow();
    std::cout << "hi"<<std::endl;
    return 0;
}