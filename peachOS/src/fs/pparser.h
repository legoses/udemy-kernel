#ifndef PATHPARSER_H   
#define PATHPARSER_H

// splitting it up like this will allow us parse the path string once, instead of parsing it everytime we use it
struct path_root {
    int drive_no;
    struct path_part *first;
};

struct path_part {
    const char *part;
    struct path_part *next;
};

struct path_root *pathparser_parse(const char *path, const char *current_directory_path);
void pathparser_free(struct path_root *root);

#endif
