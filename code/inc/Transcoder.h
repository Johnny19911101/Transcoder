#ifndef __TRANSCODER_H__
#define __TRANSCODER_H__
#include "Stream.h"
#include "TransState.h"
#include "CtxWrapper.h"
struct buffer_data {
    uint8_t *ptr;
    size_t size; ///< size left in the buffer
};
namespace Noovo{
    class Transcoder  {
    public:
        Transcoder();  
        static Transcoder& Instance();
        int OutputAmount();
        void CleanUp();
        void SetConfig(std::string,std::string,std::vector<std::pair<int,int> >&);
        void Process();
        void InitalAvio(buffer_data* bd);
        std::string ReturnStateName();
        int StopProcess();//This funtcion will automatically Cleanup ,so dont cleanup at other thread again
    private:      
        static std::unique_ptr<Transcoder> _instance;
        static std::once_flag _transcoder_flag;
        std::unique_ptr<Status> _state;
        std::shared_ptr<CtxWrapper> _wrapper;
        bool _turnoff;
        std::condition_variable _cleanup_done;
        std::mutex _lock_cleanup,_lock_process;
        std::unordered_map<int,std::shared_ptr<Stream> > _pidObject;
        std::vector<AVFormatContext*> _ofmt_list;
    };
}

#endif