/**
 * jlink_putc.c
 *
 * Enable printf with jlink
 *
 * Usage :  1. Goto Target Options -> Debug -> Settings(JLink) -> Trace
 *          2. Select Enable
 *          3. Set Core freq (120MHz for LPC1788)
 *          4. Set ITM Port values: Enable = 0x00000001, Privilege = 0x00000000
 *          5. Add this file to workspace
 *          6. Include stdio.h when using output functions like printf
 *          7. Start debug session
 *          8. Select View -> Serial Windows -> Debug(printf) Viewer
 *
 * Guo Jian
 * 2014.04.28
 */
 
#include "stdio.h"

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

/**
 * fputc
 *
 * @param   ch  - charactor
            f   - file
 * @return  ch
 *
 * @brief  put char via ITM
 */
int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
