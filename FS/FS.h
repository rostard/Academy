#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <cstring>
#include <cmath>

class FS
{
public:

    static const size_t BLOCK_SIZE = 4 * 1024;

    struct Attr{
        size_t m_size;
        std::chrono::steady_clock::time_point m_creation_date;
        std::string m_created_by;
        bool m_readOnly;
        bool m_isFile;
    };

    struct Inode{

        Attr m_attr;
        std::array<void*, 12> m_data_block;

    };
    using Path = std::string;

    // It can be std::any C++17(or boost::any), but in general, void* is also OK
    using Attribute = void*;

    enum class EAttributes
    {
        Size,
        CreationDate,
        CreatedBy,
        ReadOnly,
        IsFile

        /*... what you wish ...*/
    };

    struct FolderHandle;
    struct FileHandle;

    struct ItemHandle
    {
        Attribute GetAttribute(EAttributes i_attribute) const;
        // Some attributes might be modified, for instance, ReadOnly flag
        bool SetAttribute(EAttributes i_attr_type, Attribute i_value);

        Path GetFullPath() const;

        FolderHandle* GetFolder() const;
        FileHandle* GetFile() const;
    };

    struct FolderHandle : ItemHandle
    {
        // TODO: Use RAII here to Close Folder when this Handle is destroyed
        FolderHandle(Inode* i_folder_inode) : mp_some_handle(i_folder_inode){}

        std::vector<ItemHandle*> GetSubItems();

        // Find all files in subfolders with pattern(regex  p maybe)
        std::vector<ItemHandle*> Find(const Path& i_pattern);

    private:
        Inode* mp_some_handle;
    };

    struct FileHandle : ItemHandle
    {
        // TODO: Use RAII here to Close File when this Handle is destroyed
        FileHandle(Inode* i_file_inode) : mp_some_handle(i_file_inode){}

        size_t Length() const;

        // Let read and write only text files, so we can read to some string buffer
        // If empty string is returned - Read failed
        std::string Read(size_t i_file_offset, size_t i_number_of_bytes_to_read);
        // Write with offset. If offset < lenght - override buffer, otherwise - append.
        bool Write(size_t i_file_offset, const std::string& i_buffer);
        // Write with offset. If offset < lenght - insert some text in the middle.
        bool Append(size_t i_file_offset, const std::string& i_buffer);

        // !!! Hint: Do not write data directly to the disk every time when Write/Append is performed
        // Store the data in some Buffer
        // Write only when FileHandle is closed(is destroyed);
        // So, if we will use 1000 Writes, it will update data on the disk only once, when file is closed!!!
    private:
        Inode* mp_some_handle;
    };

    FS(size_t i_size_in_bytes)
            : mp_data(std::make_unique<unsigned char[]>(i_size_in_bytes)), m_size(i_size_in_bytes)
    {
        const size_t bitset_size = i_size_in_bytes / BLOCK_SIZE / 8;
        new(mp_data.get()) char[bitset_size];
        memset(mp_data.get(), 0, bitset_size);

        auto blocks_for_bitset = static_cast<const size_t>(ceil((double) bitset_size / BLOCK_SIZE));
        for(int i = 0; i < blocks_for_bitset + 1; ++i)
        {
            mp_data[i/8] |= 1 << i%8;
        }

        auto root_inode = GetRoot();
        root_inode->m_data_block[0] = GetFreeBlock();
        root_inode->m_attr.m_size = sizeof(size_t);

        char* root_folder = (char*)root_inode->m_data_block[0];
        *(size_t*)root_folder = 2;
        auto* items = (ItemEntry*)((size_t*)root_folder + 1);


        items[0].file_inode = root_inode;
        strcpy(items[0].name, ".");

        items[1].file_inode = root_inode;
        strcpy(items[1].name, "..");
    }

    bool CreateEmptyFolder(const Path& i_full_path);
    bool DeleteFolder(const Path& i_full_path);

    bool CreateEmptyFile(const Path& i_full_path);
    bool DeleteFile(const Path& i_full_path);

    // Move or rename file or folder(recursive)
    bool Move(const Path& i_full_path_from, const Path& i_full_path_to);
    // Copy or renames file or folder(recursive)
    bool Copy(const Path& i_full_path_from, const Path& i_full_path_to);

    bool CreateLink(const Path& i_full_path, const Path& i_full_path_link);

    FolderHandle OpenFolder(const Path& i_full_path);
    FileHandle OpenFile(const Path& i_full_path);

private:
    struct ItemEntry{
        Inode* file_inode;
        char name[32];
    };

    void* GetFreeBlock();
    Inode* GetAddressByPath(const FS::Path &i_full_path);
    Inode* GetRoot();

    std::unique_ptr<unsigned char[]> mp_data;
    size_t m_size;
};
