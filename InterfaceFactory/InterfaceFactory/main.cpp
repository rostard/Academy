
#include <iostream>
#include <type_traits>

#include "StaticInterfaceFactory.h"
#include "DynamicInterfaceFactory.h"
struct Base
{
	virtual void say() {
		std::cout << "BASE" << '\n';
	}
};

struct Derived : public Base
{
	virtual void say() {
		std::cout << "Derived" << '\n';
	}
};

struct Derived2 : public Base
{
	virtual void say() {
		std::cout << "Derived2" << '\n';
	}
};



int main()
{
	std::cout << StaticInterfaceFactory::Instance().IsRegistered<Base>() << std::endl;
	StaticInterfaceFactory::Instance().Register<Base>([] {return new Derived2; });
	StaticInterfaceFactory::Instance().Create<Base>()->say();
	StaticInterfaceFactory::Instance().Register<Base, Derived>();
	StaticInterfaceFactory::Instance().Create<Base>()->say();
	std::cout << StaticInterfaceFactory::Instance().IsRegistered<Base>() << std::endl;
	StaticInterfaceFactory::Instance().Unregister<Base>();
	std::cout << StaticInterfaceFactory::Instance().IsRegistered<Base>() << std::endl;

	std::cout << std::endl;

	std::cout << DynamicInterfaceFactory::Instance().IsRegistered<Base>() << std::endl;
	DynamicInterfaceFactory::Instance().Register<Base>([] {return new Derived2; });
	DynamicInterfaceFactory::Instance().Create<Base>()->say();
	DynamicInterfaceFactory::Instance().Register<Base, Derived>();
	DynamicInterfaceFactory::Instance().Create<Base>()->say();
	std::cout << DynamicInterfaceFactory::Instance().IsRegistered<Base>() << std::endl;
	DynamicInterfaceFactory::Instance().Unregister<Base>();
	std::cout << DynamicInterfaceFactory::Instance().IsRegistered<Base>() << std::endl;

	std::cin.get();

	return 0;
}