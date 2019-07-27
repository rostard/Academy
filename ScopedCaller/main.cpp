#include <iostream>
#include <functional>
#include <bits/unique_ptr.h>

struct ScopedCaller{

    template <typename F>
    ScopedCaller(F&& i_fun) : mp_f(std::make_unique<std::function<void()>>(
            [fun = std::forward<F>(i_fun)] () mutable {fun();})) {}
    ~ScopedCaller() {
        if(mp_f)(*mp_f)();
    }

    std::unique_ptr<std::function<void()>> Release(){
        return std::move(mp_f);
    }


    template <typename F>
    void Reset(F&& i_fun){
        auto new_f = std::make_unique<std::function<void()>>(
                [fun = std::forward<F>(i_fun)] () mutable {fun();});
        mp_f = std::move(new_f);
    }

private:
    std::unique_ptr<std::function<void()>> mp_f;
};



struct Functor
{
    Functor(){
        std::cout<<"Constructor" << '\n';
    }

    Functor(std::string i_msg) : msg(std::move(i_msg)){
        std::cout<<"Constructor " << msg <<'\n';
    }

    ~Functor(){
        std::cout<<"Destructor" << msg <<'\n';
    }

    Functor(const Functor& rhs){
        msg = rhs.msg;
        std::cout<<"Constructor copy " << msg <<'\n';
    }

    Functor(Functor&& rhs){
        msg = std::move(rhs.msg);
        std::cout<<"Constructor move " << msg <<'\n';
    }


    Functor& operator=(const Functor& rhs){
        msg = rhs.msg;
        std::cout<<"copy " << msg <<'\n';
        return *this;
    }


    Functor& operator=(Functor&& rhs){
        msg = std::move(rhs.msg);
        std::cout<<"move " << msg <<'\n';
        return *this;
    }

    int operator()(){
        std::cout<<"Goodbye " <<msg <<'\n';
    }

    std::string msg;
};

int main() {
    {
        ScopedCaller sc(Functor{"First"});
        auto f = sc.Release();
        sc.Reset(Functor{"Second"});
        //(*f)();
    }
    std::function<void()> function([]{});
    return 0;
}