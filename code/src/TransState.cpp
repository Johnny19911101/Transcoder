#include "TransState.h"
using namespace Noovo;
Status::Status() : _modes(), _current(0) 
{
}
Status::~Status()
{
}

void Noovo::Status::Initialize()
{	std::lock_guard<std::mutex> temp_lock(_lock_state);
	/*lock*/
	_init_state<Status::Initial>();//"Initialize");
	_init_state<Status::SetConfig>();//"SetConfig");
	_init_state<Status::SetConfigFinished>();//SetConfigFinished
	_init_state<Status::Process>();//"Process");
	_init_state<Status::Finished>();//"Finished");
	_init_state<Status::Reset>();//"Reset");
	_init_state<Status::Idle>();//"Idle");
	auto it = _modes.find(typeid(Status::Idle));
	_current = it->second.get();
}

void Noovo::Status::SetState(const std::type_index & state_index)
{	
	std::lock_guard<std::mutex> temp_lock(_lock_state);	
	/*lock*/
	auto it = _modes.find(state_index);
	if (it != _modes.end()) {
		_current = it->second.get();
	}
}

TransState & Noovo::Status::GetTransState()
{	
	std::lock_guard<std::mutex> temp_lock(_lock_state);	
	/*lock*/
	return *_current;
}
std::string Status::GetCurrentName(){
	return _current->ReturnName();
}