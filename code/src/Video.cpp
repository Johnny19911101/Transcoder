#include "Video.h"
using namespace Noovo;
int64_t Video::_count_frames = 0;
Video::Video(){

}
Video::~Video(){

}
void Video::SetTool(AVStream* input_stream,int outputindex ,AVFormatContext* ofmt_ctx){
    _input_stream = input_stream;
    _video_index = outputindex;
    _ofmt_ctx = ofmt_ctx;
}
void Video::SetTime(int64_t start_pts,int64_t mux_pts){
    _start_time = start_pts;
    _mux_pts=mux_pts;
    _caluculateduration();
}
void Video::_caluculateduration(){
    _duration = _ofmt_ctx->streams[_video_index]->time_base.den*av_q2d(_ofmt_ctx->streams[_video_index]->codec->time_base);
}
int Video::VideoDecoder(AVPacket* packet){
    try{

        av_packet_rescale_ts(packet,
                                 _input_stream->time_base,
                                 _ofmt_ctx->streams[_video_index]->codec->time_base);
        _decode_frame = nullptr;

        _decode_frame = av_frame_alloc();

        if (!_decode_frame) {
                throw std::runtime_error("Decoding frame initalize error");
            }

        int ret =  avcodec_decode_video2(_input_stream->codec,_decode_frame,&_got_frame,packet);
       
        if(ret < 0){
            throw std::runtime_error("Decoding failed");
        }
        return 0;
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        _got_frame = 0;
        av_frame_free(&_decode_frame);
        return -1 ;
    } 
}
void Video::Codecname(){
    std::cout << "Input Stream Codec " << _input_stream->codec->codec->name<<std::endl;
    std::cout << "Output Stream Codec " << _ofmt_ctx->streams[_video_index]->codec->codec->name<<std::endl;
    std::cout << std::boolalpha << _interlaced << std::endl;
}

int Video::Encode_write_frame(AVPacket* enc_pkt) {
    try{
        int ret;
        if(_got_frame){        
            _decode_frame->pict_type = AV_PICTURE_TYPE_NONE;
          
            ret = avcodec_encode_video2(_ofmt_ctx->streams[_video_index]->codec,enc_pkt,_decode_frame, &_got_frame);

            av_frame_free(&_decode_frame);
            if (ret < 0)
                throw std::runtime_error("Encoding failed");      
    
            enc_pkt->stream_index = _video_index;
            av_packet_rescale_ts(enc_pkt,
                                _ofmt_ctx->streams[_video_index]->codec->time_base,
                                _ofmt_ctx->streams[_video_index]->time_base);
            
            if(enc_pkt->pts!=AV_NOPTS_VALUE){
                _endpoint=enc_pkt->pts+_duration;
            }
            ret = av_interleaved_write_frame(_ofmt_ctx, enc_pkt);
            if (ret < 0)
                throw std::runtime_error("Write frame error");
            if (!(_got_frame))
                throw std::runtime_error("Encoding failed");
        }else{
            av_frame_free(&_decode_frame);
        }          
        return 0;
        /* prepare packet for muxing */
        /* mux encoded frame */
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        return -1 ;
    } 
}
void Video::FlushEncoder(){
    AVPacket enc_pkt;  
    while (1) {      
        av_init_packet(&enc_pkt);
        enc_pkt.data = nullptr;
        enc_pkt.size = 0 ;  
        int ret = avcodec_encode_video2(_ofmt_ctx->streams[_video_index]->codec,&enc_pkt,NULL, &_got_frame);
        if (ret < 0)
            break;
        if (!_got_frame)
            break ;       
        av_packet_rescale_ts(&enc_pkt, _ofmt_ctx->streams[_video_index]->codec->time_base, _ofmt_ctx->streams[_video_index]->time_base);
        enc_pkt.stream_index = 0;
        ret = av_interleaved_write_frame(_ofmt_ctx, &enc_pkt);
        av_free_packet(&enc_pkt);
    }
}
void Video::_resetpts(AVPacket* packet){
    packet->pts = packet->pts-_start_time;
    packet->dts = packet->dts-_start_time;
}
int Video::TranscodeFlow(AVPacket* packet){
    AVPacket enc;
    try{    
        av_init_packet(&enc);
        enc.data = NULL;
        enc.size = 0;
        _resetpts(packet);
        packet->pts +=_mux_pts;
        int ret = VideoDecoder(packet);
        if(ret < 0){           
            throw std::runtime_error("Error decodeing packet in function TranscodeFlow");
        } 
        ret = Encode_write_frame(&enc);
        if(ret < 0){
            throw std::runtime_error("Error encoding packet in function TranscodeFlow");
        } 
        av_free_packet(&enc);
#ifdef TIMER
        _count_frames++;
#endif
        return 0;
    }
    catch(std::exception const& e){   
        av_free_packet(&enc);
        std::cout << "Exception: " << e.what() << "\n";
        return -1;
    }
}
void Video::MuxFlow(AVPacket* pkt){
    try{
        AVStream *in_stream = _input_stream;
        AVStream *out_stream = _ofmt_ctx->streams[_video_index];
        if(pkt->dts == AV_NOPTS_VALUE){
             last_correct_dts += pkt->duration;
             pkt->dts = last_correct_dts;
        }else{
            last_correct_dts = pkt->dts;                
        }
        if(pkt->pts == AV_NOPTS_VALUE){
             last_correct_pts += pkt->duration;
             pkt->pts = last_correct_pts;
        }else{
            last_correct_pts = pkt->pts;                
        }
  
        pkt->pts = av_rescale_q_rnd(pkt->pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        pkt->dts = av_rescale_q_rnd(pkt->dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        pkt->duration = av_rescale_q(pkt->duration, in_stream->time_base, out_stream->time_base);
        pkt->pos = -1;
        pkt->stream_index= _video_index;
        int ret = av_interleaved_write_frame(_ofmt_ctx, pkt);
        if (ret < 0) {
            std::cout << "dts : "<<pkt->dts<<std::endl;
            throw std::runtime_error("Error muxing packet");
        }
    }catch(std::exception const& e){
        std::cout << "Exception: " << e.what() << "\n";
    }
}
int Video::Flow(AVPacket* packet){
    if(_interlaced){
        TranscodeFlow(packet);
    }
    else{
        MuxFlow(packet);
    }
    return 0;
}
void Video::InitalTool(){

}
AVFrame* Video::ReturnFrame(AVPacket *packet){
    try{    
        int ret = VideoDecoder(packet);
        if(ret < 0){
            throw std::runtime_error("Error decodeing packet in function TranscodeFlow");
        } 
        return _decode_frame;
     }catch(std::exception const& e){
        std::cout << "Exception: " << e.what() << "\n";
        return nullptr;
    }
}
int64_t Video::ReturnAmount(){
    return _count_frames;
}
int64_t Video::ReturnEndPoint(){
    return _endpoint;
}
void Video::ResetAmount(){
    _count_frames = 0;
}