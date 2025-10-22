#include "boot_jump.h"
#include "S32K144.h"

/* Jump to application by setting MSP and VTOR, then branching to reset handler */
int boot_jump_to_application(uint32_t app_base_address)
{
    if (app_base_address < APP_FLASH_BASE) return -1;

    uint32_t initial_sp = *(uint32_t*)(app_base_address + 0);
    uint32_t reset_handler = *(uint32_t*)(app_base_address + 4);
    if ((initial_sp == 0xFFFFFFFFu) || (reset_handler == 0xFFFFFFFFu)) return -2;

    /* Deinit hardware as needed (clocks, peripherals) - omitted in mock */

    /* Set VTOR */
    SCB->VTOR = app_base_address & 0xFFFFFF80UL; /* Align as per ARM requirement */

    /* Set MSP */
    __set_MSP(initial_sp);

    app_entry_t app = (app_entry_t)reset_handler;
    app(); /* Never returns */
    return 0; /* Should not reach */
}
