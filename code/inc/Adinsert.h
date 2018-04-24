#ifndef __ADINSERT_H__
#define __ADINSERT_H__

#include "Stream.h"

namespace Noovo{

    class Adinsert  {
    public:
        Adinsert(std::string);
        ~Adinset();
        std::thread Process_thread(std::shared_ptr<Ofmt_list>);

    private:
        void _process(std::shared_ptr<Ofmt_list>);
        std::map<int,AVMediaType> _index_type;
        AVFormatContext _ifmt_ctx=nullptr;
    };
}
#endif