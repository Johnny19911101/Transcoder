#include <stdexcept>
#include <iostream>
#include "Transcoder.h"
#include "Stream.h"
#include "Video.h"
#include "Audio.h"
#define BUFFER_SIZE 188*100
using namespace Noovo;
std::unique_ptr<Transcoder> Transcoder::_instance;
std::once_flag Transcoder::_transcoder_flag;

static void init_packet(AVPacket *packet){
    av_init_packet(packet);
    /* Set the packet data and size so that it is recognized as being empty. */
    packet->data = NULL;
    packet->size = 0;
}
static int read_packet(void *opaque, uint8_t *buf, int buf_size){   
    struct buffer_data *bd = (struct buffer_data *)opaque;
    buf_size = FFMIN(buf_size, bd->size);
    printf("ptr:%p size:%zu\n", bd->ptr, bd->size);
    /* copy internal buffer data to buf */
    memcpy(buf, bd->ptr, buf_size);
    bd->ptr  += buf_size;
    bd->size -= buf_size;
    return buf_size;
}
Transcoder::Transcoder() : ifmt_ctx(nullptr),avio_ctx(nullptr),avio_ctx_buffer(nullptr),_state(new Status()){      
        av_register_all();
	    avfilter_register_all();
        _state->Initialize();//Statusinital
        _state->SetState(typeid(Status::Initial));
}

Transcoder& Transcoder::Instance(){
    std::call_once(_transcoder_flag,[](){
		Transcoder::_instance.reset(new Transcoder);       
	});
	return *(_instance.get());
}

void Transcoder::InitalAvio(buffer_data* bd){
    int ret;
    if(ret < 0){
        std::cout << "Cannot open file"<<std::endl;
    }
    if (!(ifmt_ctx = avformat_alloc_context())) {
        std::cout << "Cannot alloc context"<<std::endl;
    }
    avio_ctx_buffer =(uint8_t*) av_malloc(BUFFER_SIZE);

    if (!avio_ctx_buffer) {
        std::cout << "Cannot malloc"<<std::endl;
    }
    avio_ctx = avio_alloc_context(avio_ctx_buffer, BUFFER_SIZE,0, bd, &read_packet, NULL, NULL);
    if (!avio_ctx) {
        ret = AVERROR(ENOMEM);
    }
    ifmt_ctx->pb = avio_ctx;
    InputFile(nullptr);
}

void Transcoder::InputFile(const char* filename){
//If you want to use buffer to input , just pass nullptr filename
    try{
        _state->SetState(typeid(Status::SetConfig));
        std::lock_guard<std::mutex> temp_lock(_lock_process);	
         /*mutex lock*/
        int ret;
        unsigned int i;
        if ((ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL)) < 0) {
            throw std::runtime_error("Cannot open input file");
        }
        if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
           throw std::runtime_error("Cannot find stream information");
        }
        for (i = 0; i < ifmt_ctx->nb_streams; i++) {
            AVStream *stream = ifmt_ctx->streams[i];
            AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
            AVCodecContext *codec_ctx;
            if (!dec) {
                throw std::runtime_error("Failed to find decoder for stream ");
            }
            codec_ctx = avcodec_alloc_context3(dec);
            if (!codec_ctx) {
                throw std::runtime_error("Failed to allocate the decoder context for stream");
            }
            ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
            if (ret < 0) {
                throw std::runtime_error("Failed to copy decoder parameters to input decoder context");
            }
            /* Reencode video & audio and remux subtitles etc. */
            if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
                if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
                    codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, NULL);
                /* Open decoder */
                ret = avcodec_open2(codec_ctx, dec, NULL);
                if (ret < 0) {
                    throw std::runtime_error("Failed to open decoder for stream");
                }
            }
            ifmt_ctx->streams[i]->codec=codec_ctx;
       
        }
        av_dump_format(ifmt_ctx, i, filename, 0);
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        avformat_close_input(&ifmt_ctx);
    } 
}
void Transcoder::OutputFile(const char *filename,int pid_video,int pid_audio){
    try{
        _state->SetState(typeid(Status::SetConfig));
        std::lock_guard<std::mutex> temp_lock(_lock_process);
        /*Lock the process*/
        AVFormatContext* ofmt_ctx = nullptr;
        AVStream *out_stream = nullptr;
        AVStream *in_stream = nullptr;
        AVCodecContext *dec_ctx =nullptr, *enc_ctx = nullptr;
        AVCodec *encoder = nullptr;
        int64_t audio_starttime = 0,dts_starttime=0;
        int ret;
        int stream_count=0;
        unsigned int i;
        auto Pid_video = std::make_shared<Video>();
        auto Pid_audio = std::make_shared<Audio>();
        ofmt_ctx = NULL;
        avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename);
        if (!ofmt_ctx) {
            throw std::runtime_error("Could not create output context\n");
        }
        for (i = 0; i < ifmt_ctx->nb_streams; i++) {
            if(ifmt_ctx->streams[i]->id == pid_video ||ifmt_ctx->streams[i]->id == pid_audio ){               
                out_stream = avformat_new_stream(ofmt_ctx, NULL);
                if (!out_stream) {               
                    throw std::runtime_error("Failed allocating output stream\n");              
                }
                in_stream = ifmt_ctx->streams[i];
                dec_ctx = ifmt_ctx->streams[i]->codec;
                if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO || dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
                    /* in this example, we choose transcoding to same codec */
                    if(dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO){
                        encoder = avcodec_find_encoder_by_name("libx264");                             
                        Pid_video -> SetTool(ifmt_ctx->streams[i],stream_count,ofmt_ctx);                       
                        }
                    else if(dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO){
                        encoder = avcodec_find_encoder_by_name("libfdk_aac");             
                        Pid_audio -> SetTool(ifmt_ctx->streams[i],stream_count,ofmt_ctx);
                        audio_starttime = ifmt_ctx->streams[i]->start_time;
                        dts_starttime =  ifmt_ctx->streams[i]->first_dts;
                    }
                    else{
                        throw std::runtime_error("Pid is not audio or video\n");
                    }
                    if (!encoder) {
                        throw std::runtime_error("Necessary encoder not found\n");
                    }
                    enc_ctx = avcodec_alloc_context3(encoder);
                    if (!enc_ctx) {                    
                        throw std::runtime_error("Failed to allocate the encoder context\n");                        
                    }
                    /* In this example, we transcode to same properties (picture size,
                    * sample rate etc.). These properties can be changed for output
                    * streams easily using filters */
                    if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
                        enc_ctx->height = dec_ctx->height;
                        enc_ctx->width = dec_ctx->width;
                        enc_ctx->sample_aspect_ratio = dec_ctx->sample_aspect_ratio;
                        /* take first format from list of supported formats */   
                        if (encoder->pix_fmts)
                            enc_ctx->pix_fmt = encoder->pix_fmts[0];
                        else
                            enc_ctx->pix_fmt = dec_ctx->pix_fmt;
                        _pidObject.insert(std::make_pair(i,Pid_video));
                        /* video time_base can be set to whatever is handy and supported by encoder */
                        enc_ctx->time_base = av_inv_q(dec_ctx->framerate);
                    } else if(dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
                            enc_ctx->channels       = OUTPUT_CHANNELS;
                            enc_ctx->channel_layout = av_get_default_channel_layout(OUTPUT_CHANNELS);
                            enc_ctx->sample_rate    = dec_ctx->sample_rate;
                            enc_ctx->sample_fmt     = encoder->sample_fmts[0];
                            enc_ctx->bit_rate       = OUTPUT_BIT_RATE;
                            enc_ctx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
                            out_stream->time_base.den = dec_ctx->sample_rate;
                            out_stream->time_base.num = 1;
                            _pidObject.insert(std::make_pair(i,Pid_audio));
                    }               
                   /* Third parameter can be used to pass settings to encoder */
                    ret = avcodec_open2(enc_ctx, encoder, NULL);
                    if (ret < 0) {                  
                        throw std::runtime_error("Cannot open video encoder for stream \n");                        
                    }
                    ret = avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);
                    if (ret < 0) {           
                        throw std::runtime_error("Failed to copy encoder parameters to output stream \n");                   
                    }
                    if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
                        enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

                    out_stream->time_base = enc_ctx->time_base;
                    ofmt_ctx->streams[stream_count]->codec = enc_ctx;
                    ++stream_count;
                    //這邊不能用i應該要手動去加
                } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {                
                    throw std::runtime_error("Elementary stream #%d is of unknown type, cannot proceed\n");
                } else {
                    /* if this stream must be remuxed */
                    ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
                    if (ret < 0) {
                        throw std::runtime_error("Copying parameters for stream failed\n");
                     }
                    out_stream->time_base = in_stream->time_base;
                }
            }
            Pid_video->SetTime(audio_starttime,dts_starttime) ;
        }     
        av_dump_format(ofmt_ctx, 0, filename, 1);        
        if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
            ret = avio_open(&ofmt_ctx->pb, filename, AVIO_FLAG_WRITE);
            if (ret < 0) {    
                throw std::runtime_error("Could not open output file\n");            
            }
        }
        /* init muxer, write output file header */
        ret = avformat_write_header(ofmt_ctx, NULL);
        if (ret < 0) {          
            throw std::runtime_error("Error occurred when opening output file\n");          
        }
        ofmt_list.push_back(ofmt_ctx);
    }
    catch(std::exception const& e) {
        std::cout << "Exception: " << e.what() ;
    } 
}
int Transcoder::OutputAmount(){
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
        std::cout << "pid : " << it->first <<std::endl;
        it->second->Codecname();
    }
    return _pidObject.size();
    _state->SetState(typeid(Status::SetConfigFinished));
}
void Transcoder::Flow(){
    AVPacket packet;
    int ret=0;
    init_packet(&packet);
    _state->SetState(typeid(Status::Process));
    std::lock_guard<std::mutex> temp_lock(_lock_process);
     /*mutex lock*/
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
        std::cout << it->first <<std::endl;
        it->second->InitalTool();
    }
    while (1) {
        // Use the encoder's desired frame size for processing. 
        if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
            break; 
        auto it = _pidObject.find(packet.stream_index);
        if(it !=_pidObject.end()){
            it->second->Flow(&packet);
        }
        av_packet_unref(&packet);
    }  
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
        it->second->FlushEncoder();
    }
    for(auto it = ofmt_list.begin();it!=ofmt_list.end();++it){
        av_write_trailer(*it);
    }
    av_free_packet(&packet);
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
        it->second->CleanUp();
    }
    _state->SetState(typeid(Status::Finished));
 }
void Transcoder::CleanUp(){
    std::lock_guard<std::mutex> temp_lock(_lock_process);	
    /*mutex lock*/
    _state->SetState(typeid(Status::Reset));
    for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
        avcodec_close(ifmt_ctx->streams[i]->codec);
    }
    avformat_close_input(&ifmt_ctx);
    ifmt_ctx = nullptr;//全部reset並指向null
    for(auto it = ofmt_list.begin();it!=ofmt_list.end();++it){
        if ( !((*it)->oformat->flags & AVFMT_NOFILE))
            avio_close((*it)->pb);
        for(int i = 0;i < (*it)->nb_streams;++i){
            avcodec_close((*it)->streams[i]->codec);
        }
        avformat_free_context(*it);
    }
    ofmt_list.clear();//destroy vector object
    _pidObject.clear();//destroy unordered map 
    _state->SetState(typeid(Status::Idle));
}
std::string Transcoder::ReturnStateName(){
    return _state->GetCurrentName();
}