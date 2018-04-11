#include "Audio.h"
#include <iomanip>
using namespace Noovo;
Audio::Audio(){

}
Audio::~Audio(){

}
void Audio::Codecname(){
    std::cout << "Input Stream Codec " << _input_stream->codec->codec->name<<std::endl;
    std::cout << "Output Stream Codec " << _ofmt_ctx->streams[_audio_index]->codec->codec->name<<std::endl;
}
void Audio::SetTool(AVStream* input_stream,int outputindex ,AVFormatContext* ofmt_ctx){
    _input_stream = input_stream;
    _audio_index = outputindex;
    _ofmt_ctx = ofmt_ctx;
}
void Audio::_calculatePacketDuration(){
    double duration_time((double)_ofmt_ctx->streams[_audio_index]->codec->frame_size/(double)_ofmt_ctx->streams[_audio_index]->codec->sample_rate);
    _packet_duration = duration_time*_ofmt_ctx->streams[_audio_index]->time_base.den;
}
void Audio::InitalTool(){
    InitResampler();
    InitFifo();
    _calculatePacketDuration();
}
int Audio::InitResampler(){
    try{
        int error;
        /*
         * Create a resampler context for the conversion.
         * Set the conversion parameters.
         * Default channel layouts based on the number of channels
         * are assumed for simplicity (they are sometimes not detected
         * properly by the demuxer and/or decoder).
         */
        AVCodecContext *encoder = _ofmt_ctx->streams[_audio_index]->codec;
        AVCodecContext *decoder = _input_stream->codec;

        _resample_context = swr_alloc_set_opts(NULL,
                                              av_get_default_channel_layout(encoder->channels),
                                              encoder->sample_fmt,
                                              encoder->sample_rate,
                                              av_get_default_channel_layout(decoder->channels),
                                              decoder->sample_fmt,
                                              decoder->sample_rate,
                                              0, NULL);
        if (!_resample_context) {
            throw std::runtime_error("Could not allocate resample context\n");
        }
        /*
        * Perform a sanity check so that the number of converted samples is
        * not greater than the number of samples to be converted.
        * If the sample rates differ, this case has to be handled differently
        */
        
        av_assert0(_ofmt_ctx->streams[_audio_index]->codec->sample_rate == _input_stream->codec->sample_rate);

        /* Open the resampler with the specified parameters. */
        if ((error = swr_init(_resample_context)) < 0) {
            
            swr_free(&_resample_context);
            throw std::runtime_error("Could not open resample context\n");
        
        }
        return 0;
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        return -1;
    } 
   
}
int Audio::InitFifo(){
    try{
        if (!(_fifo = av_audio_fifo_alloc(_ofmt_ctx->streams[_audio_index]->codec->sample_fmt,
                                        _ofmt_ctx->streams[_audio_index]->codec->channels, 1))) {
            throw std::runtime_error("Initial Fifo failed");
        }
        return 0;
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        return -1;
    } 
}
void Audio::_init_converted_samples(uint8_t ***converted_input_samples,
                                  int frame_size)
{
    int error;

    /* Allocate as many pointers as there are audio channels.
     * Each pointer will later point to the audio samples of the corresponding
     * channels (although it may be NULL for interleaved formats).
     */
    (*converted_input_samples) = new uint8_t*[_ofmt_ctx->streams[_audio_index]->codec->channels]();

    /* Allocate memory for the samples of all channels in one consecutive
     * block for convenience. */
    if ((error = av_samples_alloc(*converted_input_samples, NULL,
                                  _ofmt_ctx->streams[_audio_index]->codec->channels,
                                  frame_size,
                                  _ofmt_ctx->streams[_audio_index]->codec->sample_fmt, 0)) < 0) {
        throw std::runtime_error(" Could not allocate converted input samples");
    }
   
}
void  Audio::_convert_samples(const uint8_t **input_data,uint8_t **converted_data, const int frame_size)
{
    int error;

    /* Convert the samples using the resampler. */
    if ((error = swr_convert(_resample_context,
                             converted_data, frame_size,
                             input_data    , frame_size)) < 0) {
        throw std::runtime_error("Could not convert input samples");
    }
  
}
void Audio::_add_samples_to_fifo(uint8_t **converted_input_samples,const int frame_size)
{
    int error;

    /* Make the FIFO as large as it needs to be to hold both,
     * the old and the new samples. */
    if ((error = av_audio_fifo_realloc(_fifo, av_audio_fifo_size(_fifo) + frame_size)) < 0) {
        throw std::runtime_error("Could not reallocate FIFO");
    }

    /* Store the new samples in the FIFO buffer. */
    if (av_audio_fifo_write(_fifo, (void **)converted_input_samples,
                            frame_size) < frame_size) {
        throw std::runtime_error("Could not write data to FIFO");
    }

}
int Audio::AudioDecode(AVPacket* input_packet)
{
    /* Packet used for temporary storage. */
    try{
        _pts = input_packet->pts;
        _dts = input_packet->dts;
        converted_input_samples = NULL;     
        int error;
        _decode_frame = NULL;
        _decode_frame = av_frame_alloc(); 
        if ((error = avcodec_decode_audio4(_input_stream->codec,_decode_frame,
                                        &_data_present, input_packet)) < 0) {
            throw std::runtime_error("Audio decoding failed");
        }   
        if (_data_present) {
        /* Initialize the temporary storage for the converted input samples. */
            _init_converted_samples(&converted_input_samples,_decode_frame->nb_samples);
            

            /* Convert the input samples to the desired output sample format.
            * This requires a temporary storage provided by converted_input_samples. */
            _convert_samples((const uint8_t**)_decode_frame->extended_data, converted_input_samples,_decode_frame->nb_samples);
             

            /* Add the converted input samples to the FIFO buffer for later processing. */
            _add_samples_to_fifo(converted_input_samples,_decode_frame->nb_samples);

        }
        av_frame_unref(_decode_frame);
        av_frame_free(&_decode_frame);
        if (converted_input_samples) {
            av_freep(&converted_input_samples[0]);
            delete[] converted_input_samples;
        }        
        return error;
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        if (converted_input_samples) {
            av_freep(&converted_input_samples[0]);
           delete [] converted_input_samples;
         }
        av_frame_free(&_decode_frame);
        return -1;
    } 
}
void Audio::_init_output_frame(AVFrame **frame,AVCodecContext *output_codec_context,int frame_size)
{
    try{ 
        int error;
        /** Create a new frame to store the audio samples. */
        if (!(*frame = av_frame_alloc())) {   
             throw std::runtime_error("Could not allocate output frame");          
        }
        /**
         * Set the frame's parameters, especially its size and format.
         * av_frame_get_buffer needs this to allocate memory for the
         * audio samples of the frame.
         * Default channel layouts based on the number of channels
         * are assumed for simplicity.
         */
        (*frame)->nb_samples     = frame_size;
        (*frame)->channel_layout = output_codec_context->channel_layout;
        (*frame)->format         = output_codec_context->sample_fmt;
        (*frame)->sample_rate    = output_codec_context->sample_rate;
        /**
         * Allocate the samples of the created frame. This call will make
         * sure that the audio frame can hold as many samples as specified.
         */
        if ((error = av_frame_get_buffer(*frame, 0)) < 0) {
            av_frame_free(frame);
            throw std::runtime_error("Could allocate output frame samples");
        }
    }catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
    
}
int Audio::Load_encode_and_write(int64_t _now_pts)
{
    /* Temporary storage of the output samples of the frame written to the file. */
    try{    
        AVFrame *output_frame;
        /* Use the maximum number of possible samples per frame.
        * If there is less than the maximum possible frame size in the FIFO
        * buffer use this number. Otherwise, use the maximum possible frame size. */
        const int frame_size = FFMIN(av_audio_fifo_size(_fifo),_ofmt_ctx->streams[_audio_index]->codec->frame_size);
        int data_written;
        /* Initialize temporary storage for one output frame. */
        _init_output_frame(&output_frame,_ofmt_ctx->streams[_audio_index]->codec,frame_size);
        /* Read as many samples from the FIFO buffer as required to fill the frame.
        * The samples are stored in the frame temporarily. */
        if (av_audio_fifo_read(_fifo, (void **)output_frame->data, frame_size) < frame_size) {
            throw std::runtime_error("Could not read data from FIFO");
        }
        /* Encode one frame worth of audio samples. */
        _encode_audio_frame(output_frame,&data_written,_now_pts);    
        av_frame_free(&output_frame);
        return 0;
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}
void  Audio::_encode_audio_frame(AVFrame *frame,int *data_written,int64_t now_pts)
{
    /* Packet used for temporary storage. */
    AVPacket output_packet;
    int error;
    av_init_packet(&output_packet);
    /* Set the packet data and size so that it is recognized as being empty. */
    output_packet.data = NULL;
    output_packet.size = 0;
    /* Set a timestamp based on the sample rate for the container. */
    /* Encode the audio frame and store it in the temporary packet.
     * The output audio stream encoder is used to do this. */
    if ((error = avcodec_encode_audio2(_ofmt_ctx->streams[_audio_index]->codec, &output_packet,
                                       frame, data_written)) < 0) {
        av_packet_unref(&output_packet);
        av_free_packet(&output_packet);
        throw std::runtime_error("Could not encode frame");
    }
    output_packet.stream_index = _audio_index;
    output_packet.duration = _packet_duration;
    output_packet.pts =now_pts;
    /* Write one audio frame from the temporary packet to the output file. */
    if (*data_written) {
        if ((error = av_write_frame(_ofmt_ctx, &output_packet)) < 0) {
            av_packet_unref(&output_packet);
            av_free_packet(&output_packet);
            throw std::runtime_error( "Could not write frame");
        }
        av_packet_unref(&output_packet);
        av_free_packet(&output_packet);
    }
}
void Audio::FlushEncoder(){
    try{
        int data_written =0 ;
        do{
            _encode_audio_frame(NULL,&data_written,AV_NOPTS_VALUE);
        }while(data_written);
    
    }catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}
int  Audio::Flow(AVPacket* packet){
    try{
         const int output_frame_size = _ofmt_ctx->streams[_audio_index]->codec->frame_size;
        long double real_time=(double)(packet->pts/packet->duration)*((double)_input_stream->codec->frame_size/(double)_input_stream->codec->sample_rate);
        int64_t  _now_pts(real_time*_ofmt_ctx->streams[_audio_index]->codec->sample_rate*_packet_duration/output_frame_size);       
        do{
            int decoded = 0;     
            if (av_audio_fifo_size(_fifo) < output_frame_size) {   
                int ret = AudioDecode(packet);
                decoded = FFMIN(ret,packet->size);
                if(ret < 0){
                    throw std::runtime_error( "Could not decode frame in 307");
                }
            }
            while (av_audio_fifo_size(_fifo) >= output_frame_size){
                int ret = Load_encode_and_write(_now_pts);
                if(ret < 0){
                    throw std::runtime_error( "Could not encode frame in 316");
                }
                _now_pts += _packet_duration;
            }
            packet->data += decoded;
            packet->size -= decoded;
        }while(packet->size > 0);
        return 0;
    }catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        return -1;
    }
} 
void Audio::CleanUp(){
    if (_fifo)
        av_audio_fifo_free(_fifo);
    swr_free(&_resample_context);
    
}