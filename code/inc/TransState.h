#ifndef __TRANSSTATE_H__
#define __TRANSSTATE_H__
#include <typeinfo>
#include <iostream>
#include <typeindex>
#include <string>
#include <mutex>
#include <unordered_map>
#include <memory>
namespace Noovo{
    class TransState {
    public:
        TransState() : _info(0), _base(typeid(*this)),_index(_base){
        };  
        virtual ~TransState() {};
        virtual void Initialize() {
            _info = &typeid(*this);
            _index = std::type_index(*_info);
            std::cout << _info->name() << std::endl;
        };
        virtual std::string ReturnName(){
            return std::string(_info->name());
        }
        virtual std::type_index & GetIndex(){
            return _index;
        };
    protected:
        const std::type_info& _base;
        const std::type_info* _info;
        std::type_index _index;
    };
    class TranscodeStatus {
    public:
        class Initial : public TransState{};
        class SetConfig : public TransState{};
        class SetConfigFinished : public TransState{};
        class Process : public TransState{};
        class Finished : public TransState{};
        class Reset : public TransState{};
        class Idle : public TransState{};
		TranscodeStatus();
		 ~TranscodeStatus();
		void Initialize();
		void SetState(const std::type_index &state_index);
        std::string GetCurrentName();
		TransState & GetTransState(); 
        std::type_index GetCurrentId();
	protected:
		typedef std::unordered_map<std::type_index, std::unique_ptr<TransState>> Modes;
		Modes _modes;
        std::mutex _lock_state;
		TransState *_current;
        template<class T1>	inline void _init_state()
		{
			auto it = _modes.find(typeid(T1));
			if (it == _modes.end()) {
				auto& it_ptr =*(_modes.insert(std::make_pair(std::type_index(typeid(T1)),std::unique_ptr<T1> (new T1()))).first);          
                it_ptr.second->Initialize();
            }
		}
    };
}
#endif