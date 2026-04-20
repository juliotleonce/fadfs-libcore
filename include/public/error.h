//
// Created by ajl420 on 4/20/26.
//

#ifndef FADFS_CORE_ERROR_H
#define FADFS_CORE_ERROR_H

#define INVALID_FILE_SYSTEM 0xA1
#define CANT_OPEN_FILE 0xA2
#define UNMOUNTED_DISK 0xA3
#define FADISK_READ_ERROR 0xA4
#define FADISK_WRITE_ERROR 0xA5
#define NO_ERROR 0x00


#define PROPAGATE_ERROR(err) \
    if(err != NO_ERROR) return err;

#define BIND_ERROR(err, action) \
    do { if(err != NO_ERROR) action; } while (0)

#define EXIT_ON_ERROR(err) \
    if(err != NO_ERROR) exit(err);

#endif //FADFS_CORE_ERROR_H