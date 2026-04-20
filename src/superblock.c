#include "include/internal/superblock.h"

#include "include/internal/fadisk.h"
#include "include/internal/state.h"
#include "include/public/error.h"
#include "include/internal/constant.h"

int load_superblock(superblock_t *sb) {
    PROPAGATE_ERROR(fadisk_read(sb, sizeof(superblock_t), 0))
    if (sb->magic != FADFS_MAGIC) return -INVALID_FILE_SYSTEM;

    return NO_ERROR;
}

int save_superblock(const superblock_t *sb) {
    PROPAGATE_ERROR(fadisk_write(sb, sizeof(superblock_t), 0))
    return NO_ERROR;
}

superblock_t create_superblock(const uint64_t allocated_size) {
    const uint32_t block_count = allocated_size >> FADFS_BLOCK_SHIFT;
    const uint32_t inode_count = allocated_size / FADFS_FILE_AVG_SIZE;
    const uint32_t inode_table_size = inode_count * FADFS_INODE_SIZE;
    const uint32_t bitmap_size = (block_count + 7) >> 3;
    const uint32_t inode_bitmap_offset = inode_table_size + SUPER_BLOCK_SIZE;
    const uint32_t data_block_offset = inode_table_size + bitmap_size + SUPER_BLOCK_SIZE;

    return (superblock_t) {
        .magic = FADFS_MAGIC,
        .block_size = FADFS_BLOCK_SIZE,
        .block_shift = FADFS_BLOCK_SHIFT,
        .block_count = block_count,
        .free_block_count = block_count,
        .inode_count = inode_count,
        .inode_table_offset = SUPER_BLOCK_SIZE,
        .inode_size = FADFS_INODE_SIZE,
        .bitmap_size = bitmap_size,
        .inode_table_size = inode_table_size,
        .inode_bitmap_offset = inode_bitmap_offset,
        .data_block_offset = data_block_offset,
    };
}
