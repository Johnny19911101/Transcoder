#include "Transcoder.h"
#include "Stream.h"
#include "Video.h"
#include "Audio.h"
#include <unistd.h>
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

Transcoder::Transcoder() :_wrapper(std::make_shared<CtxWrapper>()),_state(new Status()),_turnoff(false){      
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

void Transcoder::InitalAvio(int buffer_size,void *callback_pointer,int(*read)(void *a, uint8_t *b, int c)
                    ,std::string output,std::vector<std::pair<int,int> >& Pids){
    try{
        _state->SetState(typeid(Status::SetConfig));
        std::lock_guard<std::mutex> temp_lock(_lock_process);	
        _wrapper->SetAvio(buffer_size,callback_pointer,read,output,Pids,_pidObject,_ofmt_list);
        _state->SetState(typeid(Status::SetConfigFinished));
    }catch(std::exception const& e){
        std::cout << "Exception: " << e.what() << "\n";
    } 
}

void Transcoder::SetConfig(std::string input,std::string output,std::vector<std::pair<int,int> >& Pids){
//If you want to use buffer to input , just pass nullptr filename
    try{
        _state->SetState(typeid(Status::SetConfig));
        std::lock_guard<std::mutex> temp_lock(_lock_process);	
        if(_wrapper->SetConfig(input,output,Pids,_pidObject,_ofmt_list)<0)
            throw std::runtime_error("Failed allocating intput file");
        _state->SetState(typeid(Status::SetConfigFinished));
    }catch(std::exception const& e){
        std::cout << "Exception: " << e.what() << "\n";
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
void Transcoder::Process(){
    AVFormatContext *ifmt_ctx=_wrapper->GetIfmt();
    AVPacket packet;
    int ret=0;
    _state->SetState(typeid(Status::Process));
    std::lock_guard<std::mutex> temp_lock(_lock_process);
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
        std::cout << it->first <<std::endl;
        it->second->InitalTool();
    }
    while (!_turnoff) {
        init_packet(&packet);
        // Use the encoder's desired frame size for processing. 
        if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
            break; 
        auto it = _pidObject.find(packet.stream_index);
        if(it !=_pidObject.end()){
            it->second->Flow(&packet);
        }
        av_packet_unref(&packet);
        av_free_packet(&packet);//Need init ->unref ->free for one time or memory leak
    }  
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
        it->second->FlushEncoder();
    }
    for(auto it = _ofmt_list.begin();it!=_ofmt_list.end();++it){
        av_write_trailer(*it);
    }
    
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
        it->second->CleanUp();
        it->second.reset();
    }
    _state->SetState(typeid(Status::Finished));
 }
void Transcoder::CleanUp(){
    std::lock_guard<std::mutex> temp_lock(_lock_process);	
    /*mutex lock*/
    _state->SetState(typeid(Status::Reset));
    _wrapper->Cleanup(&_ofmt_list);
    _ofmt_list.clear();
    _pidObject.clear();
    _state->SetState(typeid(Status::Idle));
    _cleanup_done.notify_all();
}
std::string Transcoder::ReturnStateName(){
    return _state->GetCurrentName();
}
int Transcoder::StopProcess(){
 try{   
    if(_state->GetCurrentId() != typeid(Status::Process) ){
        throw std::runtime_error("Not In PROCESS\n");     
    }
    _turnoff = true;
    std::unique_lock<std::mutex> mLock(_lock_cleanup);
    _cleanup_done.wait( mLock );
    std::cout << "Stop the process and clean up done" <<std::endl;
    return 0;
    }catch(std::exception const& e) {
        std::cout << "Exception: " << e.what() ;
        return -1;
    } 
}
