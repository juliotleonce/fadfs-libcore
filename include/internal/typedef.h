//
// Created by ajl420 on 4/16/26.
//

#ifndef FADFS_CORE_TYPE_DEF_H
#define FADFS_CORE_TYPE_DEF_H
#include <stdio.h>

typedef unsigned char byte;
typedef FILE *file_t;
typedef __uint32_t uint32;

typedef struct inode_t {

} inode_t;

typedef struct block_t {

} block_t;

typedef struct superblock_t {
    char magic_key[8];
} superblock_t;

typedef struct fadisk_t {
    file_t *file_ptr;
    superblock_t superblock;
} fadisk_t;

#endif //FADFS_CORE_TYPE_DEF_H