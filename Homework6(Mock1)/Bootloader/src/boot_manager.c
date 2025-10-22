#include "boot_manager.h"
#include "boot_comm.h"
#include "boot_image.h"
#include "boot_crc.h"
#include "flash_if.h"
#include "boot_jump.h"

/* Simple synchronous state machine performing a mock download */

static boot_stage_t stage = BOOT_STAGE_INIT;

static boot_image_header_t g_hdr;
static uint8_t g_image_buffer[1024]; /* Must be >= mock image size */

static uint32_t g_received = 0;

void boot_manager_run(void)
{
    switch (stage)
    {
        case BOOT_STAGE_INIT:
            flash_if_init();
            boot_comm_init();
            stage = BOOT_STAGE_DOWNLOAD;
            break;
        case BOOT_STAGE_DOWNLOAD:
        {
            boot_comm_session_t sess; 
            if (boot_comm_open(&sess) != BOOT_COMM_OK) { stage = BOOT_STAGE_ERROR; break; }
            uint8_t tmp[64];
            uint32_t rd;
            while (boot_comm_read(&sess, tmp, sizeof(tmp), &rd) == BOOT_COMM_OK && rd > 0)
            {
                uint32_t copy_off = 0;
                while (copy_off < rd)
                {
                    if (g_received < sizeof(boot_image_header_t))
                    {
                        ((uint8_t*)&g_hdr)[g_received] = tmp[copy_off];
                    } else {
                        uint32_t img_off = g_received - sizeof(boot_image_header_t);
                        if (img_off < sizeof(g_image_buffer))
                            g_image_buffer[img_off] = tmp[copy_off];
                    }
                    ++g_received;
                    ++copy_off;
                }
            }
            boot_comm_close(&sess);
            stage = BOOT_STAGE_VALIDATE;
            break;
        }
        case BOOT_STAGE_VALIDATE:
            if (!boot_image_header_is_valid(&g_hdr)) { stage = BOOT_STAGE_ERROR; break; }
            if (g_hdr.image_size > sizeof(g_image_buffer)) { stage = BOOT_STAGE_ERROR; break; }
        {
            uint32_t crc = boot_crc32(g_image_buffer, g_hdr.image_size, 0xFFFFFFFFu);
            if (crc != g_hdr.crc32) { stage = BOOT_STAGE_ERROR; break; }
            stage = BOOT_STAGE_PROGRAM;
            break;
        }
        case BOOT_STAGE_PROGRAM:
            if (flash_if_erase_region(APP_FLASH_BASE, g_hdr.image_size) != FLASH_IF_OK) { stage = BOOT_STAGE_ERROR; break; }
            if (flash_if_program_aligned(APP_FLASH_BASE, g_image_buffer, g_hdr.image_size) != FLASH_IF_OK) { stage = BOOT_STAGE_ERROR; break; }
            stage = BOOT_STAGE_VERIFY;
            break;
        case BOOT_STAGE_VERIFY:
            if (flash_if_verify(APP_FLASH_BASE, g_image_buffer, g_hdr.image_size) != FLASH_IF_OK) { stage = BOOT_STAGE_ERROR; break; }
            stage = BOOT_STAGE_JUMP;
            break;
        case BOOT_STAGE_JUMP:
            /* In mock we won't actually jump to preserve debug environment */
            stage = BOOT_STAGE_DONE;
            break;
        case BOOT_STAGE_DONE:
            /* Idle */
            break;
        case BOOT_STAGE_ERROR:
        default:
            /* Stay here or implement retry */
            break;
    }
}
