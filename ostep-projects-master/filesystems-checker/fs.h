// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO 1                                           // root i-number
#define BSIZE   512                                         // block size
#define T_DIR   1                                           // Directory
#define T_FILE  2                                           // File
#define T_DEV   3                                           // Device
#define SUPERBLOCK  1                                       // Block containing superblock
#define IPB           (BSIZE / sizeof(struct dinode))       // Inodes per block.
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)       // Block containing inode i
#define BPB           (BSIZE*8)                             // Bitmap bits per block
#define BBLOCK(b, sb) (b/BPB + sb.bmapstart)                // Block of free map containing bit for block b
#define DIRSIZ 14                                           // Directory is a file containing a sequence of dirent structures.
#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)


// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  unsigned int size;         // Size of file system image (blocks)
  unsigned int nblocks;      // Number of data blocks
  unsigned int ninodes;      // Number of inodes.
  unsigned int nlog;         // Number of log blocks
  unsigned int logstart;     // Block number of first log block
  unsigned int inodestart;   // Block number of first inode block
  unsigned int bmapstart;    // Block number of first free map block
}typedef superblock_t;

// On-disk inode structure
struct dinode {
  short type;                       // File type
  short major;                      // Major device number (T_DEV only)
  short minor;                      // Minor device number (T_DEV only)
  short nlink;                      // Number of links to inode in file system
  unsigned int size;                // Size of file (bytes)
  unsigned int addrs[NDIRECT+1];   // Data block addresses
}typedef dinode_t;

// directory entry
struct dirent {
  unsigned short inum;
  char name[DIRSIZ];
}typedef dirent_t;

