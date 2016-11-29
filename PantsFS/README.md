#Layout

---------
| Boot | Superblock | Root Directory | ...
----------

Boot sector is 512 bytes

Each block is 512 bytes

Superblock layout
struct Superblock {
    uint16_t magic_number;
    uint16_t version;
    uint32_t block_count;
    uint32_t free_block_count;
    uint32_t metafile_count;
    uint32_t free_metafile_count;
    const char name[64];
};

Free metafiles are stored in a doubly linked list.

enum FileType : uint32_t {
    Directory,
    RegularFile,
};



struct Metafile {
    uint32_t type;
    uint32_t size;
    uint32_t block_count;
    uint32_t blocks[29];
};
