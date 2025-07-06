#include "fat16.h"
#include "string/string.h"
#include "disk/disk.h"
#include "disk/streamer.h"
#include "status.h"
#include <stdint.h>


#define PEACHOS_FAT16_SIGNATURE 0x29
#define PEACHOS_FAT16_FAT_ENTRY_SIZE 0x02
#define PEACHOS_FAT16_BAD_SECTOR 0xFF7 // final fantasy 7?
#define PEACHOS_FAT16_UNUSED 0x00


// these will be used for internal representation of files and directories
typedef unsigned int FAT_ITEM_TYPE;
#define FAT_ITEM_TYPE_DIRECTORY 0
#define FAT_ITEM_TYPE_FILE 1

// fat file attribute bitmasks
// most of these are pretty self explanitory
#define FAT_FILE_READ_ONLY 0x01
#define FAT_FILE_HIDDEN 0x02
#define FAT_FILE_SYSTEM 0x04
#define FAT_FILE_VOLUME_LABEL 0x08
#define FAT_FILE_SUBDIRECTORY 0x10
#define FAT_FILE_ARCHIVED 0x20
#define FAT_FILE_DEVICE 0x40
#define FAT_FILE_RESERVED 0x80

struct fat_header_extended {
    uint8_t drive_number;
    uint8_t win_nt_bit; //only used for devices running windows nt, otherwise reserved
    uint8_t signature;
    uint8_t volume_id;
    uint8_t volume_id_string[11]; // padded with spaces
    uint8_t system_id_string[8]; // represents the fat filesystem type, padded with spaces
} __attribute__((packed));

    // refers to the information defined in boot.asm
struct fat_header {
    uint8_t short_jump_ins[3]; // contains short jump and null operation contained in boot.asm
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_copies;
    uint16_t root_dir_entries;
    uint16_t number_of_sectors;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t sectors_big;
} __attribute__((packed));


// primary fat header that will contain fat16_header and fat16_header_extended
struct fat_h {
    struct fat_header primary_header;
    union fat_h_e { // optional fat extended header 
        struct fat_header_extended extended_header;
    } shared;
};


struct fat_directory_item {
    uint8_t filename[8]; // filename and ext are "8.3" filename. This is first 8 bytes
    uint8_t ext[3]; // last 3 extension bytes of a filename
    uint8_t attrbiute;
    uint8_t reserved;
    uint8_t creation_time_tenths_of_a_sec;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access;
    // high 16 bits of the address of the first cluster of this directory item. if this is a subdirectory, this is the  high 16 bits of where the directories are stored. If this is a file, it is the first cluster containing the file contends
    uint16_t high_16_bits_first_cluster; 
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint16_t low_16_bits_first_cluster;
    uint32_t filesize;
} __attribute__((packed));


struct fat_directory {
    struct fat_directory_item *item; 
    int total; // total number of items in item array
    int sector_pos; // first sector the fat directory data is stored at
    int ending_sector_pos; // last sector fat directory is stored
};


struct fat_item {
    union { // unions share the same memory, which allows use of the same structure for multiple possible data types, while only allocating the required amount for one
        struct fat_directory_item *item;
        struct fat_directory *directory;
    };

    FAT_ITEM_TYPE type; // equal to either FAT_ITEM_TYPE_DIRECTORY or FAT_ITEM_TYPE_FILE
};


// this will be used when fopen is called
struct fat_item_descriptor {
    struct fat_item *item; // stored the item being opened
    uint32_t pos; // position we are seeking in the file
};


struct fat_private {
    struct fat_h header;
    struct fat_directory root_directory;

    // used to stream data clusters
    struct disk_stream *cluster_read_stream;

    //used to stream the file allocation table
    struct disk_stream *fat_read_stream;

    // used in situations where we atream the directory
    struct disk_stream *directory_stream;
}


// define before we create  struct
void *fat16_open(struct disk *disk, struct path_part *path, FILE_MODE mode);
int fat16_resolve(struct disk *disk);

struct filesystem fat16_fs = {
    .resolve = fat16_resolve,
    .open = fat16_open
};

struct filesystem *fat16_init() {
    strcpy(fat16_fs.name, "FAT16");
    return &fat16_fs;
}


// params are taken from FS_RESOLVE_FUNCTION pointer defined in file.h
int fat16_resolve(struct disk *disk) {
    return 0;
}


// these params are taken from FS_OPEN_FUNCTION pointer in file.h
void *fat16_open(struct disk *disk, struct path_part *path, FILE_MODE mode) {
    return 0;
}
