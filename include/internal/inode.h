#ifndef FADFS_CORE_INODE_H
#define FADFS_CORE_INODE_H
#include "typedef.h"

ino_t alloc_inode();

void free_inode(ino_t ino);

int write_inode(ino_t ino, const inode_t *inode);

int read_inode(ino_t ino, inode_t *inode);

#endif