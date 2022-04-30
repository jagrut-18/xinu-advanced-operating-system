#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// #ifdef FS
#include <fs.h>

fsystem_t fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0 // Superblock
#define BM_BLK 1 // Bitmapblock

#define NUM_FD 16

filetable_t oft[NUM_FD]; // open file table
#define isbadfd(fd) (fd < 0 || fd >= NUM_FD || oft[fd].in.id == EMPTY)

#define INODES_PER_BLOCK (fsd.blocksz / sizeof(inode_t))
#define NUM_INODE_BLOCKS (((fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

/**
 * Helper functions
 */
int _fs_fileblock_to_diskblock(int dev, int fd, int fileblock)
{
    int diskblock;

    if (fileblock >= INODEDIRECTBLOCKS)
    {
        errormsg("No indirect block support! (%d >= %d)\n", fileblock, INODEBLOCKS - 2);
        return SYSERR;
    }

    // Get the logical block address
    diskblock = oft[fd].in.blocks[fileblock];

    return diskblock;
}

/**
 * Filesystem functions
 */
int _fs_get_inode_by_num(int dev, int inode_number, inode_t *out)
{
    int bl, inn;
    int inode_off;

    if (dev != dev0)
    {
        errormsg("Unsupported device: %d\n", dev);
        return SYSERR;
    }
    if (inode_number > fsd.ninodes)
    {
        errormsg("inode %d out of range (> %s)\n", inode_number, fsd.ninodes);
        return SYSERR;
    }

    bl = inode_number / INODES_PER_BLOCK;
    inn = inode_number % INODES_PER_BLOCK;
    bl += FIRST_INODE_BLOCK;

    inode_off = inn * sizeof(inode_t);

    bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
    memcpy(out, &block_cache[inode_off], sizeof(inode_t));

    return OK;
}

int _fs_put_inode_by_num(int dev, int inode_number, inode_t *in)
{
    int bl, inn;

    if (dev != dev0)
    {
        errormsg("Unsupported device: %d\n", dev);
        return SYSERR;
    }
    if (inode_number > fsd.ninodes)
    {
        errormsg("inode %d out of range (> %d)\n", inode_number, fsd.ninodes);
        return SYSERR;
    }

    bl = inode_number / INODES_PER_BLOCK;
    inn = inode_number % INODES_PER_BLOCK;
    bl += FIRST_INODE_BLOCK;

    bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
    memcpy(&block_cache[(inn * sizeof(inode_t))], in, sizeof(inode_t));
    bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

    return OK;
}

int fs_mkfs(int dev, int num_inodes)
{
    int i;

    if (dev == dev0)
    {
        fsd.nblocks = dev0_numblocks;
        fsd.blocksz = dev0_blocksize;
    }
    else
    {
        errormsg("Unsupported device: %d\n", dev);
        return SYSERR;
    }

    if (num_inodes < 1)
    {
        fsd.ninodes = DEFAULT_NUM_INODES;
    }
    else
    {
        fsd.ninodes = num_inodes;
    }

    i = fsd.nblocks;
    while ((i % 8) != 0)
    {
        i++;
    }
    fsd.freemaskbytes = i / 8;

    if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR)
    {
        errormsg("fs_mkfs memget failed\n");
        return SYSERR;
    }

    /* zero the free mask */
    for (i = 0; i < fsd.freemaskbytes; i++)
    {
        fsd.freemask[i] = '\0';
    }

    fsd.inodes_used = 0;

    /* write the fsystem block to SB_BLK, mark block used */
    fs_setmaskbit(SB_BLK);
    bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(fsystem_t));

    /* write the free block bitmask in BM_BLK, mark block used */
    fs_setmaskbit(BM_BLK);
    bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

    // Initialize all inode IDs to EMPTY
    inode_t tmp_in;
    for (i = 0; i < fsd.ninodes; i++)
    {
        _fs_get_inode_by_num(dev0, i, &tmp_in);
        tmp_in.id = EMPTY;
        _fs_put_inode_by_num(dev0, i, &tmp_in);
    }
    fsd.root_dir.numentries = 0;
    for (i = 0; i < DIRECTORY_SIZE; i++)
    {
        fsd.root_dir.entry[i].inode_num = EMPTY;
        memset(fsd.root_dir.entry[i].name, 0, FILENAMELEN);
    }

    for (i = 0; i < NUM_FD; i++)
    {
        oft[i].state = 0;
        oft[i].fileptr = 0;
        oft[i].de = NULL;
        oft[i].in.id = EMPTY;
        oft[i].in.type = 0;
        oft[i].in.nlink = 0;
        oft[i].in.device = 0;
        oft[i].in.size = 0;
        memset(oft[i].in.blocks, 0, sizeof(oft[i].in.blocks));
        oft[i].flag = 0;
    }

    return OK;
}

int fs_freefs(int dev)
{
    if (freemem(fsd.freemask, fsd.freemaskbytes) == SYSERR)
    {
        return SYSERR;
    }

    return OK;
}

/**
 * Debugging functions
 */
void fs_print_oft(void)
{
    int i;

    printf("\n\033[35moft[]\033[39m\n");
    printf("%3s  %5s  %7s  %8s  %6s  %5s  %4s  %s\n", "Num", "state", "fileptr", "de", "de.num", "in.id", "flag", "de.name");
    for (i = 0; i < NUM_FD; i++)
    {
        if (oft[i].de != NULL)
            printf("%3d  %5d  %7d  %8d  %6d  %5d  %4d  %s\n", i, oft[i].state, oft[i].fileptr, oft[i].de, oft[i].de->inode_num, oft[i].in.id, oft[i].flag, oft[i].de->name);
    }

    printf("\n\033[35mfsd.root_dir.entry[] (numentries: %d)\033[39m\n", fsd.root_dir.numentries);
    printf("%3s  %3s  %s\n", "ID", "id", "filename");
    for (i = 0; i < DIRECTORY_SIZE; i++)
    {
        if (fsd.root_dir.entry[i].inode_num != EMPTY)
            printf("%3d  %3d  %s\n", i, fsd.root_dir.entry[i].inode_num, fsd.root_dir.entry[i].name);
    }
    printf("\n");
}

void fs_print_inode(int fd)
{
    int i;

    printf("\n\033[35mInode FS=%d\033[39m\n", fd);
    printf("Name:    %s\n", oft[fd].de->name);
    printf("State:   %d\n", oft[fd].state);
    printf("Flag:    %d\n", oft[fd].flag);
    printf("Fileptr: %d\n", oft[fd].fileptr);
    printf("Type:    %d\n", oft[fd].in.type);
    printf("nlink:   %d\n", oft[fd].in.nlink);
    printf("device:  %d\n", oft[fd].in.device);
    printf("size:    %d\n", oft[fd].in.size);
    printf("blocks: ");
    for (i = 0; i < INODEBLOCKS; i++)
    {
        printf(" %d", oft[fd].in.blocks[i]);
    }
    printf("\n");
    return;
}

void fs_print_fsd(void)
{
    int i;

    printf("\033[35mfsystem_t fsd\033[39m\n");
    printf("fsd.nblocks:       %d\n", fsd.nblocks);
    printf("fsd.blocksz:       %d\n", fsd.blocksz);
    printf("fsd.ninodes:       %d\n", fsd.ninodes);
    printf("fsd.inodes_used:   %d\n", fsd.inodes_used);
    printf("fsd.freemaskbytes  %d\n", fsd.freemaskbytes);
    printf("sizeof(inode_t):   %d\n", sizeof(inode_t));
    printf("INODES_PER_BLOCK:  %d\n", INODES_PER_BLOCK);
    printf("NUM_INODE_BLOCKS:  %d\n", NUM_INODE_BLOCKS);

    inode_t tmp_in;
    printf("\n\033[35mBlocks\033[39m\n");
    printf("%3s  %3s  %4s  %4s  %3s  %4s\n", "Num", "id", "type", "nlnk", "dev", "size");
    for (i = 0; i < NUM_FD; i++)
    {
        _fs_get_inode_by_num(dev0, i, &tmp_in);
        if (tmp_in.id != EMPTY)
            printf("%3d  %3d  %4d  %4d  %3d  %4d\n", i, tmp_in.id, tmp_in.type, tmp_in.nlink, tmp_in.device, tmp_in.size);
    }
    for (i = NUM_FD; i < fsd.ninodes; i++)
    {
        _fs_get_inode_by_num(dev0, i, &tmp_in);
        if (tmp_in.id != EMPTY)
        {
            printf("%3d:", i);
            int j;
            for (j = 0; j < 64; j++)
            {
                printf(" %3d", *(((char *)&tmp_in) + j));
            }
            printf("\n");
        }
    }
    printf("\n");
}

void fs_print_dir(void)
{
    int i;

    printf("%22s  %9s  %s\n", "DirectoryEntry", "inode_num", "name");
    for (i = 0; i < DIRECTORY_SIZE; i++)
    {
        printf("fsd.root_dir.entry[%2d]  %9d  %s\n", i, fsd.root_dir.entry[i].inode_num, fsd.root_dir.entry[i].name);
    }
}

int fs_setmaskbit(int b)
{
    int mbyte, mbit;
    mbyte = b / 8;
    mbit = b % 8;

    fsd.freemask[mbyte] |= (0x80 >> mbit);
    return OK;
}

int fs_getmaskbit(int b)
{
    int mbyte, mbit;
    mbyte = b / 8;
    mbit = b % 8;

    return (((fsd.freemask[mbyte] << mbit) & 0x80) >> 7);
}

int fs_clearmaskbit(int b)
{
    int mbyte, mbit, invb;
    mbyte = b / 8;
    mbit = b % 8;

    invb = ~(0x80 >> mbit);
    invb &= 0xFF;

    fsd.freemask[mbyte] &= invb;
    return OK;
}

/**
 * This is maybe a little overcomplicated since the lowest-numbered
 * block is indicated in the high-order bit.  Shift the byte by j
 * positions to make the match in bit7 (the 8th bit) and then shift
 * that value 7 times to the low-order bit to print.  Yes, it could be
 * the other way...
 */
void fs_printfreemask(void)
{ // print block bitmask
    int i, j;

    for (i = 0; i < fsd.freemaskbytes; i++)
    {
        for (j = 0; j < 8; j++)
        {
            printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
        }
        printf(" ");
        if ((i % 8) == 7)
        {
            printf("\n");
        }
    }
    printf("\n");
}

void fs_print_dirs(directory_t *working_dir, int level) {

  int i, j;

  if (working_dir == NULL) {
    working_dir = &(fsd.root_dir);
    level = 0;
    printf("\nroot\n");
  }

  inode_t _in;
  directory_t cache;

  for (i = 0; i < DIRECTORY_SIZE; i++) {
    if (working_dir->entry[i].inode_num != EMPTY) {

      _fs_get_inode_by_num(dev0, working_dir->entry[i].inode_num, &_in);

      for (j = 0; j <= level; j++) printf("\033[38;5;238m:\033[39m ");
      if (_in.type == INODE_TYPE_FILE) {
        printf("f %s\n", working_dir->entry[i].name);
      } else {
        printf("d %s\n", working_dir->entry[i].name);
        bs_bread(dev0, _in.blocks[0], 0, &cache, sizeof(directory_t));
        if (cache.numentries > 0) fs_print_dirs(&cache, level + 1);
      }
    }
  }
}

/*
 * parse_path
 * Parse the directory path given by @input
 * Output every intermediate directory as it parses
 * Writes the start of the last level to @offset
 *
 * @param    input    input path string
 * @param    offset   pointer into the input string (last level)
 */
void parse_path(char *input, char **offset) {

  /*  xxxxx/xxxxx/xxxx0
   *  ^
   *  a,b
   *
   *  #####/xxxxx/xxxx0
   *  ^    ^
   *  a    b
   *
   *  xxxxx/#####/xxxx0
   *        ^    ^
   *        a    b
   *
   *  xxxxx/xxxxx/####0
   *              ^   ^
   *              a   b
   */

  char *a, *b;
  char tmp[FILENAMELEN];

  if (input == NULL) return;

  *offset = input;

  for (a = b = input; *b != '\0'; b++) {

    if (*b == '/') {

      memset(tmp, 0, sizeof(tmp));
      memcpy(tmp, a, b-a);

      printf("> %s\n", tmp);

      a = b + 1;
    }
  }

  if (a < b) *offset = a;
}

/*
 * Test the parse_path function
 */
int fstest_parsing_directories() {

  char filename[256];
  char *offset = NULL;

  printf("Parsing: [%s]\n", NULL);
  parse_path(NULL, &offset);
  printf("Returned: [%s] [%s]\n", NULL, offset);

  sprintf(filename, "");
  printf("\nParsing: [%s]\n", filename);
  parse_path(filename, &offset);
  printf("Returned: [%s] [%s]\n", filename, offset);

  sprintf(filename, "filename");
  printf("\nParsing: [%s]\n", filename);
  parse_path(filename, &offset);
  printf("Returned: [%s] [%s] (changed: %d)\n", filename, offset, offset - filename ? 1 : 0);

  sprintf(filename, "dirname/filename");
  printf("\nParsing: [%s]\n", filename);
  parse_path(filename, &offset);
  printf("Returned: [%s] [%s] (changed: %d)\n", filename, offset, offset - filename ? 1 : 0);

  sprintf(filename, "dirname/subdir/filename");
  printf("\nParsing: [%s]\n", filename);
  parse_path(filename, &offset);
  printf("Returned: [%s] [%s] (changed: %d)\n", filename, offset, offset - filename ? 1 : 0);

  sprintf(filename, "dirname/subdir/subsub/filename");
  printf("\nParsing: [%s]\n", filename);
  parse_path(filename, &offset);
  printf("Returned: [%s] [%s] (changed: %d)\n", filename, offset, offset - filename ? 1 : 0);

  return OK;
}



/**
 * TODO: implement the functions below
 */

static int get_file_index_from_directory(directory_t directory, char *filename) {
    for (int i = 0; i < DIRECTORY_SIZE; i++) {
        dirent_t sub_directory = directory.entry[i];
        if (sub_directory.inode_num == EMPTY) continue;
        if (strcmp(sub_directory.name, filename) == 0) {
            return i;
        }
    }
    return -1;
}

static dirent_t* get_file_entry_address(directory_t *working_dir, char *filename) {

  int i, j;

  if (working_dir == NULL) {
    working_dir = &(fsd.root_dir);
  }

  inode_t _in;
  directory_t cache;

  for (i = 0; i < DIRECTORY_SIZE; i++) {
    if (working_dir->entry[i].inode_num != EMPTY) {

      _fs_get_inode_by_num(dev0, working_dir->entry[i].inode_num, &_in);

        if (strcmp(working_dir->entry[i].name, filename) == 0) {
            return &(working_dir->entry[i]);
        }
    }
  }
  return NULL;
}

int fs_open(char *filename, int flags)
{
    if (flags != O_RDONLY && flags != O_WRONLY && flags != O_RDWR) return SYSERR;

    char *a, *b;
    char tmp[FILENAMELEN];
    char *full_filename = filename;

    directory_t *current_directory_pointer = &fsd.root_dir;
    inode_t directory_inode;
    int directory_depth = 0;
    for (a = b = filename; *b != '\0'; b++) {
        if (directory_depth > 16) return SYSERR;

        if (*b == '/') {
            if ((b-a) > FILENAMELEN) return SYSERR;
            memset(tmp, 0, sizeof(tmp));
            memcpy(tmp, a, b-a);

            // printf("> %s\n", tmp);
            dirent_t* file_entry_pointer = get_file_entry_address(current_directory_pointer, tmp);
            if (file_entry_pointer == NULL) return SYSERR;
            _fs_get_inode_by_num(dev0, file_entry_pointer->inode_num, &directory_inode);
            if (directory_inode.type == INODE_TYPE_DIR) {
                current_directory_pointer = &dev0_blocks[directory_inode.blocks[0] * fsd.blocksz]; // setting current working directory
            }
            else {
                return SYSERR; // if file in between of path
            }

            directory_depth++;
            a = b + 1;
        }
    }
    filename = a;

    
    dirent_t* file_entry_pointer = get_file_entry_address(current_directory_pointer, filename);
    if (file_entry_pointer == NULL) return SYSERR;

    int fd = -1;
    for (int i = 0; i < NUM_FD; i++) {
        dirent_t file_dirent = *oft[i].de;
        if (strcmp(file_dirent.name, filename) == 0 && oft[i].de == file_entry_pointer) {
            if (oft[i].state == FSTATE_OPEN) return SYSERR;
            fd = i;
            break;
        }
        if (fd == -1 && oft[i].in.id == EMPTY){
            fd = i;
        }
    }


    int inode_id = file_entry_pointer->inode_num;
    inode_t inode;
    _fs_get_inode_by_num(dev0, inode_id, &inode);
    if (inode.type == INODE_TYPE_DIR) return SYSERR;

    oft[fd].state = FSTATE_OPEN;
    oft[fd].fileptr = 0;
    oft[fd].de = file_entry_pointer;
    oft[fd].in = inode;
    oft[fd].flag = flags;

    _fs_put_inode_by_num(dev0, inode_id, &inode);

    return fd;
}

int fs_close(int fd)
{
    if (isbadfd(fd) || oft[fd].state == FSTATE_CLOSED) {
        return SYSERR;
    }
    oft[fd].state = FSTATE_CLOSED;
    oft[fd].fileptr = 0;
    return OK;
}

static int get_empty_inode_num(){
    for (int i = 0; i < fsd.ninodes; i++) {
        inode_t inode;
        _fs_get_inode_by_num(dev0, i, &inode);
        if (inode.id == EMPTY) {
            return i;
        }   
    }
    return -1;
}

static int get_empty_data_block(){
    for (int i = 18; i < fsd.nblocks; i++) {
        if (fs_getmaskbit(i) == 1) continue;
        return i;
    }
    return -1;
}

int fs_create(char *filename, int mode)
{
    if (mode != O_CREAT && mode != O_DIR) return SYSERR;

    // char *offset = NULL;
    // parse_path(filename, offset);
    // printf("Returned: [%s] [%s]\n", filename, offset);

    char *a, *b;
    char tmp[FILENAMELEN];
    char *full_filename = filename;

    directory_t *current_directory_pointer = &fsd.root_dir;
    inode_t directory_inode;
    int directory_depth = 0;
    for (a = b = filename; *b != '\0'; b++) {
        if (directory_depth > 16) return SYSERR;

        if (*b == '/') {
            if ((b-a) > FILENAMELEN) return SYSERR;
            memset(tmp, 0, sizeof(tmp));
            memcpy(tmp, a, b-a);

            // printf("> %s\n", tmp);
            dirent_t* file_entry_pointer = get_file_entry_address(current_directory_pointer, tmp);
            if (file_entry_pointer == NULL) return SYSERR;
            _fs_get_inode_by_num(dev0, file_entry_pointer->inode_num, &directory_inode);
            if (directory_inode.type == INODE_TYPE_DIR) {
                current_directory_pointer = &dev0_blocks[directory_inode.blocks[0] * fsd.blocksz]; // setting current working directory
            }
            else {
                return SYSERR; // if file in between of path
            }

            directory_depth++;
            a = b + 1;
        }
    }

    if (current_directory_pointer->numentries >= DIRECTORY_SIZE) return SYSERR;
    filename = a;

    int empty_entry_index = -1;
    for (int i = 0; i < DIRECTORY_SIZE; i++) {
        dirent_t sub_directory = current_directory_pointer->entry[i];
        if (strcmp(sub_directory.name, filename) == 0) return SYSERR;
        if (empty_entry_index == -1 && sub_directory.inode_num == EMPTY) {
            empty_entry_index = i;
        }
    }
    if (empty_entry_index == -1) return SYSERR;

    int inode_id = get_empty_inode_num();
    if (inode_id == -1) return SYSERR;

    inode_t inode;
    inode.id = inode_id;
    inode.type = mode == O_CREAT ? INODE_TYPE_FILE : INODE_TYPE_DIR;
    inode.nlink = 1;
    inode.device = dev0;
    inode.size = 0;
    memset(inode.blocks, EMPTY, sizeof(inode.blocks));

    if (mode == O_DIR){
        int empty_data_block = get_empty_data_block();
        if (empty_data_block == EMPTY) return SYSERR;
        fs_setmaskbit(empty_data_block);

        directory_t new_directory;
        new_directory.numentries = 0;
        for (int i = 0; i < DIRECTORY_SIZE; i++) {
            new_directory.entry[i].inode_num = EMPTY;
            memset(new_directory.entry[i].name, 0, FILENAMELEN);
        }
        bs_bwrite(dev0, empty_data_block, 0, &new_directory, sizeof(directory_t));
        inode.blocks[0] = empty_data_block;
    }


    _fs_put_inode_by_num(dev0, inode_id, &inode);
    fsd.inodes_used++;

    dirent_t new_dirent;
    strcpy(new_dirent.name, filename);
    new_dirent.inode_num = inode_id;

    current_directory_pointer->entry[empty_entry_index] = new_dirent;
    current_directory_pointer->numentries++;


    if (mode == O_DIR) return OK;
    return fs_open(full_filename, O_RDWR);
}

// int fs_create(char *filename, int mode)
// {
//     if (mode != O_CREAT || fsd.root_dir.numentries >= DIRECTORY_SIZE) return SYSERR;

//     int empty_entry_index = -1;
//     for (int i = 0; i < DIRECTORY_SIZE; i++) {
//         dirent_t sub_directory = fsd.root_dir.entry[i];
//         if (strcmp(sub_directory.name, filename) == 0) return SYSERR;
//         if (empty_entry_index == -1 && sub_directory.inode_num == EMPTY) {
//             empty_entry_index = i;
//         }
//     }
//     if (empty_entry_index == -1) return SYSERR;

//     int inode_id = get_empty_inode_num();
//     if (inode_id == -1) return SYSERR;

//     inode_t inode;
//     inode.id = inode_id;
//     inode.type = INODE_TYPE_FILE;
//     inode.nlink = 1;
//     inode.device = dev0;
//     inode.size = 0;
//     memset(inode.blocks, EMPTY, sizeof(inode.blocks));

//     _fs_put_inode_by_num(dev0, inode_id, &inode);
//     fsd.inodes_used++;

//     dirent_t new_dirent;
//     strcpy(new_dirent.name, filename);
//     new_dirent.inode_num = inode_id;

//     fsd.root_dir.entry[empty_entry_index] = new_dirent;
//     fsd.root_dir.numentries++;

//     return fs_open(filename, O_RDWR);
// }


int fs_seek(int fd, int offset)
{
    if (isbadfd(fd) || oft[fd].state == FSTATE_CLOSED || offset < 0 || offset > oft[fd].in.size) {
        return SYSERR;
    }
    oft[fd].fileptr = offset;
    return OK;
}


int fs_read(int fd, void *buf, int nbytes)
{
    if (isbadfd(fd) || oft[fd].state == FSTATE_CLOSED || oft[fd].flag == O_WRONLY) return SYSERR;
    if (nbytes < 0 || !buf) return SYSERR;

    int remaining_bytes = nbytes;
    void *moving_buffer = buf;
    while (remaining_bytes > 0) {

        if (oft[fd].fileptr >= oft[fd].in.size) return nbytes - remaining_bytes;
        int block_index = oft[fd].fileptr / fsd.blocksz;
        int block = oft[fd].in.blocks[block_index];
        int offset = oft[fd].fileptr % fsd.blocksz;
        bs_bread(dev0, block, offset, moving_buffer, 1);
        moving_buffer = (char *) moving_buffer + 1;
        oft[fd].fileptr += 1;
        remaining_bytes -= 1;
    }

    return nbytes;
}


int fs_write(int fd, void *buf, int nbytes)
{
    if (oft[fd].state == FSTATE_CLOSED || oft[fd].flag == O_RDONLY || isbadfd(fd)) return SYSERR;
    if (nbytes < 0 || !buf) return SYSERR;

    int capacity = oft[fd].in.size - oft[fd].fileptr;
    int extra_bytes = 0;
    if (nbytes > capacity) {
        extra_bytes = nbytes - capacity;
        nbytes = capacity;
    }

    void *moving_buffer = buf;
    for (int i = 0; i < nbytes; i++) {
        int block_index = oft[fd].fileptr / fsd.blocksz;
        int block = oft[fd].in.blocks[block_index];
        int offset = oft[fd].fileptr % fsd.blocksz;
        bs_bwrite(dev0, block, offset, moving_buffer, 1);
        moving_buffer = (char *) moving_buffer + 1;
        oft[fd].fileptr += 1;
    }
    if (extra_bytes == 0) return nbytes;

    for (int i = 0; i < extra_bytes; i++) {
        int empty_block_index = -1;
        if (oft[fd].fileptr >= oft[fd].in.size) {
            for (int j = 18; j < fsd.nblocks; j++) {
                if (fs_getmaskbit(j) == 1) continue;
                fs_setmaskbit(j);
                empty_block_index = j;
                break;
            }
            if (empty_block_index == -1) return nbytes + i;

            int found_empty_inode_block = 0;
            for (int j = 0; j < INODEDIRECTBLOCKS; j++) {
                if (oft[fd].in.blocks[j] != EMPTY) continue;
                oft[fd].in.blocks[j] = empty_block_index;
                found_empty_inode_block = 1;
                break;
            }
            if (found_empty_inode_block == 0){
                fs_clearmaskbit(empty_block_index);
                return nbytes + i;
            }
            oft[fd].in.size += fsd.blocksz;
            _fs_put_inode_by_num(dev0, oft[fd].in.id, &oft[fd].in);
        }

        int offset = oft[fd].fileptr % fsd.blocksz;
        int block_index = oft[fd].fileptr / fsd.blocksz;
        int block = oft[fd].in.blocks[block_index];
        bs_bwrite(dev0, block, offset, moving_buffer, 1);
        moving_buffer = (char *) moving_buffer + 1;
        oft[fd].fileptr += 1;
    }

    return nbytes + extra_bytes;
}


int fs_link(char *src_filename, char *dst_filename)
{
    if (!src_filename || !dst_filename) return SYSERR;
    if (fsd.root_dir.numentries >= DIRECTORY_SIZE) return SYSERR;
    dirent_t src_dirent;
    int found_src = 0;
    int empty_entry_index = -1;
    for (int i = 0; i < DIRECTORY_SIZE; i++) {
        dirent_t sub_directory = fsd.root_dir.entry[i];
        if (strcmp(sub_directory.name, dst_filename) == 0) return SYSERR;
        if (found_src != 1 && strcmp(sub_directory.name, src_filename) == 0) {
            src_dirent = sub_directory;
            found_src = 1;
        }
        if (empty_entry_index == -1 && sub_directory.inode_num == EMPTY){
            empty_entry_index = i;
        }
    }
    if (found_src == 0 || empty_entry_index == -1) return SYSERR;

    int src_inode_id = src_dirent.inode_num;
    inode_t inode;
    _fs_get_inode_by_num(dev0, src_inode_id, &inode);
    inode.nlink++;
    _fs_put_inode_by_num(dev0, src_inode_id, &inode);

    for (int i = 0; i < NUM_FD; i++) {
		if(oft[i].in.id == src_inode_id) {
			oft[i].in.nlink++;
		}
  	}

    dirent_t dst_dirent;
    strcpy(dst_dirent.name, dst_filename);
    dst_dirent.inode_num = src_inode_id;

    fsd.root_dir.entry[empty_entry_index] = dst_dirent;
    fsd.root_dir.numentries++;

    return OK;
}



int fs_unlink(char *filename)
{
    if (!filename) return SYSERR;
    int file_index = get_file_index_from_directory(fsd.root_dir, filename);
    if (file_index == -1) return SYSERR;
    dirent_t file_dirent = fsd.root_dir.entry[file_index];

    int inode_id = file_dirent.inode_num;
    inode_t inode;
    _fs_get_inode_by_num(dev0, inode_id, &inode);
    
    if (inode.nlink == 1) {
        for (int i = 0; i < INODEDIRECTBLOCKS; i++) {
            if (inode.blocks[i] == EMPTY) continue;
            fs_clearmaskbit(inode.blocks[i]);
        }
        inode.id = EMPTY;
        inode.size = 0;
        inode.nlink = 0;
        inode.type = 0;
        memset(inode.blocks, EMPTY, sizeof(inode.blocks));
        // COMMENTED THIS CODE BECAUSE FILE WON'T BE OPEN WHEN UNLINKING
        // for (int i = 0; i < NUM_FD; i++) {
        //     if (oft[i].in.id == inode_id){
        //         oft[i].in = inode;
        //         oft[i].de = NULL;
        //         oft[i].state = FSTATE_CLOSED;
        //         oft[i].fileptr = 0;
        //         oft[i].flag = 0;
        //     }
        // }
        fsd.inodes_used--;
    }
    else {
        inode.nlink--;
        for (int i = 0; i < NUM_FD; i++) {
            if (oft[i].in.id == inode_id){
                oft[i].in = inode;
            }
            // COMMENTED THIS CODE BECAUSE FILE WON'T BE OPEN WHEN UNLINKING
            // if (strcmp((*oft[i].de).name, filename) == 0) {
            //     for (int j = 0; j < INODEDIRECTBLOCKS; j++) {
            //         oft[i].in.blocks[j] = EMPTY;
            //     }
            //     oft[i].in.id = EMPTY;
            //     oft[i].in.size = 0;
            //     oft[i].in.nlink = 0;
            //     oft[i].in.type = 0;
            //     oft[i].de = NULL;
            //     oft[i].state = FSTATE_CLOSED;
            //     oft[i].fileptr = 0;
            //     oft[i].flag = 0;
            // }
        }
    }
    
    fsd.root_dir.entry[file_index].inode_num = EMPTY;
    strcpy(fsd.root_dir.entry[file_index].name, "");
    fsd.root_dir.numentries--;
    
    _fs_put_inode_by_num(dev0, inode_id, &inode);
    return OK;
}

// #endif /* FS */