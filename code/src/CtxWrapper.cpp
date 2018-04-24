#include "CtxWrapper.h"
#include <algorithm>
#include <string>
#include "Video.h"
#include "Audio.h"
using namespace Noovo;
#define OUTPUT_BIT_RATE 48000
/** The number of output channels */
#define OUTPUT_CHANNELS 2
/** The audio sample output format */
#define OUTPUT_SAMPLE_FORMAT AV_SAMPLE_FMT_S16
#define VIDEO_ENCODER "h264_omx"
#define AUDIO_ENCODER "libfdk_aac"
CtxWrapper::CtxWrapper(){

}
CtxWrapper::~CtxWrapper(){

}
void CtxWrapper::SetAvio(int avio_ctx_buffer_size,void *call_back_var,int(*ffmpeg_callback)(void *opaque, uint8_t *buf, int buf_size)
                        ,const std::string& output,const std::vector<std::pair<int,int> >& pid_pair
                        ,std::unordered_map<int,std::shared_ptr<Stream> >& Pid_Obj
                        , std::vector<std::shared_ptr<Ofmt_list>>& fmtwrapper_list){
    try{
        if(!(_ifmt_ctx = avformat_alloc_context())){
            throw std::runtime_error("Cannot alloc information ");
        } 
        unsigned char *avio_ctx_buffer = nullptr;
        avio_ctx_buffer = (unsigned char*)av_malloc(avio_ctx_buffer_size);
        _avio_ctx =avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size,
                                  0, call_back_var, ffmpeg_callback, nullptr, 0);
        _ifmt_ctx -> pb = _avio_ctx;
        std::vector<int> Pids;
        for(auto it= pid_pair.begin();it!=pid_pair.end();++it){
            Pids.push_back(it->first);
            Pids.push_back(it->second);
        }
        if(_ifmtInital(nullptr,Pids)<0)
            throw std::runtime_error("Cannot find stream information");
        int i = 0;
        for(auto it= pid_pair.begin();it!=pid_pair.end();++it){
            std::string outputfile(std::to_string(i)+output);
            if(_ofmtInital(outputfile,it->first,it->second,Pid_Obj,fmtwrapper_list)<0)
             throw std::runtime_error("Cannot find stream information ");
            i++;
        }
    }catch(std::exception const& e) {
        std::cout << "Exception: " << e.what() ;
    } 
}
int CtxWrapper::SetConfig(const std::string& inputfile,const std::string& output,const std::vector<std::pair<int,int> >& pid_pair
                            ,std::unordered_map<int,std::shared_ptr<Stream> >& Pid_Obj
                           , std::vector<std::shared_ptr<Ofmt_list>>& fmtwrapper_list){
    try{
        std::vector<int> Pids;
        for(auto it= pid_pair.begin();it!=pid_pair.end();++it){
            Pids.push_back(it->first);
            Pids.push_back(it->second);
        }
        if(_ifmtInital(inputfile.c_str(),Pids)<0)
            throw std::runtime_error("Cannot find stream information");
        int i = 0;
        for(auto it= pid_pair.begin();it!=pid_pair.end();++it){
            std::string outputfile= output+std::to_string(i)+".m3u8";
            if(_ofmtInital(outputfile,it->first,it->second,Pid_Obj,fmtwrapper_list)<0)
             throw std::runtime_error("Cannot find stream information ");
        }
        return 0;
    }catch(std::exception const& e) {
        std::cout << "Exception: " << e.what() ;
        return -1;
    } 
}
int CtxWrapper::_ifmtInital(const char* filename,const std::vector<int>& pids){
    try{    
        int ret;
        if ((ret = avformat_open_input(&_ifmt_ctx, filename, NULL, NULL)) < 0) 
            throw std::runtime_error("Cannot open input file");
        if ((ret = avformat_find_stream_info(_ifmt_ctx, NULL)) < 0) 
            throw std::runtime_error("Cannot find stream information");
        for (int i = 0; i <(_ifmt_ctx)->nb_streams; i++) {
            if(find(pids.begin(),pids.end(),(_ifmt_ctx)->streams[i]->id) != pids.end())
                _findDecoder(i);
        }
        av_dump_format(_ifmt_ctx, 0, filename, 0);
        return 0;
    }catch(std::exception const& e) {
        std::cout << "Exception: " << e.what() ;
        _ifmtclean();
        return -1;
    } 
}
void CtxWrapper::_findDecoder(int streamid){
    AVStream *stream = _ifmt_ctx->streams[streamid];
    AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
    AVCodecContext *codec_ctx;
    if (!dec) {
        throw std::runtime_error("Failed to find decoder for stream ");
    }
    codec_ctx = avcodec_alloc_context3(dec);
    if (!codec_ctx) {
        throw std::runtime_error("Failed to allocate the decoder context for stream");
    }
    int ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
    if (ret < 0) {
        throw std::runtime_error("Failed to copy decoder parameters to input decoder context");
    }
    /* Reencode video & audio and remux subtitles etc. */
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
            codec_ctx->framerate = av_guess_frame_rate(_ifmt_ctx, stream, NULL);
        /* Open decoder */
        ret = avcodec_open2(codec_ctx, dec, NULL);
        if (ret < 0) {
            throw std::runtime_error("Failed to open decoder for stream");
        }
    }
    _ifmt_ctx->streams[streamid]->codec=codec_ctx;
}
int  CtxWrapper::_ofmtInital(const std::string& filename,int pid_first,int pid_sec
                            ,std::unordered_map<int,std::shared_ptr<Stream> >& Pid_Obj
                            ,std::vector<std::shared_ptr<Ofmt_list>>& fmtwrapper_list){
    AVFormatContext *ofmt_ctx = nullptr;
    try{
        AVStream *out_stream = nullptr,*in_stream = nullptr;
        AVCodecContext *dec_ctx =nullptr, *enc_ctx = nullptr;
        AVCodec *encoder = nullptr;
        int64_t video_starttime = 0;
        int stream_count=0,ret;
        std::shared_ptr<Audio> Pid_audio = std::make_shared<Audio>();
        std::shared_ptr<Video> Pid_video = std::make_shared<Video>();
        std::shared_ptr<Ofmt_list> fmt_wrapper=std::make_shared<Ofmt_list>(); 
        avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename.c_str());
        if (!ofmt_ctx) 
            throw std::runtime_error("Could not create output context\n");
        for (int i = 0; i < _ifmt_ctx->nb_streams; i++) {
            if(_ifmt_ctx->streams[i]->id == pid_first || _ifmt_ctx->streams[i]->id == pid_sec){               
                out_stream = avformat_new_stream(ofmt_ctx, NULL);
                if (!out_stream)              
                    throw std::runtime_error("Failed allocating output stream\n");              
                in_stream = _ifmt_ctx->streams[i];
                dec_ctx = _ifmt_ctx->streams[i]->codec;
                if(dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO){
                    _videoEncoder(&enc_ctx,dec_ctx,&encoder);   
                    Pid_video -> SetTool(_ifmt_ctx->streams[i],stream_count,ofmt_ctx); 
                    Pid_Obj.insert(std::make_pair(i,Pid_video));
                    video_starttime = _ifmt_ctx->streams[i]->start_time;
                    fmt_wrapper->SetVideo(stream_count);
                }     
                else if(dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO){
                   _audioEncoder(&enc_ctx,dec_ctx,&encoder); 
                    Pid_audio -> SetTool(_ifmt_ctx->streams[i],stream_count,ofmt_ctx);
                    Pid_Obj.insert(std::make_pair(i,Pid_audio));
                    fmt_wrapper->SetAudio(stream_count);
                }else{
                    throw std::runtime_error("pid not video or audio\n");
                }
                ret = avcodec_open2(enc_ctx, encoder, NULL);
                if (ret < 0)                
                    throw std::runtime_error("Cannot open video encoder for stream \n");                        
                ret = avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);
                if (ret < 0)          
                    throw std::runtime_error("Failed to copy encoder parameters to output stream \n");                   
                if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
                    enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
                out_stream->time_base = in_stream->time_base;
                out_stream->codec = enc_ctx;
                stream_count++;      
            }
        }     
        Pid_audio->SetTime(video_starttime,0);//為了切齊video跟audio且讓影片從零開始
        Pid_video->SetTime(video_starttime,0);
        _option(ofmt_ctx);
        fmt_wrapper->SetAvformat(ofmt_ctx);
        av_dump_format(ofmt_ctx, 0, filename.c_str(),1);
        _ofmtheader(ofmt_ctx,filename);
        fmt_wrapper->SetStream(Pid_audio,Pid_video);
        fmtwrapper_list.push_back(fmt_wrapper); 
        return 0;       
    }catch(std::exception const& e) {
        std::cout << "Exception: " << e.what() ;
        _oneofmtclean(ofmt_ctx);
        return -1;
    }    
}
void  CtxWrapper::_option(AVFormatContext *ctx){
        av_opt_set_int(ctx->priv_data,"hls_list_size",10,0);
        av_opt_set_int(ctx->priv_data,"hls_time",10,0);
        av_opt_set(ctx->priv_data,"hls_flags","split_by_time",0);
        av_opt_set(ctx->priv_data,"hls_flags","delete_segments",0);
}
void  CtxWrapper::_ofmtheader(AVFormatContext*& ofmt_ctx,const std::string& filename){
    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        int ret = avio_open(&ofmt_ctx->pb, filename.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0)     
            throw std::runtime_error("Could not open output file\n");            
    }
    /* init muxer, write output file header */
    int ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0)           
        throw std::runtime_error("Error occurred when opening output file\n");          
}
void CtxWrapper::_videoEncoder(AVCodecContext** enc_ctx,AVCodecContext*& dec_ctx,AVCodec** encoder){  
    
    (*encoder) = avcodec_find_encoder_by_name(VIDEO_ENCODER);                                        
    if (!*encoder) {
        throw std::runtime_error("Necessary encoder not found\n");
    }
    (*enc_ctx) = avcodec_alloc_context3(*encoder);
    if (!enc_ctx) {                    
        throw std::runtime_error("Failed to allocate the encoder context\n");                        
    }
    (*enc_ctx)->height = dec_ctx->height;
    (*enc_ctx)->width = dec_ctx->width;
    (*enc_ctx)->sample_aspect_ratio = dec_ctx->sample_aspect_ratio;
    /* take first format from list of supported formats */   
    if ((*encoder)->pix_fmts)
        (*enc_ctx)->pix_fmt = (*encoder)->pix_fmts[0];
    else
        (*enc_ctx)->pix_fmt = dec_ctx->pix_fmt;
    /* video time_base can be set to whatever is handy and supported by encoder */
    (*enc_ctx)->time_base = av_inv_q(dec_ctx->framerate);
    /* Third parameter can be used to pass settings to encoder */  
}
void CtxWrapper::_audioEncoder(AVCodecContext** enc_ctx,AVCodecContext*& dec_ctx,AVCodec** encoder){  
    (*encoder) = avcodec_find_encoder_by_name(AUDIO_ENCODER);             
    if (!*encoder) {
        throw std::runtime_error("Necessary encoder not found\n");
    }
    (*enc_ctx) = avcodec_alloc_context3(*encoder);
    if (!enc_ctx) {                    
        throw std::runtime_error("Failed to allocate the encoder context\n");                        
    }
    /* In this example, we transcode to same properties (picture size,
    * sample rate etc.). These properties can be changed for output
    * streams easily using filters */
    (*enc_ctx)->channels       = OUTPUT_CHANNELS;
    (*enc_ctx)->channel_layout = av_get_default_channel_layout(OUTPUT_CHANNELS);
    (*enc_ctx)->sample_rate    = dec_ctx->sample_rate;
    (*enc_ctx)->sample_fmt     = (*encoder)->sample_fmts[0];
    (*enc_ctx)->bit_rate       = OUTPUT_BIT_RATE;
    (*enc_ctx)->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    /* Third parameter can be used to pass settings to encoder */
}
void CtxWrapper::_ifmtclean(){
    if (_avio_ctx) {
        av_freep(&_avio_ctx->buffer);
        av_freep(&_avio_ctx);
    }
    for (int i = 0; i < _ifmt_ctx->nb_streams; i++) {
        if(_ifmt_ctx->streams[i]->codec){
           avcodec_close(_ifmt_ctx->streams[i]->codec);
           avcodec_free_context(&_ifmt_ctx->streams[i]->codec);
        }
    }
    avformat_close_input(&_ifmt_ctx);
}

void CtxWrapper::_oneofmtclean(AVFormatContext* ofmt_ctx){
    
    for(int i = 0;i < (ofmt_ctx)->nb_streams;++i){
         avcodec_close((ofmt_ctx)->streams[i]->codec);  
         avcodec_free_context(&((ofmt_ctx)->streams[i]->codec));
    }
    if ( !((ofmt_ctx)->oformat->flags & AVFMT_NOFILE))
        avio_close((ofmt_ctx)->pb);
    avformat_free_context(ofmt_ctx);
}
void CtxWrapper::Cleanup(std::vector<std::shared_ptr<Ofmt_list>>& fmtwrapper_list){
    _ifmtclean();
    for(auto it = fmtwrapper_list.begin();it !=fmtwrapper_list.end();++it)  
        _oneofmtclean((*it)->ReturnAvformat());
}
AVFormatContext* CtxWrapper::GetIfmt(){
   return _ifmt_ctx;
}
void CtxWrapper::Switch(int avio_ctx_buffer_size,void *call_back_var,int(*ffmpeg_callback)(void *opaque, uint8_t *buf, int buf_size)
                        ,std::pair<int,int> Pairs,std::shared_ptr<Ofmt_list> fmt_wrapper,std::unordered_map<int,std::shared_ptr<Stream>>& Pid_object){
    try{   
        _ifmtclean();
        _ifmt_ctx = nullptr;
        if(!(_ifmt_ctx = avformat_alloc_context())){
            throw std::runtime_error("Cannot alloc information ");
        } 
        unsigned char *avio_ctx_buffer = nullptr;
        avio_ctx_buffer = (unsigned char*)av_malloc(avio_ctx_buffer_size);
        _avio_ctx =avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size,
                                  0, call_back_var, ffmpeg_callback, nullptr, 0);
        _ifmt_ctx -> pb = _avio_ctx;
        std::vector<int> Pids{Pairs.first,Pairs.second};
        if(_ifmtInital(nullptr,Pids)<0){
            throw std::runtime_error("Cannot find stream information");
        }
        int64_t start_time; 
        std::shared_ptr<Audio> Pid_audio = std::make_shared<Audio>();
        std::shared_ptr<Video> Pid_video= std::make_shared<Video>();
        for (int i = 0; i <(_ifmt_ctx)->nb_streams; i++) {
            if(find(Pids.begin(),Pids.end(),(_ifmt_ctx)->streams[i]->id) != Pids.end()){
                if(_ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
                    Pid_audio->SetTool(_ifmt_ctx->streams[i],fmt_wrapper->Returnaudio(),fmt_wrapper->ReturnAvformat());
                    Pid_object.insert(std::make_pair(i,Pid_audio));
                }else if(_ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
                    Pid_video->SetTool(_ifmt_ctx->streams[i],fmt_wrapper->Returnvideo(),fmt_wrapper->ReturnAvformat());
                    start_time=_ifmt_ctx->streams[i]->start_time;
                    Pid_object.insert(std::make_pair(i,Pid_video));
                }
            }
        }
        Pid_audio->SetTime(start_time,fmt_wrapper->Returnconcate());
        Pid_video->SetTime(start_time,fmt_wrapper->Returnconcate());
        fmt_wrapper->SetStream(Pid_audio,Pid_video);
    }catch(std::exception const& e) {
        std::cout << "Exception: " << e.what() ;
        _ifmtclean();
    } 
}