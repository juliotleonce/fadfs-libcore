//
// Created by ajl420 on 4/16/26.
//

#ifndef FADFS_CORE_API_H
#define FADFS_CORE_API_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#define FADFS_NAME_MAX 255

#define FADFS_O_RDONLY  0x01
#define FADFS_O_WRONLY  0x02
#define FADFS_O_RDWR    0x03
#define FADFS_O_CREAT   0x04
#define FADFS_O_TRUNC   0x08

#define FADFS_SEEK_SET  0
#define FADFS_SEEK_CUR  1
#define FADFS_SEEK_END  2

#define FADFS_TYPE_FILE 0x01
#define FADFS_TYPE_DIR  0x02

typedef struct {
    uint32_t ino;
    uint8_t type;
    uint32_t size;
    uint32_t blocks;
} fadfs_stat_t;

typedef struct {
    uint32_t ino;
    uint32_t offset;
    uint8_t flags;
} fadfs_file_t;

typedef struct {
    uint32_t ino;
    char name[FADFS_NAME_MAX + 1];
} fadfs_dirent_t;

int fadfs_mkfs(const char *path, uint64_t allocated_size);
int fadfs_mount(const char *path);
int fadfs_umount(void);

int fadfs_open(const char *path, uint8_t flags, fadfs_file_t *fh);
int fadfs_close(fadfs_file_t *fh);
ssize_t fadfs_read(fadfs_file_t *fh, void *buf, size_t n);
ssize_t fadfs_write(fadfs_file_t *fh, const void *buf, size_t n);
int fadfs_seek(fadfs_file_t *fh, int32_t offset, uint8_t whence);
int fadfs_unlink(const char *path);
int fadfs_rename(const char *src, const char *dst);
int fadfs_stat(const char *path, fadfs_stat_t *st);
int fadfs_fstat(fadfs_file_t *fh, fadfs_stat_t *st);
int fadfs_truncate(const char *path, uint32_t new_size);

int fadfs_mkdir(const char *path);
int fadfs_rmdir(const char *path);
int fadfs_opendir(const char *path, fadfs_file_t *dh);
int fadfs_readdir(fadfs_file_t *dh, fadfs_dirent_t *entry);
int fadfs_closedir(fadfs_file_t *dh);

#endif
