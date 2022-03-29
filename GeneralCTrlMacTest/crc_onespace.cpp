#include "crc_onespace.h"




 OS_BOOL CRC_ONESPACE::Chk16CRC_U8(OS_U8* pu8Start, OS_U32 u32Length, OS_U16 CRC)
{
   if((pu8Start == NULL)
       ||(u32Length <= 0))
       return OS_FALSE;

   OS_U16 wCRC = 0x0000;

   while ( 0 != (u32Length--) )
       wCRC = crc16_byte(wCRC, *pu8Start++);

   if(CRC == wCRC)
       return OS_TRUE;
   else
       return OS_FALSE;
}


