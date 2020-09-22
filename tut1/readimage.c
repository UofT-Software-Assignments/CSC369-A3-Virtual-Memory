#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ext2.h"

#define FS1
#define FS2

// Pointer to the beginning of the disk (byte 0)
static const unsigned char *disk = NULL;

static void print_blockgroup(const struct ext2_group_desc *group, int verbose)
{
	if (verbose)
	{
		printf("Block group:\n");
		printf("    block bitmap: %d\n", group->bg_block_bitmap);
		printf("    inode bitmap: %d\n", group->bg_inode_bitmap);
		printf("    inode table: %d\n", group->bg_inode_table);
		printf("    free blocks: %d\n", group->bg_free_blocks_count);
		printf("    free inodes: %d\n", group->bg_free_inodes_count);
		printf("    used_dirs: %d\n", group->bg_used_dirs_count);
	}
	else
	{
		printf("%d, %d, %d, %d, %d, %d\n",
			   group->bg_block_bitmap,
			   group->bg_inode_bitmap,
			   group->bg_inode_table,
			   group->bg_free_blocks_count,
			   group->bg_free_inodes_count,
			   group->bg_used_dirs_count);
	}
}

void print_usage()
{
	fprintf(stderr, "Usage: readimage [-tv] <image file name>\n");
	fprintf(stderr, "     -t will print the output in terse format for auto-testing\n");
	fprintf(stderr, "     -v will print the output in verbose format for easy viewing\n");
}

void print_binary(const unsigned char *ptr, int number_bytes){
		unsigned char byte;
		for(int i = 0; i < number_bytes; i++){
			byte = *(ptr + i);
			for(int j = 0; j < 8; j++){
				printf("%d", (byte & (1 << j)) >> j);
			}
			printf(" ");
		}
	}

int main(int argc, char *argv[])
{
	int option;
	int verbose = 1;
	while ((option = getopt(argc, argv, "tv")) != -1)
	{
		switch (option)
		{
		case 't':
			verbose = 0;
			break;
		case 'v':
			verbose = 1;
			break;
		default:
			print_usage();
			exit(1);
		}
	}

	if (optind >= argc)
	{
		print_usage();
		exit(1);
	}

	int fd = open(argv[optind], O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}

	// Map the disk image into memory so that we don't have to do any reads and writes
	disk = mmap(NULL, 128 * EXT2_BLOCK_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (disk == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	const struct ext2_super_block *sb = (const struct ext2_super_block *)(disk + EXT2_BLOCK_SIZE);

	if (verbose)
	{
		printf("Inodes: %d\n", sb->s_inodes_count);
		printf("Blocks: %d\n", sb->s_blocks_count);
	}
	else
	{
		printf("%d, %d, ", sb->s_inodes_count, sb->s_blocks_count);
	}

	const struct ext2_group_desc *group = (const struct ext2_group_desc *)(disk + EXT2_BLOCK_SIZE * 2);
	print_blockgroup(group, verbose);

	const unsigned char *block_bitmap = disk + EXT2_BLOCK_SIZE * (group->bg_block_bitmap);
    const unsigned char *inode_bitmap = disk + EXT2_BLOCK_SIZE * (group->bg_inode_bitmap);
    
	if(verbose)
	{
		printf("Block bitmap: ");
		print_binary(block_bitmap, (sb->s_blocks_count)/8);
		printf("\n");
		printf("Inode bitmap: ");
		print_binary(inode_bitmap, (sb->s_inodes_count)/8);
	}
	else
	{
		print_binary(block_bitmap, (sb->s_blocks_count)/8);
		printf("\n");
		print_binary(inode_bitmap, (sb->s_inodes_count)/8);
	}

	if(verbose){
		printf("\n\nInodes:");
	}

	const unsigned char *inode_table = disk + EXT2_BLOCK_SIZE * (group->bg_inode_table); 
	const struct ext2_inode *root_inode = (const struct ext2_inode *)(inode_table + sb->s_inode_size);

	void print_inode_pointers(const unsigned int *i_block, int i_blocks){
		int num_pointers = i_blocks > 24 ? 13 : (i_blocks +1)/2;
		for(int i = 0; i < num_pointers; i ++){
			printf(" %d", (int)(i_block[i]));
		}
	}

	void print_inode(const struct ext2_inode *inode, int index, int verbose){
		
		char filetype = (inode->i_mode & EXT2_S_IFREG ) == EXT2_S_IFREG ? 'f': 'd';
		if(verbose){
			printf("\n[%d] type: %c size: %d links: %d blocks: %d", 
					index, 
					filetype,
					inode->i_size,
					inode->i_links_count,
					inode->i_blocks);
			printf("\n[%d] Blocks:", index);
		}else{
			printf("\n[%d] %c, %d, %d, %d | ", 
					index, 
					filetype,
					inode->i_size,
					inode->i_links_count,
					inode->i_blocks);
		}
		print_inode_pointers(inode->i_block, inode->i_blocks);
		
		
	}
	print_inode(root_inode, 1, verbose);

	int in_use(const unsigned char *inode_bitmap, int index){
		unsigned char byte = *(inode_bitmap + index/8);
		if(byte & (1 << (index % 8))){
			return 1;
		}
		return 0;
	}

	for(int index = 11; index < sb->s_inodes_count; index++){
		if(in_use(inode_bitmap, index)){
			print_inode((const struct ext2_inode *)(inode_table + index*sb->s_inode_size), index, verbose);
		}
	}


	return 0;
}
