#ifndef __ADINSERT_H__
#define __ADINSERT_H__
#include <map>
#include <thread>
#include "Stream.h"

namespace Noovo{

    class Adinsert  {
    public:
        Adinsert(std::string);
        ~Adinsert();
        std::thread Process_thread(std::shared_ptr<Ofmt_list>);

    private:
        void _initpacket(AVPacket* packet);
        void _process(std::shared_ptr<Ofmt_list>);
        std::map<int,AVMediaType> _index_type;
        AVFormatContext *_ifmt_ctx=nullptr;
    };
}
#endif