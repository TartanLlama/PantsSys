#include "PantsFS.hpp"

namespace pants {
namespace fs {

uint32_t NextUnusedBlock(std::istream &is, uint32_t block_id,
                         bool can_allocate = false); {
    is.seekg(ByteFromBlockID(block_id));
    uint32_t prev, next;
    is >> prev >> next;
    if (next != g_invalid_block_id) {
        return next;
    }

    if (can_allocate) {
        // TODO, allocate new block group
    }

    return next;
}


uint32_t PrevUnusedBlock(std::istream &is, uint32_t block_id) {
    is.seekg(ByteFromBlockID(block_id));
    uint32_t prev;
    is >> prev;
    return prev;
}

std::vector<Block> AllocateBlocks(std::iostream &file, const Superblock &super,
                                  uint32_t metafile_id, std::size_t bytes) {
    uint32_t block_group_id = BlockGroupIDFromMetafileID(metafile_id);
    uint32_t block_group_block_id = BlockIDFromBlockGroupID(block_group_id);
    uint32_t block_byte_pos = ByteFromBlockID(block_group_block_id);

    file.seekg(block_byte_pos);
    BlockGroup group;
    file >> group;

    auto Dec = [](std::size_t &b) {
        b = (b < g_block_size) ? 0 : (b - g_block_size);
    };

    std::vector<Block> blocks{};
    blocks.reserve(bytes / g_block_size);

    auto unused_block = group.first_unused_block;
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

    return blocks;
}

}
}
