#include "file.h"
#include "config.h"

// represents every filesystem on the system
struct filesystem *filesystems[PEACHOS_MAX_FILESYSTEMS];
struct file_descriptor *file_descriptors[PEACHOS_MAX_FILE_DESCRIPTORS];

static struct filesystems *fs_get_free_filesystem() {
    int i = 0;

    /*
    for(i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++) {
        if(filesystems[i] == 0) {
            return &filesystems[i]; //return a pointer to that position in the array
        }
    }
    */

    for(i = 0; i < PEACHOS_MAX_FILESYSTEMS && filesystems[i] != 0; ++i); // test. probably wont work in this case
}
