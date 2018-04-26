#include "TransState.h"

using namespace Noovo;
TranscodeStatus::TranscodeStatus() : _modes(), _current(0) 
{
}
TranscodeStatus::~TranscodeStatus()
{
}

void TranscodeStatus::Initialize()
{	std::lock_guard<std::mutex> temp_lock(_lock_state);
	/*lock*/
	_init_state<TranscodeStatus::Initial>();//"Initialize");
	_init_state<TranscodeStatus::SetConfig>();//"SetConfig");
	_init_state<TranscodeStatus::SetConfigFinished>();//SetConfigFinished
	_init_state<TranscodeStatus::Process>();//"Process");
	_init_state<TranscodeStatus::Finished>();//"Finished");
	_init_state<TranscodeStatus::Reset>();//"Reset");
	_init_state<TranscodeStatus::Idle>();//"Idle");
	auto it = _modes.find(typeid(TranscodeStatus::Idle));
	_current = it->second.get();
}

void TranscodeStatus::SetState(const std::type_index & state_index)
{	
	std::lock_guard<std::mutex> temp_lock(_lock_state);	
	/*lock*/
	auto it = _modes.find(state_index);
	if (it != _modes.end()) {
		_current = it->second.get();
	}
}

TransState & TranscodeStatus::GetTransState()
{	
	std::lock_guard<std::mutex> temp_lock(_lock_state);	
	/*lock*/
	return *_current;
}
std::type_index TranscodeStatus::GetCurrentId(){
	return typeid(*_current);
}
std::string TranscodeStatus::GetCurrentName(){
	return _current->ReturnName();
}