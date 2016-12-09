#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

namespace pants {
namespace fs {

/// Describes a file
struct Metafile {
    uint32_t type; //< Matches a [pants::fs::FileType]()
    uint32_t size; //< Size of the file in bytes
    uint32_t block_count; //< Number of blocks currently used by the file
    uint32_t blocks[29]; //< IDs of the blocks storing the file contents
};


/// When a metafile is unused, it participates in a linked list of unused metafiles
struct UnusedMetafile {
    uint32_t prev; //< The block ID of the previous unused metafile
    uint32_t next; //< The block ID of the next unused metafile
    uint32_t padding[30]; //< Blank padding to match the size of [pants::fs::Metafile]()

    UnusedMetafile (uint32_t prev)
        : prev{prev}, next{prev+1} {}

    UnusedMetafile (uint32_t prev, uint32_t next)
        : prev{prev}, next{next} {}
};

constexpr uint16_t g_magic_number = 0x9A27; //< Magic number identifying a PantsFS file system
constexpr uint16_t g_block_size = 512; //< Size of each file system block in bytes
constexpr uint16_t g_metafiles_per_block_group = 1024; //< Number of metafiles per block group
constexpr uint16_t g_metafile_size = sizeof(Metafile); //< Size of each metafile in bytes
constexpr uint16_t g_blocks_per_metafile = 29; //< Number of file system blocks per metafile

/// Number of blocks in each block group
constexpr uint16_t g_blocks_per_block_group =
    (g_metafile_size * g_metafiles_per_block_group) /
        g_block_size +
    g_blocks_per_metafile * g_metafiles_per_block_group;

/// The block ID of the first block group
constexpr uint16_t g_first_block_group_block_id = 3;

/// Used to mark an invalid block ID
constexpr uint16_t g_invalid_block_id = 0;


/// The master block for the file system
///
/// This tracks global information about the file system
struct Superblock {
    uint16_t magic_number; //< Should always store g_magic_number
    uint16_t version; //< Version of PantsFS which is stored on that partition
    uint32_t block_count; //< Number of blocks allocated
    uint32_t free_block_count; //< Number of blocks which are currently free
    uint32_t metafile_count; //< Number of metafiles allocated
    uint32_t free_metafile_count; //< Number of metafiles which are currently free
    char name[64]; //< Label for the file system
};

/// Type of a file
enum class FileType : uint32_t {
    Directory,
    RegularFile,
};




/// Manages a group of blocks and metafiles
struct BlockGroup {
    Metafile metafile_table[g_metafiles_per_block_group]; //< The metafiles tracked by this block group
    uint16_t first_unused_metafile; //< Index into [pants::fs::BlockGroup::metafile_table]() for the first unused metafile
    uint16_t unused_metafile_count; //< Number of unused metafiles in [pants::fs::BlockGroup::metafile_table]()
    uint16_t first_unused_block; //< Group-local block ID of the first unused block for this block group
    uint16_t unused_block_count; //< Number of unused blocks in this block group
};


/// A directory is just a metafile with a name attached
struct Directory {
    uint32_t metafile;
    uint8_t name_size;
    char name[64];
};


/// A simple wrapper around a uint32_t for type safety
struct Block {
    uint32_t id;
};


/// Read a metafile in from a stream
inline std::istream &operator>>(std::istream &is, Metafile &mf) {
    is >> mf.type >> mf.size >> mf.block_count;
    for (auto &&block : mf.blocks)
        is >> block;

    return is;
}

/// Read a block group in from a stream
std::istream &operator>>(std::istream &is, BlockGroup &bg);


/// Get a byte offset into the file system from a block ID
inline uint32_t ByteFromBlockID(uint32_t block_id) {
    return block_id * g_block_size;
}


/// Get the block group block ID from a metafile ID
inline uint32_t BlockGroupIDFromMetafileID(uint32_t metafile_id) {
    return metafile_id / g_metafiles_per_block_group;
}

/// Get the block ID of a block group from a block group ID
inline uint32_t BlockIDFromBlockGroupID(uint32_t block_group_id) {
    return block_group_id * g_blocks_per_block_group +
           g_first_block_group_block_id;
}


/// Get a byte offset into the file system from a metafile ID
inline uint32_t ByteFromMetafileID(uint32_t metafile_id) {
    uint32_t block_group_id = BlockGroupIDFromMetafileID(metafile_id);
    uint32_t block_group_block_id = BlockIDFromBlockGroupID(block_group_id);

    //TODO implement
    (void)block_group_id;
    (void)block_group_block_id;
    return 0;
}

uint16_t NextUnusedBlock(std::istream &is, uint32_t block_id,
                         bool can_allocate = false);

uint16_t PrevUnusedBlock(std::istream &is, uint32_t block_id);


inline void SetPrevUnusedBlock(std::ostream &os, uint32_t block_id, uint32_t prev) {
    os.seekp(ByteFromBlockID(block_id));
    os << prev;
}

inline void SetNextUnusedBlock(std::ostream &os, uint32_t block_id, uint32_t next) {
    os.seekp(ByteFromBlockID(block_id) + sizeof(uint32_t));
    os << next;
}

inline uint32_t CreateFile(std::iostream &fs_file, uint32_t dir_metafile_id,
                    const std::string &name) {
//TODO implement
    (void)fs_file;
    (void)dir_metafile_id;
    (void)name;
    return 0;
}


/// Allocate blocks to cover the given size
std::vector<Block> AllocateBlocks(std::iostream &file, Superblock &super,
                                 uint32_t metafile_id, std::size_t bytes);

/// Allocate new block group at the end of the file system
void AllocateBlockGroup(std::iostream &is, Superblock &super);

}
}
