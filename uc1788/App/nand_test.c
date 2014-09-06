#include <nandflash_k9f4g08u0b.h>
#include "norflash_sst39vf1601.h"
#include "stdlib.h"
#include "stdio.h"

/*********************************************************************
*
*             NandTest
*/

void NandTest(void)
{
    uint32_t id;
    uint32_t i, e;
    uint8_t *buf1, *buf2;
    
    buf1 = malloc(2048);
    buf2 = malloc(2112);
    
    NandFlash_Reset();
    id = NORFLASHCheckID();
    printf("norflash id : 0x%x\n", id);
    
/*
    NORFLASHErase();
*/
/*
    NORFLASHWriteWord(0, 0x1234);
    NORFLASHWriteWord(1, 0x5678);
    NORFLASHWriteWord(2, 0x90AB);
    NORFLASHWriteWord(3, 0xFEDC);
*/
    for (i = 0; i <4096; ++ i)
    {
        Bool b;
        b = NandFlash_BlockErase(i);
        if (!b)
            printf("erase failed %d\n", i);
        b = NandFlash_ValidBlockCheck(i);
        if (!b)
            printf("check failed %d\n", i);
    }
    id = NandFlash_ReadId();
    
    NandFlash_Reset();
    
    for (i = 0; i < 2048; ++ i)
        buf1[i] = i & 0xFF;
    
/*
    NandFlash_BlockErase(0);
    NandFlash_PageProgram(0, 0, buf1);
*/
    NandFlash_PageRead(0, 0, buf2);
    
    e = 0;
    for (i = 0; i < 2048; ++ i)
        if (buf1[i] != buf2[i])
            ++ e;
    printf("norflash check error : %d\n", e);
    free(buf1);
    free(buf2);
}
