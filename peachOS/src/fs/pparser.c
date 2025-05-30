#include "fs/pparser.h"
#include "kernel.h"
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "status.h"

static int pathparser_path_valid_format(const char *filename) { // does not support relative paths for now
    int len = strnlen(filename, PEACHOS_MAX_PATH);

    // makes sure path is proper length, that is starts with a drive number, and that is begins with path specifier ':/'
    return (len >= 3 && isdigit(filename[0]) && memcmp((void*) &filename[1], ":/", 2) == 0);
}


static int pathparser_get_drive_by_path(const char **path) {
    if(!pathparser_path_valid_format(*path)) {
        return -EBADPATH;
    }

    // extract drive number from path
    int drive_no = tonumericdigit(*path[0]);

    // Add 3 bytes to skip the drive number e.g. 0:/ 1:/ 2:/
    *path += 3;
    return drive_no;
}


static struct path_root *pathparser_create_root(int drive_number) {
    struct path_root *path_r = kzalloc(sizeof(struct path_root)); // allocate memory for path_root
    path_r->drive_no = drive_number;
    path_r->first = 0;
    return path_r;
}


// this function will extract each part of the path between slashes
static const char *pathparser_get_path_part(const char **path) {
    // Is it important to used the passed path as is, because we do not know who owns the memory is resides in. It could be overwritten and deleted
    // This is why we allocate new memory and copy the path string into it
    char *result_path_part = kzalloc(PEACHOS_MAX_PATH); // do we need to allocate max path len each time? no strlen?
    
    int i = 0;
    while(**path != '/' && **path != 0x00) { // loop until a slash or null terminator is met
        result_path_part[i] = **path;
        *path += 1;
        i++;
    }

    if(**path == '/') {
        // if function is called while current byte is '/', skip the byte. This way the next time this function is called, the while loop will be able to run
        *path += 1;
    }


    // if i is never iterated, delete the allocated memory because none if it was used
    if(i == 0) {
        kfree(result_path_part);
        result_path_part = 0;
    }

    return result_path_part;
}


struct path_part *pathparser_parse_path_part(struct path_part *last_part, const char **path) {
    const char *path_part_str = pathparser_get_path_part(path);
    if(!path_part_str) {
        return 0;
    }

    struct path_part *part = kzalloc(sizeof(struct path_part));
    part->part = path_part_str;
    part->next = 0x00; // initialize next pointer to null
    
    // if the last part exists, (this is not the first part of a path) add current part pointer to last part
    if(last_part) {
        last_part->next = part;
    } 

    return part;
}


void pathparser_free(struct path_root *root) {
    struct path_part *part = root->first;
    while(part) {
        struct path_part *next_part = part->next;
        kfree((void*) part->part);
        kfree(part);
        part = next_part;
    }

    kfree(root);
}


// public function that parts of the kernel will call to parse a path
struct path_root *pathparser_parse(const char *path, const char *current_directory_path) {
    int res = 0; // either drive number or error code
    const char *tmp_path = path; // path is copied to tmp path because several of the functuions we will call iterate the pointer. This will allow us to keep the origional pointer
    struct path_root *path_root = 0;

    if(strlen(path) > PEACHOS_MAX_PATH) {
        goto out;
    }

    if((res = pathparser_get_drive_by_path(&tmp_path)) < 0) {
        goto out;
    }

    if(!(path_root = pathparser_create_root(res))) {
        goto out;
    }

    struct path_part *first_part = pathparser_parse_path_part(NULL, &tmp_path); // NULL is passed because there is no last part yet
    if(!first_part) {
        goto out;
    }

    path_root->first = first_part;
    struct path_part *part = pathparser_parse_path_part(first_part, &tmp_path);
    while(part) {
        part = pathparser_parse_path_part(part, &tmp_path);
    }

out:
    return path_root;
}
