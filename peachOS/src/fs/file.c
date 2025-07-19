#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "status.h"
#include "disk/disk.h"
#include "fat/fat16.h"
#include "memory/heap/kheap.h"
#include "kernel.h"


// represents every filesystem on the system
struct filesystem *filesystems[PEACHOS_MAX_FILESYSTEMS];
struct file_descriptor *file_descriptors[PEACHOS_MAX_FILE_DESCRIPTORS];

static struct filesystem **fs_get_free_filesystem() {
    int i = 0;

    
    for(i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++) {
        if(filesystems[i] == 0) {
            return &filesystems[i]; //return a pointer to that position in the array
        }
    }
    

    // search through file systems
    // trying to be fancy with my code here. may not work
    //for(i = 0; i < PEACHOS_MAX_FILESYSTEMS && filesystems[i] != 0; ++i) {} // test. probably wont work in this case
    if(i != 0)
        return &filesystems[i];
        
    return 0;
}


void fs_insert_filesystem(struct filesystem *filesystem) {
    struct filesystem **fs = fs_get_free_filesystem();

    if(!fs) {
        print("Problem inserting filesystem");
        while(1) {} // will replace with panic function later
    } 

    *fs = filesystem; // this fills the location provided by fs_get_free_filesystem with the filesystem passed to this function
}


// load filesystems that are build into the kernel
static void fs_static_load() {
    fs_insert_filesystem(fat16_init());
}


// zero out filesystems memroy before loading
void fs_load() {
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}


// begin procress and prepare arrays to load kernel file systems
void fs_init() {
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}


static int file_new_descriptor(struct file_descriptor **desc_out) {
    int res = -ENOMEM;

    for(int i = 0; i < PEACHOS_MAX_FILE_DESCRIPTORS; i++) {
        if(file_descriptors[i] == 0) { // if we find an unallocated entry in this array
            struct file_descriptor *desc = kzalloc(sizeof(struct file_descriptor));
            desc->index = i + 1; // descriptors start at 1
            file_descriptors[i] = desc;
            *desc_out = desc;
            res = 0;
            break;
        }
    }

    return res;
}


static struct file_descriptor *file_get_descriptor(int fd) {
    if(fd <= 0 || fd >= PEACHOS_MAX_FILE_DESCRIPTORS) {
        return 0;
    }

    //descriptors start at 1
    int index = fd-1;
    return file_descriptors[index];
}


// loop through filesystems, and call resolve function to determing filesystem
struct filesystem *fs_resolve(struct disk *disk) {
    struct filesystem *fs = 0;
    for(int i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++) { // loop through all filesystems loaded into the system
        // file systems define their own resolve function, so we do not need to define
        if(filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0) { // resolve function is defined in the filesystem struct
            fs = filesystems[i];
            break;
        }
    }

    return fs;
}


// FILE_MODE is just an enum that represends the mode a file is opened in
FILE_MODE file_get_mode_by_string(const char *str) {
    FILE_MODE mode = FILE_MODE_INVALID;

    if(strncmp(str, "r", 1) == 0) {
        mode = FILE_MODE_READ;
    }
    else if(strncmp(str, "w", 1) == 0) {
        mode = FILE_MODE_WRITE;
    }
    else if(strncmp(str, "a", 1) == 0) {
        mode = FILE_MODE_APPEND;
    }

    return mode;
}


int fopen(const char *filename, const char *mode) {
    int res = 0;
    struct path_root *root_path = pathparser_parse(filename, NULL);

    // make sure we have a root path, and that the path passed is not just 0:/, it is a file eg. 0:/file.txt
    if(!root_path || !root_path->first) {
        res = -EINVARG;
        goto out;
    }

    // ensure the drive exists
    struct disk *disk = disk_get(root_path->drive_no) // all this does is make sure we are getting disk 0, since we only support one disk
    if(!disk || !disk->filesystem) {
        res = -EIO;
        goto out;
    }




out:
    return res; 
}
