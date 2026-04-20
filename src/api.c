#include "include//public/api.h"

#include "include/internal/fadisk.h"
#include "include/internal/superblock.h"
#include "include/internal/typedef.h"

int fadfs_mkfs(const char *path, const uint64_t allocated_size) {
    fadisk_t disk;
    PROPAGATE_ERROR(fadisk_open(path, &disk));
    init_fadisk_state(&disk);

    const superblock_t new_sb = create_superblock(allocated_size);
    PROPAGATE_ERROR(save_superblock(&new_sb));

    return NO_ERROR;
}

int fadfs_mount(const char *path) {
    fadisk_t disk;
    PROPAGATE_ERROR(fadisk_open(path, &disk));
    init_fadisk_state(&disk);

    superblock_t sb;
    PROPAGATE_ERROR(load_superblock(&sb));
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


