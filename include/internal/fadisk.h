#ifndef FADFS_CORE_FADISK_CTL_H
#define FADFS_CORE_FADISK_CTL_H
#include "include/internal/state.h"
#include "include/public/error.h"

#define FADISK_MOUNTED_REQUIRED() \
    if(get_fadisk_state()->fs_status != FADISK_STATE_MOUNTED) \
        return -UNMOUNTED_DISK;

#define FADISK_OPENED_REQUIRED() \
    if(get_fadisk_state()->is_fd_opened != true) \
        return -FADISK_WRITE_ERROR;

int fadisk_open(const char *path, fadisk_t *disk);

int fadisk_close(void);

int fadisk_read(void *buf, size_t n, uint32_t offset);

int fadisk_write(const void *buf, size_t n, uint32_t offset);

#endif