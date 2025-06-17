#ifndef FILE_H
#define FILE_H

typedef unsigned int FILE_SEEK_MODE;
enum {
    SEEK_SET,
    SEEK_CUR,
    SEEK_END,
};


typedef unsigned int FILE_MODE;

enum {
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};


// FS_OPEN_FUNCTION will be a pointer to the respective file systems open function, will probabky be stored in some kind of array
struct disk;
typedef void*(*FS_OPEN_FUNCTION)(struct disk *disk, struct path_part *path, FILE_MODE mode);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk *disk); // determines if the disk of the specified filesysted is valid


struct filesystem {
    // filesystem should return 0 from resolve if the provided disk is using its file system
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;

    char name[20];
};

#endif
