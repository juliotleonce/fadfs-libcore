#include "include//public/api.h"

#include <string.h>

#include "include/internal/dir.h"
#include "include/internal/fadisk.h"
#include "include/internal/inode.h"
#include "include/internal/path.h"
#include "include/internal/superblock.h"
#include "include/internal/typedef.h"

int fadfs_mkfs(const char *path, const uint64_t allocated_size) {
    fadisk_t disk;
    PROPAGATE_ERROR(fadisk_open(path, &disk));
    init_fadisk_state(&disk);

    const superblock_t new_sb = create_superblock(allocated_size);
    init_superblock_state(&new_sb);

    PROPAGATE_ERROR(save_superblock_state());
    PROPAGATE_ERROR(mkrootdir());
    PROPAGATE_ERROR(save_superblock_state());
    PROPAGATE_ERROR(fadisk_close());

    return NO_ERROR;
}

int fadfs_mount(const char *path) {
    fadisk_t disk;
    PROPAGATE_ERROR(fadisk_open(path, &disk));
    init_fadisk_state(&disk);

    superblock_t sb;

    HANDLE_THEN_PROPAGATE_ERROR(
        load_superblock(&sb),
        { fadisk_close(); }
    );

    init_superblock_state(&sb);
    get_fadisk_state()->fs_status = FADISK_STATE_MOUNTED;

    return NO_ERROR;
}

int fadfs_umount(void) {
    PROPAGATE_ERROR(fadisk_close());
    init_fadisk_state(&(fadisk_t){0});
    init_superblock_state(&(superblock_t){0});

    return NO_ERROR;
}

int fadfs_read_superblock(fadfs_superblock_t *sb) {
    const superblock_t *sb_stat = get_superblock_state();
    *sb = (fadfs_superblock_t){
        .inode_max_count = sb_stat->inode_max_count,
        .block_size = sb_stat->block_size,
        .data_block_offset = sb_stat->data_block_offset,
        .inode_table_offset = sb_stat->inode_table_offset,
        .root_ino = sb_stat->root_ino
    };

    strcpy(sb->magic, sb_stat->magic);
    return NO_ERROR;
}

int fadfs_stat(const char *path, fadfs_stat_t *st) {
    dirent_t target_dirent;
    inode_t target_inode;

    PROPAGATE_ERROR(resolve_path(path, &target_dirent));
    PROPAGATE_ERROR(read_inode(target_dirent.ino, &target_inode));

    st->ino = target_dirent.ino;
    st->type = target_inode.type;
    st->size = target_inode.size;

    return NO_ERROR;
}

int fadfs_opendir(const char *path, fadfs_file_t *dh) {
    dirent_t target_dirent;
    inode_t target_inode;

    PROPAGATE_ERROR(resolve_path(path, &target_dirent));
    PROPAGATE_ERROR(read_inode(target_dirent.ino, &target_inode));

    if (target_inode.type != FADFS_TYPE_DIR)
        return -NOT_A_DIRECTORY;
    dh->ino = target_dirent.ino;

    return NO_ERROR;
}

int fadfs_readdir(fadfs_file_t *dh, fadfs_dirent_t *entry) {
    inode_t dir_inode;
    dirent_t read_dirent;
    read_inode(dh->ino, &dir_inode);

    if (dh->offset >= dir_inode.size) return -END_OF_FILE;
    dir_get_entry_from_index(&dir_inode, dh->offset / sizeof(dirent_t), &read_dirent);

    strcpy(entry->name, read_dirent.name);
    entry->ino = read_dirent.ino;
    dh->offset += sizeof(dirent_t) + 1;

    return NO_ERROR;
}




