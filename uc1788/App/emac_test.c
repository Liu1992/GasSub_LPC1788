#include "lpc177x_8x_emac.h"

uint8_t buf[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 
  0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 
  0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x0a, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x01};
  
void EMACTest()
{
  EMAC_PACKETBUF_Type pack;
  
  pack.pbDataBuf = (uint32_t *)buf;
  pack.ulDataLen = sizeof(buf);
  
  EMAC_WritePacketBuffer(&pack);
}
