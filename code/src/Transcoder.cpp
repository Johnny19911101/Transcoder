#include "Transcoder.h"

#include "Stream.h"
#include "Video.h"
#include "Audio.h"
#include "Adinsert.h"
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

Transcoder::Transcoder() :_wrapper(std::make_shared<CtxWrapper>()),_state(new TranscodeStatus()),_turnoff(false){      
        av_register_all();
	    avfilter_register_all();
        _state->Initialize();//Status inital
        _state->SetState(typeid(TranscodeStatus::Initial));
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
        _state->SetState(typeid(TranscodeStatus::SetConfig));
        std::lock_guard<std::mutex> temp_lock(_lock_process);	
        _wrapper->SetAvio(buffer_size,callback_pointer,read,output,Pids,_pidObject,_fmtwrapper_list);
        _state->SetState(typeid(TranscodeStatus::SetConfigFinished));
    }catch(std::exception const& e){
        std::cout << "Exception: " << e.what() << "\n";
    } 
}
std::thread Transcoder::Thread_process(){
    return std::thread(&Transcoder::Process,this);
}
void Transcoder::SetConfig(std::string input,std::string output,std::vector<std::pair<int,int> >& Pids){
//If you want to use buffer to input , just pass nullptr filename
    try{
        _state->SetState(typeid(TranscodeStatus::SetConfig));
        std::lock_guard<std::mutex> temp_lock(_lock_process);	
        if(_wrapper->SetConfig(input,output,Pids,_pidObject,_fmtwrapper_list)<0)
            throw std::runtime_error("Failed allocating intput file");
        _state->SetState(typeid(TranscodeStatus::SetConfigFinished));
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
    _state->SetState(typeid(TranscodeStatus::SetConfigFinished));
}
void Transcoder::Process(){
    AVFormatContext *ifmt_ctx=_wrapper->GetIfmt();
    AVPacket packet;
    int ret=0;
    _state->SetState(typeid(TranscodeStatus::Process));
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
    for(auto it = _fmtwrapper_list.begin();it!=_fmtwrapper_list.end();++it){
        (*it)->GetConcatpoint();
    }
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
        it->second->CleanUp();
        it->second.reset();
    }
  
    _state->SetState(typeid(TranscodeStatus::Finished));
 }
void Transcoder::WriteEnd(){

    for(auto it = _fmtwrapper_list.begin();it!=_fmtwrapper_list.end();++it){
        av_write_trailer((*it)->ReturnAvformat());
    }
}
void Transcoder::CleanUp(){
    std::lock_guard<std::mutex> temp_lock(_lock_process);	
    /*mutex lock*/
    _state->SetState(typeid(TranscodeStatus::Reset));
    for(auto it = _pidObject.begin();it!=_pidObject.end();++it){
       it->second->FlushEncoder();
    }
    _wrapper->Cleanup(_fmtwrapper_list);
    _fmtwrapper_list.clear();
    _pidObject.clear();
    _state->SetState(typeid(TranscodeStatus::Idle));
    _cleanup_done.notify_all();
}
std::string Transcoder::ReturnStateName(){
    return _state->GetCurrentName();
}
std::type_index Transcoder::ReturnState(){
    return _state->GetCurrentId();
}
int Transcoder::StopProcess(){
 try{   
    if(_state->GetCurrentId() != typeid(TranscodeStatus::Process) ){
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

void Transcoder::SwitchChannel(int buffer_size,void *callback_pointer,int(*read)(void *a, uint8_t *b, int c)
                    ,int ofmt_index,std::pair<int,int> pairs){
    try{   
        std::shared_ptr<Adinsert> adinsert=std::make_shared<Adinsert>("ad001.ts");
        _turnoff = true;
        std::lock_guard<std::mutex> temp_lock(_lock_process);     
        std::thread adinsert_thread = adinsert->Process_thread(_fmtwrapper_list[ofmt_index]);
        _pidObject.clear();
        _wrapper->Switch(188*10*5,callback_pointer,read,pairs,_fmtwrapper_list[ofmt_index],_pidObject); 
        adinsert_thread.join();
        adinsert.reset();
        _turnoff = false;
        return 0;
    }catch(std::exception const& e) {
        std::cout << "Exception: " << e.what() ;
        return -1;
    } 
}
