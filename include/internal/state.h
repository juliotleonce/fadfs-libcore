//
// Created by ajl420 on 4/18/26.
//

#ifndef FADFS_CORE_STATE_H
#define FADFS_CORE_STATE_H
#include "typedef.h"

superblock_t *get_superblock_state();

void init_superblock_state(const superblock_t *sb);

fadisk_t *get_fadisk_state();

void init_fadisk_state(const fadisk_t *disk);

#endif //FADFS_CORE_STATE_H