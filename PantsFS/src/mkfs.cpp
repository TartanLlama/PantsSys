#include <fstream>
#include <iterator>

#include "PantsFS.hpp"
#include "fmt/format.h"

using namespace pants;

void WriteBootSector(std::ostream &file) {
    for (std::size_t i = 0; i < g_pantsfs_block_size; ++i)
        file << '\0';
}

void WriteSuperblock(std::ostream &file, std::string volume_label) {
    Superblock superblock{};
    superblock.magic_number = g_pantsfs_magic_number;
    superblock.version = 1;
    superblock.block_count = 3;
    superblock.free_block_count = 0;
    superblock.metafile_count = 1;
    superblock.free_metafile_count = 0;
    std::copy(volume_label.begin(), volume_label.end(), superblock.name);

    auto block_bytes = reinterpret_cast<unsigned char *>(&superblock);
    std::copy(block_bytes, block_bytes + sizeof(Superblock),
              std::ostream_iterator<unsigned char>{file});
}

void WriteRootDir(std::ostream &file) {
    Metafile dir{};
    dir.block_count = 1;
    dir.type = static_cast<uint32_t>(FileType::Directory);
    dir.size = 0;
    dir.blocks[0] = 3; // first block after root dir metafile
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fmt::print(stderr, "Usage: mkfs.pantsfs <file> <label>");
        return -1;
    }

    std::ofstream file{argv[1]};
    if (!file) {
        fmt::print(stderr, "Failed to open file {}", argv[1]);
        return -1;
    }

    WriteBootSector(file);
    WriteSuperblock(file, argv[2]);
    WriteRootDir(file);
}
