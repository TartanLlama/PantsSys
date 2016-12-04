#include "PantsFS.hpp"
#include <algorithm>

namespace pants {
namespace fs {

uint32_t NextUnusedBlock(std::istream &is, uint32_t block_id,
                         bool can_allocate) {
    is.seekg(ByteFromBlockID(block_id));
    uint32_t prev, next;
    is >> prev >> next;
    if (next != g_invalid_block_id) {
        return next;
    }

    if (can_allocate) {
        // TODO allocate new block group
    }

    return next;
}


uint32_t PrevUnusedBlock(std::istream &is, uint32_t block_id) {
    is.seekg(ByteFromBlockID(block_id));
    uint32_t prev;
    is >> prev;
    return prev;
}

void AllocateBlockGroup(std::iostream &is, Superblock &super) {
    BlockGroup group;
    auto first_metafile_block_id = super.block_count;

    uint32_t i = 0;
    for (i = first_metafile_block_id;
         i <= g_metafiles_per_block_group + first_metafile_block_id;
         ++i) {
        UnusedMetafile unused {i};
        std::copy(reinterpret_cast<unsigned char *>(&unused),
                  reinterpret_cast<unsigned char *>(&unused + sizeof(unused)),
                  reinterpret_cast<unsigned char *>(group.metafile_table));
    }

    UnusedMetafile unused {i, g_invalid_block_id};
    std::copy(reinterpret_cast<unsigned char *>(&unused),
              reinterpret_cast<unsigned char *>(&unused + sizeof(unused)),
              reinterpret_cast<unsigned char *>(group.metafile_table));

    super.block_count += g_blocks_per_block_group;

    for (size_t i = 0; i < g_blocks_per_metafile * g_block_size; ++i) {
        is << uint32_t{0};
    }
}


std::vector<Block> AllocateBlocks(std::iostream &file, Superblock &super,
                                  uint32_t metafile_id, std::size_t bytes) {
    uint32_t block_group_id = BlockGroupIDFromMetafileID(metafile_id);
    uint32_t block_group_block_id = BlockIDFromBlockGroupID(block_group_id);
    uint32_t block_byte_pos = ByteFromBlockID(block_group_block_id);

    file.seekg(block_byte_pos);
    BlockGroup group;
    file >> group;

    //decrement by block size, but don't go past zero
    auto Dec = [&group](std::size_t &b) {
        --group.unused_block_count;

        b = (b < g_block_size) ? 0 : (b - g_block_size);
    };

    std::vector<Block> blocks{};
    blocks.reserve(bytes / g_block_size);

    auto unused_block = group.first_unused_block;

    //TODO what if all blocks are used?

    blocks.push_back(Block{unused_block});
    Dec(bytes);

    while (bytes > 0) {
        auto can_allocate = true;
        unused_block = NextUnusedBlock(file, unused_block, can_allocate);
        blocks.push_back(Block{unused_block});
        Dec(bytes);
    }

    // Relink unused blocks
    auto first = PrevUnusedBlock(file, blocks.front().id);
    auto last = NextUnusedBlock(file, blocks.back().id);
    SetNextUnusedBlock(file, first, last);
    SetPrevUnusedBlock(file, last, first);

    group.first_unused_block = last;

    return blocks;
}

std::istream &operator>>(std::istream &is, BlockGroup &bg) {
    for (auto &&mf : bg.metafile_table)
        is >> mf;
    is >> bg.first_unused_metafile >> bg.unused_metafile_count >>
        bg.first_unused_block >> bg.unused_block_count;

    return is;
}


}
}
