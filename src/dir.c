#include  "include/internal/dir.h"

#include <stdlib.h>
#include <string.h>

#include "include/internal/inode.h"
#include "include/internal/inode_data.h"
#include "include/internal/state.h"
#include "include/public/error.h"

int mkrootdir() {
    superblock_t *sb = get_superblock_state();
    const ino32_t root_ino = alloc_inode();

    inode_t root_inode = {
        .used = 1,
        .type = FADFS_TYPE_DIR,
        .size = 0
    };

    dirent_t parent_dir = {
        .ino = root_ino,
        .name = ".."
    };

    dirent_t self_dir = {
        .ino = root_ino,
        .name = "."
    };

    dir_insert(&root_inode, &parent_dir);
    dir_insert(&root_inode, &self_dir);
    write_inode(root_ino, &root_inode);

    sb->root_ino = root_ino;
    return NO_ERROR;
}

int dir_insert(inode_t *dir_inode, dirent_t *dirent) {
    buff_data_t dirent_data = {
        .data = dirent,
        .size = sizeof(dirent_t)
    };

    write_inode_data(dir_inode, dir_inode->size, &dirent_data);

    return NO_ERROR;
}

int dir_get_entry_from_index(inode_t *dir_inode, const uint32_t index, dirent_t *dirent) {
    buff_data_t dirent_data = {
        .data = calloc(sizeof(dirent_t), 1),
        .size = sizeof(dirent_t)
    };

    read_inode_data(dir_inode, index * sizeof(dirent_t), &dirent_data);
    memcpy(dirent, dirent_data.data, sizeof(dirent_t));
    free(dirent_data.data);

    return NO_ERROR;
}

int dir_set_entry_from_index(inode_t *dir_inode, const uint32_t index, dirent_t *dirent) {
    buff_data_t dirent_data = {
        .data = dirent,
        .size = sizeof(dirent_t)
    };

    write_inode_data(dir_inode, index * sizeof(dirent_t), &dirent_data);

    return NO_ERROR;
}

int32_t dir_get_entry_index(inode_t *dir_inode, const char *filename) {
    const uint32_t entry_count = dir_inode->size / sizeof(dirent_t);
    for (uint32_t i = 0; i < entry_count; i++) {
        dirent_t dirent;
        dir_get_entry_from_index(dir_inode, i, &dirent);
        if (strcmp(dirent.name, filename) == 0) return (int32_t) i;
    }

    return -1;
}

int dir_remove(inode_t *dir_inode, const char *filename) {
    const int32_t entry_index = dir_get_entry_index(dir_inode, filename);
    const uint32_t dir_size = dir_inode->size;
    const uint32_t last_entry_index = dir_size / sizeof(dirent_t) - 1;
    dirent_t last_entry;

    if (entry_index == -1) return -FILE_NOT_FOUND;
    dir_get_entry_from_index(dir_inode, last_entry_index, &last_entry);
    dir_set_entry_from_index(dir_inode, entry_index, &last_entry);
    truncate_inode_data(dir_inode, dir_size - sizeof(dirent_t));

    return NO_ERROR;
}

int dir_lookup(inode_t *dir_inode, dirent_t *found_dirent, const char *filename) {
    const int32_t entry_index = dir_get_entry_index(dir_inode, filename);
    if (entry_index == -1) return -FILE_NOT_FOUND;

    dir_get_entry_from_index(dir_inode, entry_index, found_dirent);

    return NO_ERROR;
}


