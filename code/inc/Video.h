#ifndef __VIDEO_H__
#define __VIDEO_H__
#include "Stream.h"

namespace Noovo{

    class Video : public Stream
    {
    public:
        Video();
        virtual ~Video();      
        virtual void SetTool(AVStream* ,int, AVFormatContext* );
        virtual void FlushEncoder();
        virtual int Flow(AVPacket*);
        virtual void Codecname();
        virtual void SetTime(int64_t,int64_t);
        int TranscodeFlow(AVPacket*);
        void MuxFlow(AVPacket*);
        AVFrame* ReturnFrame(AVPacket*);
        virtual void InitalTool();
        virtual void CleanUp(){};
        static int64_t ReturnAmount();
        static void ResetAmount();
    private:
        AVStream* _input_stream;
        AVStream* _output_stream;
        AVFormatContext* _ofmt_ctx;
        int64_t _start_time;
        int64_t _first_dts;
        int _got_frame;
        int _video_index = 0;
        AVFrame* _decode_frame;
        bool _interlaced = true;
        int64_t last_correct_pts = 0;//用這個來修改pts小於0的overflow
        int64_t last_correct_dts = 0;
        static int64_t _count_frames;//for Timer use
        void ResetPts(AVPacket*);
        void ResetDts(AVPacket*);
        int  VideoDecoder(AVPacket*);
        int Encode_write_frame(AVPacket*);

    };
}
#endif