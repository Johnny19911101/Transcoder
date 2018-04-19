#ifndef __AUDIO_H__
#define __AUDIO_H__
#include"Stream.h"

namespace Noovo{

    class Audio : public Stream {
     public:
        Audio();
        virtual ~Audio();
        int AudioDecode(AVPacket* );
        int InitResampler();
        int Load_encode_and_write(int64_t);
        int InitFifo();
        int Encode_write_frame(AVPacket* );
        virtual void SetTool(AVStream* ,int  ,AVFormatContext* );
        virtual void FlushEncoder();
        virtual int Flow(AVPacket*);
        virtual void Codecname();
        virtual void InitalTool();
        virtual void CleanUp();
        virtual void SetTime(int64_t,int64_t);
        virtual int64_t ReturnEndPoint();
    private:
        AVStream* _input_stream;
        AVFormatContext* _ofmt_ctx;
        AVFrame* _decode_frame;
        int _data_present;
        int _audio_index;
        int64_t _start_time,_mux_pts,_endpoint;
        int _packet_duration;
        SwrContext* _resample_context;
        uint8_t **converted_input_samples ;
        AVAudioFifo* _fifo;
        void _init_converted_samples(uint8_t***,int );
        void  _convert_samples(const uint8_t**,uint8_t**, const int );
        void  _encode_audio_frame(AVFrame*,int*,int64_t);
        void _add_samples_to_fifo(uint8_t**,const int);
        void _init_output_frame(AVFrame**,AVCodecContext*,int);
        void _calculatePacketDuration();
    };
}
#endif