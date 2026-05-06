#include "include/internal/blocks.h"

#include "include/internal/fadisk.h"
#include "include/internal/state.h"

uint32_t alloc_block() {
    const superblock_t *sb = get_superblock_state();
    uint32_t bitmap_offset = sb->inode_bitmap_offset;
    const uint32_t bitmap_end = sb->inode_bitmap_offset + sb->bitmap_size;
    uint32_t bitmap_table_row = 0;

    while (bitmap_offset < bitmap_end) {
        uint64_t bitset;
        fadisk_read(&bitset, sizeof(uint64_t), bitmap_offset);
        bitset = ~bitset;
        if (bitset == 0) continue;

        for (int8_t i = 1; i < 65; i++) {
            if ((bitset & 1ULL << (64-i)) != 0) {
                const uint64_t new_bitset = ~bitset | 1ULL << (64-i);
                fadisk_write(&new_bitset, sizeof(uint64_t), bitmap_offset);
                return i + (bitmap_table_row << 6);
            }
        }

        bitmap_table_row++;
        bitmap_offset += 8;
    }

    return 0;
}

void free_block(const uint32_t block) {
    const superblock_t *sb = get_superblock_state();
    const uint8_t bit_pos = (block-1) % 64;
    const uint32_t bitmap_row = (uint32_t)(((float)block - 0.5) / 64);
    const uint32_t bitmap_offset = sb->inode_bitmap_offset + (bitmap_row << 3);

    uint64_t bitset;
    fadisk_read(&bitset, sizeof(uint64_t), bitmap_offset);
    bitset |= 1ULL << (64-bit_pos);
    fadisk_write(&bitset, sizeof(uint64_t), bitmap_offset);
}

size_t write_data_on_block(const uint32_t bloc, const uint32_t bloc_offset, buff_data_t *buff_data) {
    const superblock_t *sb = get_superblock_state();
    const int64_t data_offset = sb->data_block_offset + (bloc-1) * sb->block_size + bloc_offset;
    const uint32_t written_max_size = sb->block_size - bloc_offset;
    const size_t written = fadisk_write(
        buff_data->data + buff_data->data_seek,
        written_max_size,
        data_offset
    );

    buff_data->data_seek += written;
    return written;
}

size_t read_data_on_block(const uint32_t bloc, const uint32_t bloc_offset, buff_data_t *buff_data) {
    const superblock_t *sb = get_superblock_state();
    const int64_t data_offset = sb->data_block_offset + (bloc-1) * sb->block_size + bloc_offset;
    const uint32_t read_max_size = sb->block_size - bloc_offset;
    const size_t read = fadisk_read(
        buff_data->data + buff_data->data_seek,
        read_max_size,
        data_offset
    );

    buff_data->data_seek += read;
    return read;
}

