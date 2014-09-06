/**
 * yaffs_k9f4g08u0b.h
 *
 * yaffs k9f4g08u0b driver
 *
 * Guo Jian
 * 2014.04.27
 */
 
#include "yaffs_k9f4g08u0b.h"
#include "yaffs_nand_drv.h"

#include "nand_chip.h"
#include "yaffs_guts.h"
#include <stddef.h>

#include "nandflash_k9f4g08u0b.h"

struct yaffs_dev *yaffs_k9f4g08u0b_install_drv(const char *dev_name)
{
	struct yaffs_dev *dev;
	char *name_copy = NULL;
	struct yaffs_param *param;
	struct nand_chip *chip = NULL;

	dev = malloc(sizeof(struct yaffs_dev));
	name_copy = malloc(strlen(dev_name) + 1);
    chip = malloc(sizeof(struct nand_chip));
    
	if(!dev || !name_copy || !chip)
		goto fail;
    
    strcpy(name_copy, dev_name);

	memset(dev, 0, sizeof(*dev));

	param = &dev->param;

	param->name = name_copy;

	param->total_bytes_per_chunk = NANDFLASH_RW_PAGE_SIZE;
	param->chunks_per_block = NANDFLASH_PAGE_PER_BLOCK;
	param->n_reserved_blocks = 5;
	param->start_block = 0; // First block
	param->end_block = NANDFLASH_NUMOF_BLOCK - 1; // Last block
	param->is_yaffs2 = 1;
	param->use_nand_ecc = 1;
	param->n_caches = 10;

    chip->data_bytes_per_page = NANDFLASH_RW_PAGE_SIZE;
    chip->spare_bytes_per_page = NANDFLASH_SPARE_SIZE;
    chip->pages_per_block = NANDFLASH_PAGE_PER_BLOCK;
    chip->blocks = NANDFLASH_NUMOF_BLOCK;
    
	if(yaffs_nand_install_drv(dev, chip) != YAFFS_OK)
		goto fail;

	/* The yaffs device has been configured, install it into yaffs */
	yaffs_add_device(dev);

	return dev;

fail:
	free(dev);
	free(name_copy);
	free(chip);
	return NULL;
}
