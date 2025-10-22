#ifndef BOOT_COMM_H
#define BOOT_COMM_H

#include <stdint.h>

typedef enum {
    BOOT_COMM_OK = 0,
    BOOT_COMM_E_TIMEOUT,
    BOOT_COMM_E_IO,
    BOOT_COMM_E_CLOSED
} boot_comm_status_t;

typedef struct {
    const uint8_t *buffer; /* Pointer to current payload buffer */
    uint32_t length;       /* Total length */
    uint32_t offset;       /* Current read pointer */
} boot_comm_session_t;

void boot_comm_init(void);
/* Begin download session. For real hardware this could wait for a host command */
boot_comm_status_t boot_comm_open(boot_comm_session_t *sess);
/* Read up to 'wanted' bytes into dst, returns number actually read via *read_out */
boot_comm_status_t boot_comm_read(boot_comm_session_t *sess, uint8_t *dst, uint32_t wanted, uint32_t *read_out);
boot_comm_status_t boot_comm_close(boot_comm_session_t *sess);

#endif /* BOOT_COMM_H */
