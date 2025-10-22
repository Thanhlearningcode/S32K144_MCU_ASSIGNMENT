#ifndef BOOT_JUMP_H
#define BOOT_JUMP_H

#include <stdint.h>
#include "boot_image.h"

typedef void (*app_entry_t)(void);

int boot_jump_to_application(uint32_t app_base_address);

#endif /* BOOT_JUMP_H */
