#ifndef FADFS_CORE_DIR_H
#define FADFS_CORE_DIR_H
#include "typedef.h"

int mkrootdir();

int dir_insert(inode_t *dir_inode, dirent_t *dirent);

int dir_get_entry_from_index(inode_t *dir_inode, uint32_t index, dirent_t *dirent);

int dir_set_entry_from_index(inode_t *dir_inode, uint32_t index, dirent_t *dirent);

int32_t dir_get_entry_index(inode_t *dir_inode, const char *filename);

int dir_remove(inode_t *dir_inode, const char *filename);

ino32_t dir_lookup(inode_t *dir_inode, const char *filename);

#endif