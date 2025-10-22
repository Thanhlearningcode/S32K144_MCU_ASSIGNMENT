/* Boot Image Header definition for Mock Bootloader
 * Assumptions:
 *  - Application image starts at 0x00008000 (bootloader < 32KB)
 *  - Flash size large enough (e.g. S32K144 512KB)
 */
#ifndef BOOT_IMAGE_H
#define BOOT_IMAGE_H

#include <stdint.h>

#define BOOT_IMAGE_MAGIC        (0x424C444DU) /* 'BLDM' */
#define BOOT_IMAGE_VERSION_CURR  (0x00010000U)

/* Location assumptions (can be moved to linker script or build system) */
#define APP_FLASH_BASE           (0x00008000UL)
#define APP_MAX_SIZE_BYTES       (384 * 1024UL) /* Arbitrary safe region */

typedef struct __attribute__((packed))
{
    uint32_t magic;          /* Magic number */
    uint32_t header_size;    /* Size of this header */
    uint32_t image_size;     /* Size of image that follows header */
    uint32_t version;        /* Semantic / build version */
    uint32_t crc32;          /* CRC32 over image payload (not including header) */
    uint32_t flags;          /* Reserved for future use (encryption, etc.) */
    uint32_t reserved[2];    /* Alignment / future */
} boot_image_header_t;

static inline int boot_image_header_is_valid(const boot_image_header_t *h)
{
    if (!h) return 0;
    if (h->magic != BOOT_IMAGE_MAGIC) return 0;
    if (h->header_size != sizeof(boot_image_header_t)) return 0;
    if (h->image_size == 0 || h->image_size > APP_MAX_SIZE_BYTES) return 0;
    return 1;
}

#endif /* BOOT_IMAGE_H */
