#include <iostream>
#include <functional>

class ScopedCaller
{
public:
    template<typename F, typename = std::enable_if_t<!std::is_same<ScopedCaller&, F>::value>>
    ScopedCaller(F&& f)
            : m_func(std::forward<F>(f))
    {
    }

    ScopedCaller(const ScopedCaller& rhs) = delete;
    ScopedCaller(ScopedCaller&& rhs) = default;
    ScopedCaller& operator=(const ScopedCaller& rhs) = default;
    ScopedCaller& operator=(ScopedCaller&& rhs) = default;

    ~ScopedCaller()
    {
        if(m_func)m_func();
    }


    std::function<void()> Release()
    {
        return std::move(m_func);
    }

    template <typename F>
    void Reset(F&& f)
    {
        auto new_func(std::forward<F>(f));
        if(m_func)m_func();
        m_func = std::move(new_func);
    }

    void Reset()
    {
        m_func = std::function<void()>{};
    }

private:
    std::function<void()> m_func;
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
        std::cout<<"Destructor " << msg <<'\n';
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
//        ScopedCaller sc2 = sc;
//        auto f = sc.Release();
//        sc.Reset(Functor{"Second"});
//        sc.Reset();

    }
    std::function<void()> function([]{});
    return 0;
}
