#include "flash_if.h"
#include "boot_image.h"
#include <string.h>

/* NOTE: This is a MOCK implementation. Real flash operations require NXP SDK drivers (FTFC) */
/* We simulate flash with a static RAM buffer representing APP region */
static uint8_t simulated_flash[APP_MAX_SIZE_BYTES];

flash_if_status_t flash_if_init(void)
{
    /* In real code: initialize flash clocking, etc. */
    return FLASH_IF_OK;
}

flash_if_status_t flash_if_erase_region(uint32_t address, uint32_t size_bytes)
{
    if (address < APP_FLASH_BASE) return FLASH_IF_E_RANGE;
    if ((address + size_bytes) > (APP_FLASH_BASE + APP_MAX_SIZE_BYTES)) return FLASH_IF_E_RANGE;
    uint32_t offset = address - APP_FLASH_BASE;
    if (offset + size_bytes > sizeof(simulated_flash)) return FLASH_IF_E_RANGE;
    memset(&simulated_flash[offset], 0xFF, size_bytes);
    return FLASH_IF_OK;
}

flash_if_status_t flash_if_program_aligned(uint32_t address, const uint8_t *data, uint32_t length)
{
    if (!data) return FLASH_IF_E_HW;
    if (address < APP_FLASH_BASE) return FLASH_IF_E_RANGE;
    if ((address + length) > (APP_FLASH_BASE + APP_MAX_SIZE_BYTES)) return FLASH_IF_E_RANGE;
    uint32_t offset = address - APP_FLASH_BASE;
    memcpy(&simulated_flash[offset], data, length);
    return FLASH_IF_OK;
}

flash_if_status_t flash_if_verify(uint32_t address, const uint8_t *data, uint32_t length)
{
    if (!data) return FLASH_IF_E_HW;
    if (address < APP_FLASH_BASE) return FLASH_IF_E_RANGE;
    if ((address + length) > (APP_FLASH_BASE + APP_MAX_SIZE_BYTES)) return FLASH_IF_E_RANGE;
    uint32_t offset = address - APP_FLASH_BASE;
    if (memcmp(&simulated_flash[offset], data, length) == 0) return FLASH_IF_OK;
    return FLASH_IF_E_HW;
}
