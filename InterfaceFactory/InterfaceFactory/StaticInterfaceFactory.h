#pragma once
#include <memory>
#include <functional>
class StaticInterfaceFactory
{
	template<typename TInterface>
	struct Wrapper
	{
		virtual TInterface* create() = 0;
	};

	template<typename TInterface, typename TImplementation>
	struct ConcreteWrapper : public Wrapper<TInterface>
	{
		virtual TInterface* create() override
		{
			return new TImplementation{};
		}
	};

	template<typename TInterface>
	struct FuncWrapper : public Wrapper<TInterface>
	{
		FuncWrapper(std::function<TInterface*()> i_func) : m_init_func(i_func) {}

		virtual TInterface* create() override
		{
			return  m_init_func();
		}

		std::function<TInterface*()> m_init_func;
	};
public:

	template<typename TInterface>
	static Wrapper<TInterface>*& getInstance()
	{
		static Wrapper<TInterface>* instance = nullptr;
		return instance;
	}


	template<class TInterface>
	inline bool IsRegistered() const
	{
		return (getInstance<TInterface>());
	}

	template<class TInterface, class TImplementation>
	inline void Register()
	{
		if (getInstance<TInterface>())
			delete getInstance<TInterface>();
		getInstance<TInterface>() = new ConcreteWrapper<TInterface, TImplementation>;
	}

	template<class TInterface>
	inline void Register(std::function<TInterface*()> i_create_proc)
	{
		if (getInstance<TInterface>())
			delete getInstance<TInterface>();
		getInstance<TInterface>() = new FuncWrapper<TInterface>(i_create_proc);
	}

	template<class TInterface>
	inline void Unregister()
	{
		getInstance<TInterface>() = nullptr;
	}

	template<class TInterface>
	inline std::unique_ptr<TInterface> Create() const
	{
		std::unique_ptr<TInterface> ret(getInstance<TInterface>()->create());
		return ret;
	}

	template<class TInterface>
	inline TInterface* CreateRaw() const
	{
		getInstance<TInterface>()->create();
	}

	static StaticInterfaceFactory& Instance() {
		static StaticInterfaceFactory IF;
		return IF;
	}
};
