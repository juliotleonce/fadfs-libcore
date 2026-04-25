#ifndef FADFS_CORE_DATA_BLOCS_H
#define FADFS_CORE_DATA_BLOCS_H
#include "typedef.h"

uint32_t alloc_block();

void free_block(uint32_t block);

size_t write_data_on_block(uint32_t bloc, uint32_t bloc_offset, buff_data_t *buff_data);

size_t read_data_on_block(uint32_t bloc, uint32_t bloc_offset, buff_data_t *buff_data);

#endif //FADFS_CORE_DATA_BLOCS_H