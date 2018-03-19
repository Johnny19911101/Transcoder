
#include "Transcoder.h"
#include "Video.h"
#include <iostream>
using namespace Noovo;


int main(){
    Transcoder test;
    test.InputFile("test.ts");
    test.OutputFile("test2.aac",0,0x100);
    test.OutputAmount();
    test.Flow();
    std::cout << "hi"<<std::endl;
    return 0;
}