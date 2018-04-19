#ifndef __STREAM_H__
#define __STREAM_H__
#include <mutex>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <thread>
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
        virtual void SetTime(int64_t,int64_t)=0;
        virtual void FlushEncoder()=0;
        virtual int Flow(AVPacket*)=0;
        virtual void Codecname()=0;
        virtual void InitalTool()=0;
        virtual void CleanUp()=0;
        virtual int64_t ReturnEndPoint()=0;
    };
    class Ofmt_list{
    public:
        Ofmt_list():ofmt_ctx(nullptr),video_index(-1),audio_index(-1){};
        ~Ofmt_list(){};
        AVFormatContext* ofmt_ctx;
        int video_index;
        int audio_index;
    };
}

#endif