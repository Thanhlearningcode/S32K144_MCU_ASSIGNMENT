#ifndef BOOT_MANAGER_H
#define BOOT_MANAGER_H

#include <stdint.h>

typedef enum {
    BOOT_STAGE_INIT = 0,
    BOOT_STAGE_CHECK_EXISTING,
    BOOT_STAGE_DOWNLOAD,
    BOOT_STAGE_VALIDATE,
    BOOT_STAGE_PROGRAM,
    BOOT_STAGE_VERIFY,
    BOOT_STAGE_JUMP,
    BOOT_STAGE_ERROR,
    BOOT_STAGE_DONE
} boot_stage_t;

void boot_manager_run(void);

#endif /* BOOT_MANAGER_H */
