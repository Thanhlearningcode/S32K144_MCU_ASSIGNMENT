#include "boot_comm.h"
#include "boot_image.h"
#include "boot_crc.h"

/* MOCK payload: header + small dummy image (pattern bytes) */
#ifdef MOCK_DOWNLOAD_IMAGE
static uint8_t mock_payload[256]; /* Will be filled at init */
#endif

static uint8_t mock_image_body[128];
static boot_image_header_t mock_header;

static const uint8_t *g_stream = 0;
static uint32_t g_stream_len = 0;

void boot_comm_init(void)
{
    /* Prepare mock image */
    for (uint32_t i = 0; i < sizeof(mock_image_body); ++i) {
        mock_image_body[i] = (uint8_t)(i & 0xFF);
    }
    mock_header.magic = BOOT_IMAGE_MAGIC;
    mock_header.header_size = sizeof(boot_image_header_t);
    mock_header.image_size = sizeof(mock_image_body);
    mock_header.version = BOOT_IMAGE_VERSION_CURR;
    mock_header.flags = 0;
    for (int i = 0; i < 2; ++i) mock_header.reserved[i] = 0;
    mock_header.crc32 = boot_crc32(mock_image_body, sizeof(mock_image_body), 0xFFFFFFFFu);

    g_stream_len = sizeof(mock_header) + sizeof(mock_image_body);
    /* To avoid alignment issues, we provide a simple contiguous view without copying full array if not needed */
}

boot_comm_status_t boot_comm_open(boot_comm_session_t *sess)
{
    if (!sess) return BOOT_COMM_E_IO;
    sess->offset = 0;
    sess->length = g_stream_len;
    sess->buffer = (const uint8_t *)&mock_header; /* Header followed by body in memory layout assumption */
    g_stream = sess->buffer;
    return BOOT_COMM_OK;
}

boot_comm_status_t boot_comm_read(boot_comm_session_t *sess, uint8_t *dst, uint32_t wanted, uint32_t *read_out)
{
    if (!sess || !dst || !read_out) return BOOT_COMM_E_IO;
    if (sess->offset >= sess->length) { *read_out = 0; return BOOT_COMM_E_CLOSED; }

    uint32_t remaining = sess->length - sess->offset;
    uint32_t take = (wanted < remaining) ? wanted : remaining;
    for (uint32_t i = 0; i < take; ++i) {
        uint32_t idx = sess->offset + i;
        if (idx < sizeof(mock_header)) {
            /* header bytes */
            const uint8_t *hbytes = (const uint8_t *)&mock_header;
            dst[i] = hbytes[idx];
        } else {
            uint32_t body_idx = idx - sizeof(mock_header);
            dst[i] = mock_image_body[body_idx];
        }
    }
    sess->offset += take;
    *read_out = take;
    return BOOT_COMM_OK;
}

boot_comm_status_t boot_comm_close(boot_comm_session_t *sess)
{
    (void)sess;
    return BOOT_COMM_OK;
}
