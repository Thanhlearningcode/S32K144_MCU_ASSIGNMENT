#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>

typedef enum
{
    PCC_ERROR_NONE = 0,       // Không có lỗi
    PCC_ERROR_INVALID_PORT,   // Lỗi: Cổng không hợp lệ
    PCC_ERROR_CLOCK_FAILED,   // Lỗi: Không thể bật clock cho cổng
    PCC_ERROR_UNKNOWN         // Lỗi không xác định
} PCC_status;

typedef enum
{
    PORT_ERROR_NONE = 0,       // Không có lỗi
    PORT_ERROR_INVALID_PORT,   // Lỗi: Cổng không hợp lệ
    PORT_ERROR_CLOCK_FAILED,   // Lỗi: Không thể bật clock cho cổng
    PORT_ERROR_UNKNOWN         // Lỗi không xác định
} Port_status;

#endif /* SYSTEM_H_ */
