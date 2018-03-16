#ifndef __TRANSCODER_H__
#define __TRANSCODER_H__
#include "Stream.h"
#include <mutex>
#include <memory>
#include <vector>
#include <unordered_map>
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
    class Transcoder{
    private:          
        static std::unique_ptr<Transcoder> _instance;
        static std::once_flag _transcoder_flag;
        AVFormatContext* ifmt_ctx;
        std::unordered_map<int,std::shared_ptr<Stream> > _pidObject;
        std::vector<AVFormatContext*> ofmt_list;//把每個ofmt_list拿進來這個vector方便free
        AVIOContext *avio_ctx;
        uint8_t *avio_ctx_buffer;
        void _cleanUp();
        void _muxingFlow();
    public:
        Transcoder();  
        static Transcoder& Instance();
        int OutputAmount();
        void CleanUp();
        void InputFile(const char* filename);
        void OutputFile(const char*,int ,int );//這個outputfunction 可以call很多次for multiple outputformat
        void MainTask();
        void Flow();
        void InitalAvio(buffer_data* bd);
    };
};
#endif