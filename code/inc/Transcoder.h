#ifndef __TRANSCODER_H__
#define __TRANSCODER_H__
#include <mutex>
#include <memory>
#include <vector>
#include <condition_variable>
#include <unordered_map>
#include "Stream.h"
#include "TransState.h"
#define OUTPUT_BIT_RATE 48000
/** The number of output channels */
#define OUTPUT_CHANNELS 2
/** The audio sample output format */
#define OUTPUT_SAMPLE_FORMAT AV_SAMPLE_FMT_S16
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
        void InputFile(const char* filename);
        void OutputFile(const char*,int ,int );
        void MainTask();
        void Flow();
        void InitalAvio(buffer_data* bd);
        std::string ReturnStateName();
        int StopProcess();//This funtcion will automatically Cleanup ,so dont cleanup at other thread again
    private:      
        std::unique_ptr<Status> _state;
        bool _turnoff;
        std::condition_variable _cleanup_done;
        std::mutex _lock_cleanup;
        std::mutex _lock_process; 
        static std::unique_ptr<Transcoder> _instance;
        static std::once_flag _transcoder_flag;
        AVFormatContext* ifmt_ctx;
        std::unordered_map<int,std::shared_ptr<Stream> > _pidObject;
        std::vector<AVFormatContext*> ofmt_list;
        AVIOContext *avio_ctx;
        uint8_t *avio_ctx_buffer;
        void _cleanUp();
        void _muxingFlow();
    };
}
#endif