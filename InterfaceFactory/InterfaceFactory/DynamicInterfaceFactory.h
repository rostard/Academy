#pragma once
#include <memory>
#include <functional>
#include <map>
#include <typeindex>

class DynamicInterfaceFactory
{
public:

	template<class TInterface>
	inline bool IsRegistered() const
	{
		return m_map.find(std::type_index(typeid(TInterface))) != m_map.end();
	}

	template<class TInterface, class TImplementation>
	inline void Register()
	{
		m_map[std::type_index(typeid(TInterface))] = [] {return new TImplementation(); };
	}

	template<class TInterface>
	inline void Register(std::function<TInterface*()> i_create_proc)
	{
		m_map[std::type_index(typeid(TInterface))] = i_create_proc;
	}

	template<class TInterface>
	inline void Unregister()
	{
		m_map.erase(std::type_index(typeid(TInterface)));
	}

	template<class TInterface>
	inline std::unique_ptr<TInterface> Create() const
	{
		std::unique_ptr<TInterface> ret ((TInterface*)(m_map.at(std::type_index(typeid(TInterface)))()));
		return ret;
	}

	template<class TInterface>
	inline TInterface* CreateRaw() const
	{
	}

	static DynamicInterfaceFactory& Instance() {
		static DynamicInterfaceFactory IF;
		return IF;
	}

private:
	std::map<std::type_index, std::function<void*()>> m_map;
};
