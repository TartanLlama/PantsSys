#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

namespace pants {
namespace fs {

constexpr uint16_t g_magic_number = 0x9A27;
constexpr uint16_t g_block_size = 512;
constexpr uint16_t g_metafiles_per_block_group = 1024;
constexpr uint16_t g_metafile_size = 32;
constexpr uint16_t g_blocks_per_metafile = 29;
constexpr uint16_t g_blocks_per_block_group =
    (g_metafile_size * g_metafiles_per_block_group) /
        g_block_size +
    g_blocks_per_metafile * g_metafiles_per_block_group;
constexpr uint16_t g_first_block_group_block_id = 2;
constexpr uint16_t g_invalid_block_id = 0;

struct Superblock {
    uint16_t magic_number;
    uint16_t version;
    uint32_t block_count;
    uint32_t free_block_count;
    uint32_t metafile_count;
    uint32_t free_metafile_count;
    char name[64];
};

enum class FileType : uint32_t {
    Directory,
    RegularFile,
};

struct Metafile {
    uint32_t type;
    uint32_t size;
    uint32_t block_count;
    uint32_t blocks[29];
};

struct UnusedMetafile {
    uint32_t prev;
    uint32_t next;
    uint32_t padding[30];
};

struct BlockGroup {
    Metafile metafile_table[g_metafiles_per_block_group];
    uint16_t first_unused_metafile;
    uint16_t unused_metafile_count;
    uint16_t first_unused_block;
    uint16_t unused_block_count;
};

struct Directory {
    uint32_t metafile;
    uint8_t name_size;
    char name[64];
};

struct Block {
    uint32_t id;
};

std::istream &operator>>(std::istream &is, Metafile &mf) {
    is >> mf.type >> mf.size >> mf.block_count;
    for (auto &&block : mf.blocks)
        is >> block;
}

std::istream &operator>>(std::istream &is, BlockGroup &bg) {
    for (auto &&mf : bg.metafile_table)
        is >> mf;
    is >> bg.first_unused_metafile >> bg.unused_metafile_count >>
        bg.first_unused_block >> bg.unused_block_count;
}

inline uint32_t ByteFromBlockID(uint32_t block_id) {
    return block_id * g_block_size;
}

inline uint32_t BlockGroupIDFromMetafileID(uint32_t metafile_id) {
    return metafile_id / g_metafiles_per_block_group;
}

inline uint32_t BlockIDFromBlockGroupID(uint32_t block_group_id) {
    return block_group_id * g_blocks_per_block_group +
           g_first_block_group_block_id;
}

uint32_t ByteFromMetafileID(uint32_t metafile_id) {
    uint32_t block_group_id = BlockGroupIDFromMetafileID(metafile_id);
    uint32_t block_group_block_id = BlockIDFromBlockGroupID(block_group_id);
}

uint32_t NextUnusedBlock(std::istream &is, uint32_t block_id,
                         bool can_allocate = false);

uint32_t PrevUnusedBlock(std::istream &is, uint32_t block_id);


inline void SetPrevUnusedBlock(std::ostream &os, uint32_t block_id, uint32_t prev) {
    os.seekp(ByteFromBlockID(block_id));
    os << prev;
}

inline void SetNextUnusedBlock(std::ostream &os, uint32_t block_id, uint32_t next) {
    os.seekp(ByteFromBlockID(block_id) + sizeof(uint32_t));
    os << next;
}

inline uint32_t CreateFile(std::iostream &fs_file, uint32_t dir_metafile_id,
                    const std::string &name) {}

std::vector<Block> AllocateBlocks(std::iostream &file, const Superblock &super,
                                 uint32_t metafile_id, std::size_t bytes);

}
}
