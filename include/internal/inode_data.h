#ifndef FADFS_CORE_FILE_H
#define FADFS_CORE_FILE_H

#include "typedef.h"

int write_inode_data(inode_t *inode, uint32_t file_offset, buff_data_t *data);

int read_inode_data(inode_t *inode, uint32_t file_offset, buff_data_t *data);

int truncate_inode_data(inode_t *inode, uint32_t new_size);

#endif