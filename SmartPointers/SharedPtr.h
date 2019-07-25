//
// Created by rostard on 24.07.19.
//

#ifndef SMARTPOINTERS_SHAREDPTR_H
#define SMARTPOINTERS_SHAREDPTR_H

#include "SharedBase.h"

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr : public SharedBase<T>{
public:

    SharedPtr() : SharedBase<T>(nullptr, nullptr) {}
    SharedPtr(T* i_data) : SharedBase<T>(i_data, new FragmentedBlock<T>(i_data)) {  }
    SharedPtr(const SharedPtr<T>& rhs) : SharedBase<T>(rhs)
    {
        ++this->mp_control_block->m_shared_counter;
    }

    SharedPtr(const WeakPtr<T>& rhs) : SharedBase<T>(rhs)
    {
        ++this->mp_control_block->m_shared_counter;
    }


    SharedPtr& operator=(const WeakPtr<T>& rhs)
    {
        if(this->mp_control_block)
            this->mp_control_block->DecreeseSharedCounterAndMaybeFree();
        this->mp_control_block = rhs.mp_control_block;
        this->mp_data = rhs.mp_data;
        ++this->mp_control_block->m_shared_counter;
        return *this;
    }

    SharedPtr& operator=(WeakPtr<T>&& rhs)
    {
        if(this->mp_control_block)
            this->mp_control_block->DecreeseSharedCounterAndMaybeFree();
        this->mp_control_block = rhs.mp_control_block;
        this->mp_data = rhs.mp_data;
        ++this->mp_control_block->m_shared_counter;
        return *this;
    }


    virtual ~SharedPtr();

    SharedPtr(SharedPtr&& rhs) : SharedBase<T>(rhs.mp_data, rhs.mp_control_block)
    {
        rhs.mp_control_block = nullptr;
        rhs.mp_control_block = nullptr;
    }


    T& get()
    {
        return *this->mp_data;
    }

    const T& get() const
    {
        return *this->mp_data;
    }

    T&operator*()
    {
        return *this->mp_data;
    }

    const T& operator*() const
    {
        return *this->mp_data;
    }

    explicit operator bool()
    {
        return &get() != nullptr;
    }

private:
    template <typename U, typename ... Args>
    friend SharedPtr<U> make_shared(Args&&... args);

    SharedPtr(ContiniousBlock<T>* rhs) : SharedBase<T>(&rhs->m_data, rhs){  }
};

template <typename U, typename ... Args>
SharedPtr<U> make_shared(Args&&... args)
{
    auto block = new ContiniousBlock<U>(std::forward<Args>(args)...);
    return SharedPtr<U>(block);
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    if(this->mp_control_block)
        this->mp_control_block->DecreeseSharedCounterAndMaybeFree();
}

#endif //SMARTPOINTERS_SHAREDPTR_H
