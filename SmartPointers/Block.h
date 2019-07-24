//
// Created by rostard on 24.07.19.
//

#ifndef SMARTPOINTERS_BLOCK_H
#define SMARTPOINTERS_BLOCK_H


#include <cstdint>
#include <utility>
#include <new>

class Block {
public:

    virtual void Free() = 0;



    void DecreeseSharedCounterAndMaybeFree()
    {
        if(--m_shared_counter == 0)
        {
            Free();
            if(m_weak_counter == 0) operator delete(this);
        }
    }

    void DecreeseWeakCounterAndMaybeDelete()
    {
        if(--m_weak_counter == 0 && m_shared_counter == 0)
            operator delete(this);

    }
    virtual ~Block() = default;
    uint16_t m_shared_counter = 1;
    uint16_t m_weak_counter = 0;
};


template<typename T>
class FragmentedBlock : public Block
{
public:
    FragmentedBlock(T* i_data) : mp_data(i_data) {}
    void Free() override
    {
        delete mp_data;
        mp_data = nullptr;
    }
    T* mp_data;
};

template<typename T>
class ContiniousBlock : public Block
{
public:

    template <typename ... Args>
    ContiniousBlock(Args&&...args) : m_data(std::forward<Args>(args)...) {}
    void Free() override
    {
        m_data.~T();
    }
    T m_data;
};

#endif //SMARTPOINTERS_BLOCK_H
