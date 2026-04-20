#include "include/internal/typedef.h"
#include "include/internal/fadisk.h"
#include "include/public/error.h"


int fadisk_open(const char *path, fadisk_t *disk) {
    disk->fd = fopen(path, "a+");
    if (disk->fd == NULL) return -CANT_OPEN_FILE;
    disk->fs_status = FADISK_STATE_MOUNTED;
    disk->is_fd_opened = true;

    return NO_ERROR;
}

int fadisk_close(void) {
    FADISK_OPENED_REQUIRED();
    const fadisk_t *disk = get_fadisk_state();
    PROPAGATE_ERROR(fclose(disk->fd));

    return NO_ERROR;
}

int fadisk_read(void *buf, const size_t n, const uint32_t offset) {
    FADISK_OPENED_REQUIRED();
    const fadisk_t *disk = get_fadisk_state();

    fseek(disk->fd, offset, SEEK_SET);

    if (fread(buf, n, 1, disk->fd) != n) {
        return -FADISK_READ_ERROR;
    }

    fflush(disk->fd);
    return NO_ERROR;
}

int fadisk_write(const void *buf, const size_t n, const uint32_t offset) {
    FADISK_MOUNTED_REQUIRED();
    const fadisk_t *disk = get_fadisk_state();

    fseek(disk->fd, offset, SEEK_SET);

    if (fwrite(buf, n, 1, disk->fd) != n) {
        return -FADISK_WRITE_ERROR;
    }

    fflush(disk->fd);
    return NO_ERROR;
}


