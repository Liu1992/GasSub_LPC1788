/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * This is an interface module for handling NAND in yaffs2 mode.
 */

/* This code calls a driver that accesses "generic" NAND. In the simulator
 * this is direceted at a file-backed NAND simulator. The NAND access functions
 * should also work with real NAND.
 *
 * This driver is designed for use in yaffs2 mode with a 2k page size and
 * 64 bytes of spare.
 *
 * The spare ares is used as follows:
 * offset 0: 2 bytes bad block marker.
 * offset 2: 8x3 bytes of ECC over the data.
 * offset 26: rest available to store Yaffs tags etc.
 */

#include "yaffs_nand_drv.h"
#include "yportenv.h"
#include "yaffs_trace.h"

#include "nand_store.h"
#include "yaffs_guts.h"
#include "nandflash_k9f4g08u0b.h"
#include "yaffs_ecc.h"

struct nand_context {
	struct nand_chip *chip;
	u8 *buffer;
};

static struct nand_chip *dev_to_chip(struct yaffs_dev *dev)
{
	struct nand_context *ctxt =
		(struct nand_context *)(dev->driver_context);
	return ctxt->chip;
}

static u8 *dev_to_buffer(struct yaffs_dev *dev)
{
	struct nand_context *ctxt =
		(struct nand_context *)(dev->driver_context);
	return ctxt->buffer;
}

static int yaffs_nand_drv_WriteChunk(struct yaffs_dev *dev, int nand_chunk,
				   const u8 *data, int data_len,
				   const u8 *oob, int oob_len)
{
	struct nand_chip *chip = dev_to_chip(dev);
	u8 *buffer = dev_to_buffer(dev);
	u8 *e;
	int i;

	if(!data || !oob)
		return YAFFS_FAIL;


	/* Calc ECC and marshall the oob bytes into the buffer */

	memset(buffer, 0xff, chip->spare_bytes_per_page);

	for(i = 0, e = buffer + 2; i < chip->data_bytes_per_page; i+=256, e+=3)
		yaffs_ecc_calc(data + i, e);

	memcpy(buffer + 26, oob, oob_len);

	/* Set up and execute transfer */

	return NandFlash_PageProgramWithSpare(nand_chunk, data, data_len, buffer) ? YAFFS_OK : YAFFS_FAIL;
}

static int yaffs_nand_drv_ReadChunk(struct yaffs_dev *dev, int nand_chunk,
				   u8 *data, int data_len,
				   u8 *oob, int oob_len,
				   enum yaffs_ecc_result *ecc_result_out)
{
	struct nand_chip *chip = dev_to_chip(dev);
	u8 *buffer = dev_to_buffer(dev);
	int ret;
	enum yaffs_ecc_result ecc_result;
	int i;
	u8 *e;
	u8 read_ecc[3];

	ret = NandFlash_PageReadWithSpare(nand_chunk, data, buffer);

	if(ret < 0) {
		if (ecc_result_out)
			*ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;
		return YAFFS_FAIL;
	}

	/* Do ECC and marshalling */
	if(oob)
		memcpy(oob, buffer + 26, oob_len);

	ecc_result = YAFFS_ECC_RESULT_NO_ERROR;

	if(data) {
		for(i = 0, e = buffer + 2; i < chip->data_bytes_per_page; i+=256, e+=3) {
			yaffs_ecc_calc(data + i, read_ecc);
			ret = yaffs_ecc_correct(data + i, e, read_ecc);
			if (ret < 0)
				ecc_result = YAFFS_ECC_RESULT_UNFIXED;
			else if( ret > 0 && ecc_result == YAFFS_ECC_RESULT_NO_ERROR)
				ecc_result = YAFFS_ECC_RESULT_FIXED;
		}
	}

	if (ecc_result_out)
		*ecc_result_out = ecc_result;

	return YAFFS_OK;
}

static int yaffs_nand_drv_EraseBlock(struct yaffs_dev *dev, int block_no)
{
	return NandFlash_BlockErase(block_no) ? YAFFS_OK : YAFFS_FAIL;
}

static int yaffs_nand_drv_MarkBad(struct yaffs_dev *dev, int block_no)
{
	struct nand_chip *chip = dev_to_chip(dev);
	u8 *buffer = dev_to_buffer(dev);
	int nand_chunk = block_no * chip->pages_per_block;

    /* in case wrongly marked */
    BUG();
    
	memset(buffer, 0xff, chip->spare_bytes_per_page);
	buffer[0] = 'Y';
	buffer[1] = 'Y';

	return NandFlash_PageReadWithSpare(nand_chunk, NULL, buffer);
}

static int yaffs_nand_drv_CheckBad(struct yaffs_dev *dev, int block_no)
{
    return NandFlash_ValidBlockCheck(block_no) ? YAFFS_OK : YAFFS_FAIL;
}

static int yaffs_nand_drv_Initialise(struct yaffs_dev *dev)
{
	struct nand_chip *chip = dev_to_chip(dev);

	(void)chip;
	return YAFFS_OK;
}

static int yaffs_nand_drv_Deinitialise(struct yaffs_dev *dev)
{
	struct nand_chip *chip = dev_to_chip(dev);

	(void) chip;
	return YAFFS_OK;
}


int yaffs_nand_install_drv(struct yaffs_dev *dev, struct nand_chip *chip)
{
	struct yaffs_driver *drv = &dev->drv;
	u8 *buffer = NULL;
	struct nand_context *ctxt = NULL;

	ctxt = malloc(sizeof(struct nand_context));
	buffer = malloc(chip->spare_bytes_per_page);

	if(!buffer || !ctxt)
		goto fail;

	drv->drv_write_chunk_fn = yaffs_nand_drv_WriteChunk;
	drv->drv_read_chunk_fn = yaffs_nand_drv_ReadChunk;
	drv->drv_erase_fn = yaffs_nand_drv_EraseBlock;
	drv->drv_mark_bad_fn = yaffs_nand_drv_MarkBad;
	drv->drv_check_bad_fn = yaffs_nand_drv_CheckBad;
	drv->drv_initialise_fn = yaffs_nand_drv_Initialise;
	drv->drv_deinitialise_fn = yaffs_nand_drv_Deinitialise;

	ctxt->chip = chip;
	ctxt->buffer = buffer;
	dev->driver_context = (void *) ctxt;
	return YAFFS_OK;

fail:
	free(ctxt);
	free(buffer);
	return YAFFS_FAIL;
}
