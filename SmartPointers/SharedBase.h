//
// Created by rostard on 24.07.19.
//

#pragma once

#include "Block.h"

template <typename T>
class SharedBase {
public:
    SharedBase(T* i_data, Block* i_block) : mp_data(i_data), mp_control_block(i_block) {}
protected:
    T* mp_data;
    Block* mp_control_block;

};
