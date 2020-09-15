#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>
#include <assert.h>
#include "fs.h"

#define SB_NINODES  200
#define SB_SIZE 1024

/* The FS must have a plan for where it stores inodes and content blocks on the disk.
// Block 0 -> unused (It hold the boot sector)
// Block 1 -> superblock (Contains metadata about the file system)
// Block 2 -> hold inodes
// Block 2+n -> bitmap blocks (tracking which data blocks are in use)
// The block at the end of the disk hold the logging layer's log.
*/

const char* IMAGE_NOT_FOUND     = "Image not found.";
const char* ERR_INODE           = "ERROR: bad inode.";
const char* ERR_ADDR_DIR        = "ERROR: bad direct address in inode.";
const char* ERR_ADDR_INDIR      = "ERROR: bad indirect address in inode.";
const char* ERR_ROOT_DIR        = "ERROR: root directory does not exist.";
const char* ERR_DIR_FRMT        = "ERROR: directory not properly formatted.";
const char* ERR_MKD_FREE        = "ERROR: address used by inode but marked free in bitmap.";
const char* ERR_MKD_USED        = "ERROR: bitmap marks block in use but it is not in use.";
const char* ERR_ADDR_DIR_DUP    = "ERROR: direct address used more than once.";
const char* ERR_ADDR_INDIR_DUP  = "ERROR: indirect address used more than once.";
const char* ERR_ITBL_DIR_USED   = "ERROR: inode marked use but not found in a directory.";
const char* ERR_ITBL_DIR_FREE   = "ERROR: inode referred to in directory but marked free.";
const char* ERR_LINK_FILE_REF   = "ERROR: bad reference count for file.";
const char* ERR_LINK_DIR_REF    = "ERROR: directory appears more than once in file system.";
const char* ERR_NOMATCH_PARENT  = "ERROR: parent directory mismatch.";

/*
Your checker should read through the file system image and determine the consistency of
a number of things, including the following. When a problem is detected, print the error
message (shown below) to standard error and exit immediately with exit code 1
(i.e., call exit(1)).
* 
*/

//---------------------------------------------
// GLOBAL VARIABLES
//---------------------------------------------
superblock_t *sb;       // Superblock
dinode_t *dip;          // Single inode
void *image;            // Image read from file
char *bitmap;           // Bitmap buffer
int data_block;         // Data block
int found_root = 0;     // Set to 1 when root directory is found

// Data structures to track usage
int dir_inodes[SB_NINODES];              // keep track of inodes that are directories
int ref_to_parent[SB_NINODES];           // Reference from dir_entry to parent directory
int ref_back[SB_NINODES];                // Reference back to dir_entry
int num_refs[SB_NINODES];                // Number of references to each of the 
int num_refs_real[SB_NINODES];           // nlinks if inode used
int data_bitmap[SB_SIZE];

void error(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

/*
Read image from file and set up the superblock, bitmaps and data buffers.
*/
// Strucuture: unused block | superblock | inode table | bitmaps | data blocks
int set_up_inodeBlock(char *filename) {
    // open file 
    int fd = open(filename, O_RDONLY);
    if(fd < 0){
        error(IMAGE_NOT_FOUND);
    }
    // set status
    int rc;
    struct stat sbuf;
    rc = fstat(fd, &sbuf);
    assert(rc == 0);
    // set content of file system image for mapping file
    image = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    //printf("image %s\n", image);
    assert(image != MAP_FAILED);
    // Superblock
    sb = (superblock_t *)(image + BSIZE);
    // Bitmap
    unsigned int bitmap_block = sb->ninodes / IPB + 3;
    bitmap = (char *) (image + (bitmap_block * BSIZE));
    // Data
    unsigned int num_bit_blocks = sb->size / (BSIZE * 8) + 1;
    data_block = bitmap_block + num_bit_blocks;

    printf("ninodes %d\n", sb->ninodes);
    printf("size %d\n", sb->size);
    printf("data_block %d\n", data_block);

    return 0;
}

/*
Set up the data bitmap based on the bit value for the data
*/
int setup_data_bitmap() {
    
    for(int i = data_block; i < sb->nblocks; ++i) {
        if((bitmap[i/8] & (0x1 << (i%8))) > 0) {
            data_bitmap[i] = 1;
        } else {
            data_bitmap[i] = 0;
        }
    }
    return 0;
}
/*
Process a single Inode
*/
int process_inode() {
    // printf("here\n");
    // Check Each inode is either unallocated or one of the valid types (T_FILE, T_DIR, T_DEV).
    if(dip->type != T_FILE && dip->type != T_DIR && dip->type != T_DEV) {
        error(ERR_INODE);
    }
    // Don't process unallocated inodes
    if(dip->type == 0) {
        return 1;
    }
    int curr_entry_inum, used_block, bitmap_value, i, j;
    dirent_t *dir_entry;
    int num_dir_entries = BSIZE / sizeof(struct dirent);
    /******************************** DIRECT BLOCKS *****************************/
    for(i = 0; i <= NDIRECT; ++i) {
        // For in-use inodes, each address that is used by inode is valid (points to a valid datablock address within the image).
        // If the direct block is used and is invalid, error.
        if(dip->addrs[i] != 0 && (dip->addrs[i] < data_block || dip->addrs[i] > 1023)) {
            error(ERR_ADDR_DIR);
        }
        used_block = dip->addrs[i];
        bitmap_value = data_bitmap[used_block];
        printf("bitmap_value %d\n", bitmap_value);

        if(used_block > 0) {
            // increment the next element of the index array
            data_bitmap[used_block]++;
        }
        // For in-use inodes, each direct address in use is only used once. 
        if(data_bitmap[used_block] > 2) {
            error(ERR_ADDR_DIR_DUP);
        }
        // For in-use inodes, each address in use is also marked in use in the bitmap. If not, error
        if(bitmap_value == 0) {
            error(ERR_MKD_FREE);
        }
    }
    /********** Directory specific checks ************/
    if(dip->type == 1) {
        // read the first data block of directory to check for . and ..
        dir_entry = (dirent_t *)(image + (used_block * BSIZE));
        j = 0;
        if(i == 0) {
            // Each directory contains . and .. entries, and the . entry points to the directory itself. If not,error.
            if(strcmp(dir_entry->name, ".") || strcmp((dir_entry + 1)->name, "..")) {
                error(ERR_DIR_FRMT);
            }
            // This inode is DIRECTORY 
            dir_inodes[dir_entry->inum] = 1;
            curr_entry_inum = dir_entry->inum;
            ref_to_parent[curr_entry_inum] = (dir_entry + 1)->inum;

            // Each directory contains . and .. entries, and the . entry points to the directory itself.
            if(dir_entry->inum == 1 && (dir_entry + 1)->inum == 1 && found_root == 0) {
                found_root = 1;
            }
            // Skip . and .. and continue processing
            j += 2;
            dir_entry += 2;
            // this a link
            if(i < NDIRECT) {
                for(; j < num_dir_entries; ++j) {
                    if(dir_entry->inum != 0) {
                        num_refs[dir_entry->inum]++;
                        ref_back[dir_entry->inum] = curr_entry_inum;
                    }
                    dir_entry++;
                }
            }

        }
    }
    unsigned int *indirect_block;
    int num_indi_blocks = BSIZE / sizeof(unsigned int);
    /******************************** INDIRECT BLOCKS *****************************/
    used_block = dip->addrs[NDIRECT];
    indirect_block = (unsigned int *)(image + (used_block * BSIZE));
    for(int i = 0; i < num_indi_blocks; ++i) {
        // For in-use inodes, each address that is used by inode is valid (points to a valid datablock address within the image).
        // if the indirect block is in use and is invalid, error.
        if(*indirect_block != 0 && (*indirect_block < data_block || *indirect_block > 1023)) {
            error(ERR_ADDR_INDIR);
        }
        // For in-use inodes, each address in use is also marked in use in the bitmap. If not,error
        if(*indirect_block > 0 && data_bitmap[*indirect_block] == 0) {
            error(ERR_ADDR_INDIR);
        }
        if(*indirect_block > 0) {
            // Go to next value of indirect block
            data_bitmap[*indirect_block]++;
        }
        // For in-use inodes, each indirect address in use is only used once. If not,error
        if(data_bitmap[*indirect_block] > 2) {
            error(ERR_ADDR_INDIR_DUP);
        }
        /********** Directory specific checks ************/
        // this is link
        if(dip->type == 1) {
            dir_entry = (dirent_t *)(image + (*indirect_block * BSIZE));
            for(j = 0; j < num_dir_entries; ++j) {
                if(dir_entry->inum != 0) {
                    num_refs[dir_entry->inum]++;
                    ref_back[dir_entry->inum] = curr_entry_inum;
                }
                dir_entry++;
            }
        }
        indirect_block++;
    }
    return 0;
}


/*
Process the inode table
*/
int process_inode_table() {
    int inode_number = 0;                        // Inode index
    int num_inode_blocks = sb->ninodes / IPB;    // Number of inodes blocks
    setup_data_bitmap();
    // Set up links for root directory inode
    ref_back[ROOTINO] = 1;
    num_refs[ROOTINO] = 1;

    // Process all of the inode blocks
    for(int i = 2; i < num_inode_blocks; ++i){
        dip = (dinode_t *)(image + i * BSIZE);
        // Process each inode of this block
        for(int j = 0; j < IPB; ++j){
            process_inode();
            // Mark uses for the inode
            if(dip->type != 0) {
                if(dip->nlink > 1) {
                    num_refs_real[inode_number] = dip->nlink;
                } else {
                num_refs_real[inode_number] = 1;
                }
            }
            // Check root directory exists and == 1
            if(inode_number == 1 && found_root == 0) {
                error(ERR_ROOT_DIR);
            }
            inode_number++;
            dip++;
        }
    }
    return 0;
}

/*
For blocks marked in-use in bitmap, the block should actually be in-use in an inode or indirect block somewhere. If not,error
*/
int bitmap_checks() {
    for(int i = data_block; i < data_block + sb->nblocks + 1; ++i) {
        // value should not be 1 if it is in use
        if(data_bitmap[i] == 1){
            error(ERR_MKD_USED);
        }
    }
    return 0;
}

/*
Checks after processing the inode block
*/
int post_processing_checks() {
    // Check markings bitmap
    bitmap_checks();
    /********************* Inode usage count checks **********************/
    for(int i = 0; i < sb->ninodes; ++i) {
        /************************ Directory specific checks *********************/
        if(dir_inodes[i] == 1) {
            // No extra links allowed for directories (each directory only appears in one other directory). If not,error.
            if(num_refs[i] > 1 || num_refs_real[i > 1]) {
                error(ERR_LINK_DIR_REF);
            }
            // Check that each .. dir_entry in directory refers to the proper parent inode 
            // and parent inode points back to it
            if(ref_to_parent[i] != ref_back[i]) {
                error(ERR_NOMATCH_PARENT);
            }
        }
            /************************ REFERENCE COUNT CHECKS *************************/
            // For all inodes marked in use, each must be referred to in at least one directory. If not,error.
            if(num_refs[i] == 0 && num_refs_real[i] > 0) {
                error(ERR_ITBL_DIR_USED);
            }
            // For each inode number that is referred to in a valid directory, it is actually marked in use. If not,error.
            else if(num_refs[i] > 0 && num_refs_real[i] == 0){
                error(ERR_ITBL_DIR_FREE);    
            }

            /*************************** FILE SPECIFIC CHECKS **************************/
            // Reference counts (number of links) for regular files match the number of times file is referred to in directories
            // (i.e., hard links work correctly). If not,error.
            if(dir_inodes[i] == 0 && num_refs[i] != num_refs_real[i]) {
                error(ERR_LINK_FILE_REF);
            }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    // check input
    if(argc != 2) {
        error("Usage: ./fsck <file_system_image>\n");
    } 
    // Call method to process de inode block
    set_up_inodeBlock(argv[1]);
    // printf("this is IPB %lu\n", (BSIZE / sizeof(struct dinode)));
    process_inode_table();
    post_processing_checks();
    return 0;
}