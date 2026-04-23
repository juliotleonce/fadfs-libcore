#include "include/internal/inode.h"

#include "include/internal/fadisk.h"
#include "include/public/error.h"

int64_t get_inode_offset(ino_t ino);

ino_t alloc_inode() {
    const superblock_t *sb = get_superblock_state();
    ino_t ino = sb->allocated_inode_count;

    while (true) {
        const int64_t offset = get_inode_offset(ino);
        int8_t used;

        fadisk_read(&used, sizeof(int8_t), offset);
        if (used == 0) return ino;

        ino = (ino + 1) % sb->inode_max_count;
    }
}

void free_inode(const ino_t ino) {
    const int64_t offset = get_inode_offset(ino);
    const int8_t used = 0;
    fadisk_write(&used, sizeof(int8_t), offset);
}

int write_inode(const ino_t ino, const inode_t *inode) {
    const int64_t offset = get_inode_offset(ino);
    PROPAGATE_ERROR(fadisk_write(inode, sizeof(inode_t), offset));
    return NO_ERROR;
}

int read_inode(const ino_t ino, inode_t *inode) {
    const int64_t offset = get_inode_offset(ino);
    PROPAGATE_ERROR(fadisk_read(inode, sizeof(inode_t), offset));
    return NO_ERROR;
}

int64_t get_inode_offset(const ino_t ino) {
    const superblock_t *sb = get_superblock_state();
    return sb->inode_table_offset + ino * FADFS_INODE_SIZE;
}


