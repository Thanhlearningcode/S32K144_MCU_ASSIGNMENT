#include "boot_crc.h"

uint32_t boot_crc32(const void *data, uint32_t length, uint32_t previous)
{
    const uint8_t *p = (const uint8_t*)data;
    uint32_t crc = previous ^ 0xFFFFFFFFu;
    while (length--)
    {
        crc ^= (uint32_t)(*p++);
        for (int i = 0; i < 8; ++i)
        {
            uint32_t mask = -(crc & 1u);
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }
    return crc ^ 0xFFFFFFFFu;
}
