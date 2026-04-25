#include "include/internal/file.h"
#include "include/internal/state.h"
#include "include/public/error.h"
#include "include/internal/constant.h"
#include "include/internal/blocks.h"

static bool is_ranges_not_disjoint(range_t range1, range_t range2);
static range_t ranges_intersection(range_t range1, range_t range2);
static range_t get_indirect_block_range();

static int get_direct_blocks_from_range(
    inode_t *inode,
    range_t range,
    uint32_t blocks[]
);

static int get_indirect_blocks_from_range(
    inode_t *inode,
    range_t range,
    uint32_t blocks[]
);

static int get_inode_blocks_from_range(
    inode_t *inode,
    range_t range,
    uint32_t blocks[]
);

int write_file_data(inode_t *inode, const uint32_t file_offset, buff_data_t *data) {
    const superblock_t *sb = get_superblock_state();
    uint32_t block_offset = file_offset % sb->block_size;
    const range_t block_range_needed = {
        .start = file_offset / sb->block_size,
        .end = (file_offset + data->size) / sb->block_size
    };

    const uint32_t block_count = block_range_needed.end - block_range_needed.start;
    uint32_t blocks[block_count];
    get_inode_blocks_from_range(inode, block_range_needed, blocks);

    for (uint32_t i = 0; i < block_count; i++) {
        if (i != 0) block_offset = 0;
        inode->size += write_data_on_block(blocks[i], block_offset, data);
    }

    return NO_ERROR;
}

int read_file_data(inode_t *inode, uint32_t file_offset, buff_data_t *data) {
    const superblock_t *sb = get_superblock_state();
    uint32_t block_offset = file_offset % sb->block_size;
    const range_t block_range_needed = {
        .start = file_offset / sb->block_size,
        .end = (file_offset + data->size) / sb->block_size
    };

    const uint32_t block_count = block_range_needed.end - block_range_needed.start;
    uint32_t blocks[block_count];
    get_inode_blocks_from_range(inode, block_range_needed, blocks);

    for (uint32_t i = 0; i < block_count; i++) {
        if (i != 0) block_offset = 0;
        read_data_on_block(blocks[i], block_offset, data);
    }

    return NO_ERROR;
}


/**
 * PRIVATE FUNCTION IMPLEMENTATION GOES BELOW
 */

int get_inode_blocks_from_range(
    inode_t *inode,
    const range_t range,
    uint32_t blocks[]
) {
    const range_t direct_block_range = {0,  FADFS_MAX_DIRECT_BLOCKS };
    const range_t indirect_block_range = get_indirect_block_range();

    if (is_ranges_not_disjoint(range, direct_block_range)) {
        const range_t intersection = ranges_intersection(range, direct_block_range);
        get_direct_blocks_from_range(
            inode,
            intersection,
            blocks
        );
    }

    if (is_ranges_not_disjoint(range, indirect_block_range)) {
        const range_t intersection = ranges_intersection(range, indirect_block_range);
        get_indirect_blocks_from_range(
            inode,
            intersection,
            blocks + (intersection.start - range.start)
        );
    }
    return NO_ERROR;
}

int get_direct_blocks_from_range(
    inode_t *inode,
    const range_t range,
    uint32_t blocks[]
) {
    const uint32_t start = range.start;
    const uint32_t end = range.end;
    for (uint32_t i = start; i < end; i++) {
        uint32_t block = inode->direct[i];
        if (block == 0) {
            block = alloc_block();
            inode->direct[i] = block;
            blocks[i - start] = block;
            continue;
        }

        blocks[i - start] = block;
    }

    return NO_ERROR;
}

int get_indirect_blocks_from_range(inode_t *inode, const range_t range, uint32_t blocks[]) {
    const uint32_t start = range.start;
    const uint32_t end = range.end;
    const uint32_t block_count = end - start;
    const uint32_t block_offset = (start - FADFS_MAX_DIRECT_BLOCKS) * sizeof(uint32_t);

    if (inode->indirect == 0) inode->indirect = alloc_block();

    read_data_on_block(
        inode->indirect,
        block_offset,
        &(buff_data_t) {
            .data = blocks,
            .size = block_count * sizeof(uint32_t),
            .data_seek = 0
        }
    );

    for (uint32_t i = 0; i < block_count; i++) {
        if (blocks[i] == 0) {
            blocks[i] = alloc_block();
        }
    }

    write_data_on_block(
        inode->indirect,
        block_offset,
        &(buff_data_t) {
            .data = blocks,
            .size = block_count * sizeof(uint32_t),
            .data_seek = 0
        }
    );

    return NO_ERROR;
}

bool is_ranges_not_disjoint(const range_t range1, const range_t range2) {
    return range1.start <= range2.end && range2.start <= range1.end;
}

range_t ranges_intersection(const range_t range1, const range_t range2) {
    range_t intersection;
    intersection.start = range1.start > range2.start ? range2.start : range1.start;
    intersection.end = range1.end < range2.end ? range1.end : range2.end;
    return intersection;
}

range_t get_indirect_block_range() {
    const superblock_t *sb = get_superblock_state();
    const uint32_t indirect_max_range = FADFS_MAX_DIRECT_BLOCKS + sb->block_size / sizeof(uint32_t);
    return (range_t) { FADFS_MAX_DIRECT_BLOCKS + 1, indirect_max_range };
}

