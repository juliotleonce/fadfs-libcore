//
// Created by ajl420 on 4/16/26.
//

#ifndef FADFS_CORE_TYPE_DEF_H
#define FADFS_CORE_TYPE_DEF_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "constant.h"

typedef unsigned char byte;
typedef FILE *file_t;
typedef __uint32_t uint32_t;
typedef __ssize_t ssize_t;
typedef uint32_t ino_t;

typedef struct __attribute__((packed)) inode_t {
    int8_t used;
    int8_t type;
    uint32_t size;
    uint32_t direct[FADFS_MAX_DIRECT_BLOCKS];
    uint32_t indirect;
} inode_t;


typedef struct __attribute__((packed)) superblock_t {
    uint32_t magic;
    uint32_t block_size;
    uint32_t block_shift;
    uint32_t block_count;
    uint32_t allocated_inode_count;
    uint32_t inode_max_count;
    uint32_t inode_table_offset;
    uint32_t free_block_count;
    uint32_t inode_size;
    uint32_t inode_table_size;
    uint32_t bitmap_size;
    uint32_t inode_bitmap_offset;
    uint32_t data_block_offset;
} superblock_t;

enum fadisk_fs_status {
    FADISK_STATE_UNMOUNTED = 0,
    FADISK_STATE_MOUNTED,
};

enum fadisk_fd_status {
    NOT_OPENED = 0,
    OPENED,
};

typedef struct fadisk_t {
    file_t fd;
    enum fadisk_fs_status fs_status;
    bool is_fd_opened;
} fadisk_t;

typedef struct buff_data_t {
    uint32_t data_seek;
    uint32_t size;
    void *data;
} buff_data_t;

typedef struct range_t {
    uint32_t start;
    uint32_t end;
} range_t;

#endif //FADFS_CORE_TYPE_DEF_H