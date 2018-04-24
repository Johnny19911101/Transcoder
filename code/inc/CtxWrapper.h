#ifndef __CTXWRAPPER_H__
#define __CTXWRAPPER_H__
#include"Stream.h"

namespace Noovo{

    class CtxWrapper 
    {
    public:
        CtxWrapper();
        ~CtxWrapper();
        void Switch(int ,void *,int(*)(void *, uint8_t *, int )
                        ,std::pair<int,int> ,std::shared_ptr<Ofmt_list> ,std::unordered_map<int,std::shared_ptr<Stream>>&);
        void SetAvio(int ,void *,int(*)(void *, uint8_t *, int )
                    ,const std::string& ,const std::vector<std::pair<int,int>>& 
                    ,std::unordered_map<int,std::shared_ptr<Stream>>& 
                    , std::vector<std::shared_ptr<Ofmt_list>>& );
        int SetConfig(const std::string& ,const std::string& 
                    ,const std::vector<std::pair<int,int>>&
                     ,std::unordered_map<int,std::shared_ptr<Stream> >&
                    , std::vector<std::shared_ptr<Ofmt_list>>& );    
        void Cleanup(std::vector<std::shared_ptr<Ofmt_list>>& );
        AVFormatContext* GetIfmt();    
    private:
        AVIOContext *_avio_ctx = nullptr;
        AVFormatContext *_ifmt_ctx=nullptr;
        void _videoEncoder(AVCodecContext** enc_ctx,AVCodecContext*& dec_ctx,AVCodec** encoder);
        void _audioEncoder(AVCodecContext** enc_ctx,AVCodecContext*& dec_ctx,AVCodec** encoder);
        void _findDecoder(int streamid);
        void _ofmtheader(AVFormatContext*& ,const std::string& );
        void _ifmtclean();
        void _oneofmtclean(AVFormatContext*);
        void _option(AVFormatContext*);
        int _ifmtInital(const char*,const std::vector<int>&);
        int _ofmtInital(const std::string&,int,int
                        ,std::unordered_map<int,std::shared_ptr<Stream>>&
                        , std::vector<std::shared_ptr<Ofmt_list>>&);
    };
}
#endif