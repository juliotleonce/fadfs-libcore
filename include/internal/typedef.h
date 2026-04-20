//
// Created by ajl420 on 4/16/26.
//

#ifndef FADFS_CORE_TYPE_DEF_H
#define FADFS_CORE_TYPE_DEF_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned char byte;
typedef FILE *file_t;
typedef __uint32_t uint32_t;
typedef __ssize_t ssize_t;

typedef struct inode_t {

} inode_t;

typedef struct block_t {

} block_t;

typedef struct __attribute__((packed)) superblock_t {
    uint32_t magic;
    uint32_t block_size;
    uint32_t block_shift;
    uint32_t block_count;
    uint32_t inode_count;
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

#endif //FADFS_CORE_TYPE_DEF_H