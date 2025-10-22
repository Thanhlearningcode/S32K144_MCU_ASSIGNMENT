/* Simple CRC32 implementation (Polynomial 0x04C11DB7) */
#ifndef BOOT_CRC_H
#define BOOT_CRC_H

#include <stdint.h>

uint32_t boot_crc32(const void *data, uint32_t length, uint32_t previous); /* previous = 0xFFFFFFFF for start */

#endif /* BOOT_CRC_H */
