
#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libavformat/avio.h"
#include <libavfilter/buffersink.h>
#include "libavutil/audio_fifo.h"
#include "libavutil/avassert.h"
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include "libswresample/swresample.h"
#include "libavutil/avstring.h"
#include "libavutil/avutil.h"
#include "libavutil/frame.h"
#include "libavutil/opt.h"

#define OUTPUT_BIT_RATE 48000
/** The number of output channels */
#define OUTPUT_CHANNELS 2
/** The audio sample output format */
#define OUTPUT_SAMPLE_FORMAT AV_SAMPLE_FMT_S16

static int voice_stream=-1;
static int64_t test=0;
static int64_t test2=0;
static AVFormatContext *ifmt_ctx;
static AVFormatContext *ofmt_ctx;
typedef struct FilteringContext {
    AVFilterContext *buffersink_ctx;
    AVFilterContext *buffersrc_ctx;
    AVFilterGraph *filter_graph;
} FilteringContext;
static FilteringContext *filter_ctx;


static int open_input_file(const char *filename)
{
    int ret;
    unsigned int i;

    ifmt_ctx = NULL;
    if ((ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }



        i=0;
        AVStream *stream = ifmt_ctx->streams[i];
        AVCodecContext *codec_ctx;
        AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
        printf("%s\n",dec->name);
        if (!dec) {
            av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", i);
            return AVERROR_DECODER_NOT_FOUND;
        }
        
        codec_ctx= avcodec_alloc_context3(dec);
        if (!codec_ctx) {
            av_log(NULL, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", i);
            return AVERROR(ENOMEM);
        }
     
       
        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
                || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
                codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, NULL);
         
            if(codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
                voice_stream = i;
           // ret = avcodec_open2(codec_ctx, dec, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
                return ret;
            }
        }
        ifmt_ctx->streams[i]->codec =codec_ctx;
        

      // avcodec_close(codec_ctx);
      //  avcodec_free_context(&codec_ctx);
     
      avcodec_free_context(&( ifmt_ctx->streams[i]->codec));
    
      avformat_close_input(&ifmt_ctx);
  //  av_dump_format(ifmt_ctx, 0, filename, 0);
    return 0;
}
/*
static int open_output_file(const char *filename)
{
    AVStream *out_stream;
    AVStream *in_stream;
    AVCodecContext *dec_ctx, *enc_ctx;
    AVCodec *encoder;
    int ret;
    unsigned int i;

    ofmt_ctx = NULL;
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename);
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return AVERROR_UNKNOWN;
    }


    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        out_stream = avformat_new_stream(ofmt_ctx, NULL);
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
            return AVERROR_UNKNOWN;
        }
        printf("here is %d \n",__LINE__);
        in_stream = ifmt_ctx->streams[i];
        dec_ctx = ifmt_ctx->streams[i]->codec;

        if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
                || dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            
            if(dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
                encoder = avcodec_find_encoder(dec_ctx->codec_id);
            else{
               encoder = avcodec_find_encoder(AV_CODEC_ID_AAC);
               printf("%s!\n",encoder->name);
            }
            if (!encoder) {
                av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
                return AVERROR_INVALIDDATA;
            }
            enc_ctx = avcodec_alloc_context3(encoder);
            if (!enc_ctx) {
                av_log(NULL, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
                return AVERROR(ENOMEM);
            }


            if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
                enc_ctx->height = dec_ctx->height;
                enc_ctx->width = dec_ctx->width;
                enc_ctx->sample_aspect_ratio = dec_ctx->sample_aspect_ratio;
              
                if (encoder->pix_fmts)
                    enc_ctx->pix_fmt = encoder->pix_fmts[0];
                else
                    enc_ctx->pix_fmt
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
            }
            printf("here is %d \n",__LINE__);
         
            ret = avcodec_open2(enc_ctx, encoder, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", i);
                return ret;
            }
            ret = avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to copy encoder parameters to output stream #%u\n", i);
                return ret;
            }
            if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
                enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

            out_stream->time_base = enc_ctx->time_base;
            ofmt_ctx->streams[i]->codec = enc_ctx;
        } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            av_log(NULL, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n", i);
            return AVERROR_INVA
            ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Copying parameters for stream #%u failed\n", i);
                return ret;
            }
            out_stream->time_base = in_stream->time_base;
        }
    printf("here is %d \n",__LINE__);
    }
    av_dump_format(ofmt_ctx, 0, filename, 1);
    printf("here is %d \n",__LINE__);
    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", filename);
            return ret;
        }
    }
}
*/
int main(int argc, char **argv)
{   int i,ret;
    av_register_all();
    while(1){
       open_input_file("/home/johnny/abc.ts");
     }  // open_output_file("test.ts");
      
}