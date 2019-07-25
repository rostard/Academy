#include <iostream>
#include <string>

#include "SharedPtr.h"
#include "WeakPtr.h"

class Wrapper{
public:
    Wrapper(const std::string &s) : s(s) {
        std::cout<<"Construct "<<s<<'\n';
    }

    virtual ~Wrapper() {
        std::cout<<"Destruct "<<s<<'\n';
    }

    friend std::ostream &operator<<(std::ostream &os, const Wrapper &wrapper) {
        os << "s: " << wrapper.s;
        return os;
    }

private:
    std::string s;
};

template <typename T>
void check_weak( const WeakPtr<T>& weakPtr){
    if(auto p = weakPtr.lock() ){
        std::cout<< *p << '\n';
    }
    else{
        std::cout<< "data is destroyed\n";
    }
}

int main() {
    WeakPtr<Wrapper> weakPtr;
    {
        SharedPtr<Wrapper> sharedPtr(new Wrapper("Hello"));
        auto sharedPtr2 = sharedPtr;
        auto shared = make_shared<Wrapper>("This was made with make_shared");

        std::cout << *shared.get() << std::endl;
        std::cout << *sharedPtr2.get() << std::endl;

        weakPtr = sharedPtr2;
        SharedPtr<Wrapper> sharedPtr3(weakPtr);
        std::cout << *weakPtr.lock().get() << std::endl;

        check_weak(weakPtr);
    }

	SharedPtr<int> sp;
	SharedPtr<int> sp2(sp);

    check_weak(weakPtr);
    return 0;
}