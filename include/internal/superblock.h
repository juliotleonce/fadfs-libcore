#ifndef FADFS_CORE_SUPERBLOCK_H
#define FADFS_CORE_SUPERBLOCK_H
#include "typedef.h"

int load_superblock(superblock_t *sb);

int save_superblock(const superblock_t *sb);

superblock_t create_superblock(uint64_t allocated_size);

#endif //FADFS_CORE_SUPERBLOCK_H