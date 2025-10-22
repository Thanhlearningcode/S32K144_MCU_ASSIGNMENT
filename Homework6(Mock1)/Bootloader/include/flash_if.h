/* Flash interface abstraction (stub for S32K144) */
#ifndef FLASH_IF_H
#define FLASH_IF_H

#include <stdint.h>

typedef enum {
    FLASH_IF_OK = 0,
    FLASH_IF_E_ALIGN,
    FLASH_IF_E_RANGE,
    FLASH_IF_E_HW
} flash_if_status_t;

flash_if_status_t flash_if_init(void);
flash_if_status_t flash_if_erase_region(uint32_t address, uint32_t size_bytes);
flash_if_status_t flash_if_program_aligned(uint32_t address, const uint8_t *data, uint32_t length);
flash_if_status_t flash_if_verify(uint32_t address, const uint8_t *data, uint32_t length);

#endif /* FLASH_IF_H */
