//
// Created by rostard on 24.07.19.
//

#ifndef SMARTPOINTERS_SHAREDBASE_H
#define SMARTPOINTERS_SHAREDBASE_H

#include "Block.h"

template <typename T>
class SharedBase {
public:
    SharedBase(T* i_data, Block* i_block) : mp_data(i_data), mp_control_block(i_block) {}
protected:
    T* mp_data;
    Block* mp_control_block;

};


#endif //SMARTPOINTERS_SHAREDBASE_H
