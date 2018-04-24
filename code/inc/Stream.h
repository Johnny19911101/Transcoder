#ifndef __STREAM_H__
#define __STREAM_H__
#include <mutex>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <condition_variable>
#include <unordered_map>
extern "C"{
    #include <stdio.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include "libavformat/avio.h"
    #include <libavutil/file.h>
    #include <libavfilter/buffersink.h>
    #include "libavutil/audio_fifo.h"
    #include "libavutil/avassert.h"
    #include <libavfilter/buffersrc.h>
    #include <libavutil/opt.h>
    #include <libavutil/pixdesc.h>
    #include "libavutil/timestamp.h"
    #include "libswresample/swresample.h"
    #include "libavutil/avstring.h"
    #include "libavutil/avutil.h"
    #include "libavutil/frame.h"
    #include "libavutil/opt.h"
}

namespace Noovo{
    class Stream{
    public:
        virtual ~Stream(){};
        virtual void SetTool(AVStream*,int,AVFormatContext*)=0;
        virtual void FlushEncoder()=0;
        virtual void SetTime(int64_t,int64_t)=0;
        virtual int Flow(AVPacket*)=0;
        virtual void Codecname()=0;
        virtual void InitalTool()=0;
        virtual void CleanUp()=0;
        virtual int64_t ReturnEndPoint()=0;
    };
    class Ofmt_list{
    public:
        Ofmt_list():_ofmt_ctx(nullptr),_video_index(-1),_audio_index(-1){};
        ~Ofmt_list(){};
        inline void SetAvformat(AVFormatContext* ctx){
            _ofmt_ctx=ctx;
        }
        inline void SetVideo(int index){
            _video_index=index;
        }
        inline void SetAudio(int index){
            _audio_index=index;
        }
        inline void SetConcat(int64_t concate){
            _concatenate=concate;
        }
        inline AVFormatContext* ReturnAvformat(){
            return _ofmt_ctx;
        }
        inline int Returnvideo(){
            return _video_index;
        }
        inline int Returnaudio(){
            return _audio_index;
        }
        inline int64_t Returnconcate(){
            return _concatenate;
        }
        void SetStream(std::shared_ptr<Stream> audio,std::shared_ptr<Stream> video){
            _audio = audio;
            _video = video;    
        }
        void GetConcatpoint(){
            _concatenate =_video->ReturnEndPoint();
        }

    private:
        AVFormatContext* _ofmt_ctx;
        int _video_index;
        int _audio_index;
        int64_t _concatenate;
        std::shared_ptr<Stream> _audio,_video;
    };
}

#endif