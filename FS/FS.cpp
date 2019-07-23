//
// Created by rostard on 21.07.19.
//

#include "FS.h"

void *FS::GetFreeBlock() {
    for(int i = 0; i < m_size / BLOCK_SIZE; ++i)
    {
        if(!(mp_data[i/8] & (1 << i))){
            mp_data[i/8] |= (1 << i % 8);
            return mp_data.get() + i * BLOCK_SIZE;
        }
    }
    return nullptr;
}


FS::Inode* FS::GetAddressByPath(const FS::Path &i_full_path) {
    if(i_full_path[0] != '/')
        return nullptr;
    Inode* cur_inode = GetRoot();
    unsigned long prev_slash = 1;
    unsigned long next_slash = i_full_path.find_first_of('\\', prev_slash);
    while(next_slash != -1)
    {
        auto folder_name = i_full_path.substr(prev_slash, next_slash);
    }
    return cur_inode;
}

FS::Inode *FS::GetRoot() {
    const size_t bitset_size = m_size / BLOCK_SIZE / 8;
    auto blocks_for_bitset = static_cast<const size_t>(ceil((double) bitset_size / BLOCK_SIZE));
    return (Inode*)(mp_data.get() + blocks_for_bitset * BLOCK_SIZE);

}


FS::FolderHandle FS::OpenFolder(const FS::Path &i_full_path) {
    return FS::FolderHandle{GetAddressByPath(i_full_path)};
}

std::vector<FS::ItemHandle *> FS::FolderHandle::GetSubItems() {
    std::vector<FS::ItemHandle *> ret(*((size_t*)mp_some_handle->m_data_block[0]));
    for(int i = 0; i < ret.size(); ++i){
        Inode* inode = (Inode*)((size_t*)mp_some_handle->m_data_block[0] +1) + i;
        if(inode->m_attr.m_isFile)
            ret[i] = new FileHandle(inode);
    }
    return ret;
}
