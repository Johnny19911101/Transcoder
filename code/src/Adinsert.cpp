#include "Adinsert.h"

using namespace Noovo;
Adinsert::Adinsert(std::string input){
    try{
        int ret;
        if ((ret = avformat_open_input(&_ifmt_ctx, input.c_str(), 0, 0)) < 0) {
            throw std::runtime_error("Cannot alloc stream input");
        }
        if ((ret = avformat_find_stream_info(_ifmt_ctx, 0)) < 0) {
            throw std::runtime_error("Cannot find stream information");          
        }
        for (int i = 0; i <(_ifmt_ctx)->nb_streams; i++) {
            _index_type.insert(std::make_pair(i,_ifmt_ctx->streams[i]->codecpar->codec_type));
        }
    }catch(...){

    }
}
Adinsert::~Adinsert(){  
    std::cout << "free the ifmt_ctx here" << std::endl;
    avformat_close_input(&_ifmt_ctx);
}
std::thread Adinsert::Process_thread(std::shared_ptr<Ofmt_list> fmt_wrapper){
    return std::thread(&Adinsert::_process,this,fmt_wrapper);
}
void Adinsert::_process(std::shared_ptr<Ofmt_list> fmt_wrapper){
    AVFormatContext* ofmt_ctx=fmt_wrapper->ReturnAvformat();
    int video_index = fmt_wrapper-> Returnvideo();
    int audio_index = fmt_wrapper-> Returnaudio();
    AVStream *video_outstream = fmt_wrapper->ReturnAvformat()->streams[fmt_wrapper->Returnvideo()];
    AVStream *audio_outstream = fmt_wrapper->ReturnAvformat()->streams[fmt_wrapper->Returnaudio()];
    int64_t starttime = fmt_wrapper->Returnconcate(),concatetime;
    AVPacket pkt;
    while(1){
        _initpacket(&pkt);
        int ret = av_read_frame(_ifmt_ctx, &pkt);
        if (ret < 0)
            break;
        int i=pkt.stream_index;
        pkt.pts +=starttime;
        pkt.dts +=starttime;
        if(_index_type[pkt.stream_index]==AVMEDIA_TYPE_AUDIO){
            pkt.pts = av_rescale_q_rnd(pkt.pts, _ifmt_ctx->streams[i]->time_base, audio_outstream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
            pkt.dts = av_rescale_q_rnd(pkt.dts,  _ifmt_ctx->streams[i]->time_base, audio_outstream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
            pkt.duration = av_rescale_q(pkt.duration, _ifmt_ctx->streams[i]->time_base, audio_outstream->time_base);
            pkt.stream_index=audio_index;
            concatetime=pkt.pts;
        }else if(_index_type[pkt.stream_index]==AVMEDIA_TYPE_VIDEO){
            pkt.pts = av_rescale_q_rnd(pkt.pts, _ifmt_ctx->streams[i]->time_base, video_outstream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
            pkt.dts = av_rescale_q_rnd(pkt.dts,  _ifmt_ctx->streams[i]->time_base, video_outstream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
            pkt.duration = av_rescale_q(pkt.duration, _ifmt_ctx->streams[i]->time_base, video_outstream->time_base);            
            pkt.stream_index=video_index;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
        fmt_wrapper->GetConcatpoint(concatetime);
        av_free_packet(&pkt);   
    }
}
void Adinsert::_initpacket(AVPacket* packet){
    av_init_packet(packet);
    /* Set the packet data and size so that it is recognized as being empty. */
    packet->data = NULL;
    packet->size = 0;
}