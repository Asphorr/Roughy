#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stdbool.h>

#define VFS_MAX_FILENAME 256

typedef struct vfs_node vfs_node_t;

typedef uint32_t (*read_type_t)(vfs_node_t*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(vfs_node_t*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(vfs_node_t*);
typedef void (*close_type_t)(vfs_node_t*);
typedef vfs_node_t* (*finddir_type_t)(vfs_node_t*, char* name);

struct vfs_node {
    char name[VFS_MAX_FILENAME];
    uint32_t flags;
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    finddir_type_t finddir;
    vfs_node_t* ptr;
};

void vfs_init(void);
vfs_node_t* vfs_root(void);
uint32_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
uint32_t vfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
void vfs_open(vfs_node_t* node, bool read, bool write);
void vfs_close(vfs_node_t* node);
vfs_node_t* vfs_finddir(vfs_node_t* node, char* name);

#endif // VFS_H
