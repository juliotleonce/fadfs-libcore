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
#define FILE_NOT_FOUND 0xA6
#define INVALID_PATH 0xA7
#define NOT_A_DIRECTORY 0xA8
#define END_OF_FILE 0xA9
#define NO_ERROR 0x00


#define PROPAGATE_ERROR(err) \
    do  { \
        int resp = err; \
        if(resp != NO_ERROR) return resp; \
    } while(0);

#define HANDLE_THEN_PROPAGATE_ERROR(err, action) \
    do  { \
        int resp = err; \
        if(resp != NO_ERROR) { \
            action; \
            return resp; \
         } \
    } while(0);

#define BIND_ERROR(func, action) \
    do {\
        int resp = func; \
        if(err_var != NO_ERROR) { \
            action; \
        } \
    } while(0)

#define EXIT_ON_ERROR(err) \
    do  { \
        int resp = err; \
        if(resp != NO_ERROR) { \
            printf("Error: %X | %d\n", -resp, -resp); \
            exit(resp); \
        } \
    } while(0);

#define PRINT_IF_ERROR(func, message) \
    do  { \
        int resp = func; \
        if(resp != NO_ERROR) { \
            printf("Error: %X | %d\n", -resp, -resp); \
            printf(message); \
            return resp; \
        } \
    } while (0)
#endif