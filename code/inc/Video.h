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
        virtual void SetTime(int64_t);
        virtual void InitalTool();
        virtual void CleanUp(){};
        static int64_t ReturnAmount();
        static void ResetAmount();
        int TranscodeFlow(AVPacket*);
        void MuxFlow(AVPacket*);
        AVFrame* ReturnFrame(AVPacket*);
    private:
        AVStream* _input_stream;
        AVStream* _output_stream;
        AVFormatContext* _ofmt_ctx;
        int _got_frame;
        int _video_index = 0;
        int64_t _start_time;
        AVFrame* _decode_frame;
        bool _interlaced = true;
        int64_t last_correct_pts = 0;//用這個來修改pts小於0的overflow
        int64_t last_correct_dts = 0;//For mux
        static int64_t _count_frames;//for Timer use
        int  VideoDecoder(AVPacket*);
        int Encode_write_frame(AVPacket*);
        void _resetpts(AVPacket*);

    };
}
#endif