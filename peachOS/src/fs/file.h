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


struct file_descriptor {
    // Descriptor index
    int index;
    struct filesystem *filesystem;

    // private data for internal file descriptor
    // to be used by file system to cast data into appropriate form and make use of
    // we are not necessarily aware of what this is
    void *private;

    // the disk the file descriptor should be used on
    struct disk *disk;
};


void fs_init();
int fopen(const char *filename, const char *mode);
void fs_insert_filesystem(struct filesystem *filesystem);
struct filesystem *fs_resolve(struct disk *disk);

#endif
