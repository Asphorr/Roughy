#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include <stdbool.h>

#define SECTOR_SIZE 512

typedef enum {
    DISK_TYPE_ATA,
    DISK_TYPE_ATAPI,
    DISK_TYPE_UNKNOWN
} disk_type_t;

typedef struct {
    uint8_t  drive_num;
    disk_type_t type;
    uint32_t total_sectors;
    uint16_t bytes_per_sector;
    char     model[41];
    char     serial[21];
} disk_info_t;

void disk_init(void);
int disk_read(uint8_t drive, uint32_t lba, uint8_t sector_count, void* buffer);
int disk_write(uint8_t drive, uint32_t lba, uint8_t sector_count, const void* buffer);
bool disk_get_info(uint8_t drive, disk_info_t* info);
void disk_flush_cache(uint8_t drive);

#endif
