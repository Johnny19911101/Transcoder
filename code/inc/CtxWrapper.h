#ifndef __CTXWRAPPER_H__
#define __CTXWRAPPER_H__
#include"Stream.h"

namespace Noovo{

    class CtxWrapper 
    {
    public:
        CtxWrapper();
        ~CtxWrapper();
        void SetAvio(int ,void *,int(*)(void *, uint8_t *, int )
                    ,const std::string& ,const std::vector<std::pair<int,int>>& 
                    ,std::unordered_map<int,std::shared_ptr<Stream>>& 
                    , std::vector<AVFormatContext*>& );
        int SetConfig(const std::string& ,const std::string& 
                    ,const std::vector<std::pair<int,int>>&
                     ,std::unordered_map<int,std::shared_ptr<Stream> >&
                    , std::vector<AVFormatContext*>& );    
        void Cleanup(std::vector<AVFormatContext*>* );
        AVFormatContext* GetIfmt();    
    private:
        AVIOContext *_avio_ctx = nullptr;
        AVFormatContext *_ifmt_ctx=nullptr;
        void _videoEncoder(AVCodecContext** enc_ctx,AVCodecContext*& dec_ctx,AVCodec** encoder);
        void _audioEncoder(AVCodecContext** enc_ctx,AVCodecContext*& dec_ctx,AVCodec** encoder);
        void _findDecoder(int streamid);
        void _ofmtheader(AVFormatContext*& ,const std::string& );
        void _ifmtclean();
        void _oneofmtclean(AVFormatContext**);
        int _ifmtInital(const char*,const std::vector<int>&);
        int _ofmtInital(const std::string&,int,int
                        ,std::unordered_map<int,std::shared_ptr<Stream>>&
                        , std::vector<AVFormatContext*>&);
    };
}
#endif