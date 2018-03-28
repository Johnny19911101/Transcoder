#ifndef __CTXWRAPPER_H__
#define __CTXWRAPPER_H__
#include"Stream.h"

namespace Noovo{

    class CtxWrapper 
    {
    private:
        AVFormatContext *_ifmt_ctx=nullptr;
        void _videoEncoder(AVCodecContext** enc_ctx,AVCodecContext*& dec_ctx,AVCodec** encoder);
        void _audioEncoder(AVCodecContext** enc_ctx,AVCodecContext*& dec_ctx,AVCodec** encoder);
        void _findDecoder(int streamid);
        void _ofmtheader(AVFormatContext*& ,const std::string& );
        void _ifmtclean();
        void _oneofmtclean(AVFormatContext**);
        int _ifmtInital(const std::string&,const std::vector<int>&);
        int _ofmtInital(const std::string&,int,int
                        ,std::unordered_map<int,std::shared_ptr<Stream> >&
                        , std::vector<AVFormatContext*>&);
    public :
        CtxWrapper();
        ~CtxWrapper();
        int SetConfig(const std::string& ,const std::string& 
                    ,const std::vector<std::pair<int,int>>& ,std::unordered_map<int,std::shared_ptr<Stream> >&
                    , std::vector<AVFormatContext*>& );    
        void Cleanup(std::vector<AVFormatContext*>* );
        AVFormatContext* GetIfmt();
    };
}
#endif