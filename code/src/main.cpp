#include <iostream>
#include <unistd.h>
#include "Timer.h"
#include "Transcoder.h"
#include <thread>
#include <vector>
#include <fstream>
using namespace Noovo;
using namespace std;
char ch;
struct buffer_data {
    uint8_t *ptr;
    size_t size; ///< size left in the buffer
};
struct buffer_data bd ;
static int i ;
static    uint8_t *buffer = NULL;
static    size_t buffer_size;
static int read_packet(void *opaque, uint8_t *buf, int buf_size)
{
   // struct buffer_data *bd = (struct buffer_data *)opaque;

/*    if(bd.size < 4096){
        bd.ptr = buffer;
        bd.size = buffer_size;
    }*/
    buf_size = FFMIN(buf_size, bd.size);

    
    if (!buf_size)
        return AVERROR_EOF;
   // printf("ptr:%p size:%zu\n", bd.ptr, bd.size);

    /* copy internal buffer data to buf */
    memcpy(buf, bd.ptr, buf_size);
    bd.ptr  += buf_size;
    bd.size -= buf_size;
    i++;
    return buf_size;
}

static void test1(){
    std::unordered_map<int,std::shared_ptr<Stream>> testmpa;
    std::vector<AVFormatContext*> test2;
    std::pair<int,int> t{0x100,0x110};
    std::vector<std::pair<int,int>> test4;
    test4.push_back(t);
    Transcoder& test = Transcoder::Instance();
    int i=0;
    while(1){
        test.SetConfig("/home/johnny/abc.ts","test2",test4);
        test.Process();
        test.CleanUp();

          }
    
}
static void test2(){

}

int main(){
    int ret;
    
    std::pair<int,int> t{3041,3042};
    std::vector<std::pair<int,int>> test4;
    test4.push_back(t);
    ret = av_file_map("/home/johnny/test.ts", &buffer, &buffer_size, 0, NULL);
    bd.ptr=buffer;
    bd.size =buffer_size;
    Transcoder& test = Transcoder::Instance();
    test.InitalAvio(2048,nullptr,&read_packet,"test.m3u8",test4);
    test.Process();
    return 0;

}