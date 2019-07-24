//
// Created by rostard on 24.07.19.
//

#ifndef SMARTPOINTERS_WEAKPTR_H
#define SMARTPOINTERS_WEAKPTR_H


#include "SharedPtr.h"
template <typename T>
class WeakPtr : public SharedBase<T>{
public:
    WeakPtr() : SharedBase<T>(nullptr, nullptr) {}
    WeakPtr(const SharedBase<T>& rhs) : SharedBase<T>(rhs)
    {
        ++this->mp_control_block->m_weak_counter;
    }

    WeakPtr& operator=(const WeakPtr<T>& rhs)
    {
        if(this->mp_control_block)
            this->mp_control_block->DecreeseWeakCounterAndMaybeDelete();
        this->mp_control_block = rhs.mp_control_block;
        this->mp_data = rhs.mp_data;
        ++this->mp_control_block->m_weak_counter;
        return *this;
    }

    WeakPtr& operator=(WeakPtr<T>&& rhs)
    {
        if(this->mp_control_block)
            this->mp_control_block->DecreeseWeakCounterAndMaybeDelete();
        this->mp_control_block = rhs.mp_control_block;
        this->mp_data = rhs.mp_data;
        ++this->mp_control_block->m_weak_counter;
        return *this;
    }

    ~WeakPtr() {
        this->mp_control_block->DecreeseWeakCounterAndMaybeDelete();
    }

    SharedPtr<T> lock()
    {
        return expired() ? SharedPtr<T>() : SharedPtr(*this);
    }

    const SharedPtr<T> lock() const
    {
        return expired() ? SharedPtr<T>() : SharedPtr(*this);
    }

    bool expired() const
    {
        return use_count() == 0;
    }

    uint16_t use_count() const
    {
        return this->mp_control_block->m_shared_counter;
    }
};


#endif //SMARTPOINTERS_WEAKPTR_H
