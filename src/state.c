#include "include/internal/state.h"

#include <string.h>

#include "include/internal/superblock.h"
#include "include/public/error.h"

static superblock_t g_sb;
static fadisk_t g_disk;

superblock_t *get_superblock_state() {
    return &g_sb;
}

void init_superblock_state(const superblock_t *sb) {
    memcpy(&g_sb, sb, sizeof(superblock_t));
}

int save_superblock_state() {
    PROPAGATE_ERROR(save_superblock(&g_sb));
    return NO_ERROR;
}

fadisk_t *get_fadisk_state() {
    return &g_disk;
}

void init_fadisk_state(const fadisk_t *disk) {
    memcpy(&g_disk, disk,sizeof(fadisk_t));
}






