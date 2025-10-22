# Mock Bootloader for S32K144

## Overview

This is a simplified mock implementation of a bootloader architecture for the NXP S32K144 MCU. It does **not** perform real flash programming; instead it simulates the application flash region in RAM to demonstrate module interactions:

Modules:
- boot_manager: Orchestrates the state machine (download -> validate -> program -> verify -> jump)
- boot_comm: Mock communication layer providing a synthetic image stream
- boot_image: Defines image header format and validation helpers
- boot_crc: CRC32 implementation for image integrity
- flash_if: Stub flash interface (RAM-backed)
- boot_jump: Logic to jump to application (not executed in mock to preserve debug session)

## Image Format

Header (32 bytes packed):
```
uint32_t magic      = 0x424C444D ('BLDM')
uint32_t header_size = sizeof(boot_image_header_t)
uint32_t image_size  = <payload size>
uint32_t version
uint32_t crc32       (over payload only)
uint32_t flags
uint32_t reserved[2]
```

## Memory Layout Assumptions

- Bootloader resides below 0x00008000.
- Application starts at 0x00008000 (APP_FLASH_BASE) and occupies up to APP_MAX_SIZE_BYTES.
- Adjust these in `boot_image.h` and linker scripts for production.

## Build Notes

Use S32 Design Studio / arm-none-eabi-gcc toolchain. Ensure include path contains `include/` and compile all `src/*.c` files.

Example (GNU Arm Embedded GCC – illustrative only):
```
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 -Iinclude \
  src/main.c src/boot_manager.c src/boot_comm.c src/boot_crc.c src/flash_if.c src/boot_jump.c \
  Project_Settings/Startup_Code/startup.c Project_Settings/Startup_Code/system_S32K144.c \
  -T Project_Settings/Linker_Files/S32K144_64_flash.ld -Wl,--gc-sections -o bootloader.elf
```

### Windows (PowerShell) Quick Build

Prerequisites:
1. Install GNU Arm Embedded Toolchain (e.g. 10.x or later) and add its `bin` to PATH, or set `GCC_PREFIX`.
2. (Optional) Install `make` (MSYS2, Git for Windows, or S32DS build tools).

Build using provided `Makefile`:
```
make        # builds bootloader.elf + bootloader.bin
make clean  # cleans artifacts
```

If toolchain not in PATH:
```
$env:GCC_PREFIX="C:/gcc-arm-none-eabi/bin/arm-none-eabi-"; make
```

Artifacts:
- `bootloader.elf` ELF with symbols.
- `bootloader.bin` raw binary image.
- `bootloader.map` link map.

### Linker Script Adjustment

`S32K144_64_flash.ld` modified so bootloader `.text` region ends before `0x00008000` (application base). Original `m_text` length covered almost all flash; now limited to `0x7BF0` bytes. Ensure your application project uses a linker script whose `m_text` (or equivalent) origin is `0x00008000`.

### Typical S32 Design Studio Integration
1. Create new S32DS project (bareboard, S32K144).
2. Replace generated linker file with provided modified one (or copy change to size of `m_text`).
3. Add sources under `src/` and headers under `include/` to project.
4. Define (optional) symbols: `-DMOCK_BOOTLOADER`.
5. Disable semihosting unless using `printf` via RTT/UART.
6. Build ▶️ Debug.

### Memory Reservation Summary
| Region        | Start       | End (exclusive) | Purpose          |
|---------------|-------------|-----------------|------------------|
| Bootloader    | 0x00000410  | 0x00008000      | Bootloader code  |
| Application   | 0x00008000  | (rest of flash) | Main firmware    |

Adjust as needed for real size. Keep safety margin (e.g. leave 4–8 KB slack) if bootloader grows.

### Flash Programming (Real Hardware TODO)
Replace `flash_if.c` with calls to NXP SDK Flash driver (FTFC). Sequence typically:
1. Initialize flash clock + driver.
2. Erase sector(s) (check alignment & size).
3. Program phrases/longwords.
4. Verify & optionally read-back CRC.

### Verifying Size Constraint
After build:
```
arm-none-eabi-size bootloader.elf
```
Confirm that `.text + .rodata + .data` total < 0x7BF0.

### Optional: Generate Disassembly
```
arm-none-eabi-objdump -d bootloader.elf > bootloader.disasm
```

## Next Steps (Not Implemented Here)

- Real UART/CAN driver integration.
- Real flash controller operations (FTFC) and error handling.
- Authentication / signature verification.
- Failsafe / rollback.
- Watchdog servicing and timeout-based update entry.
- LED / console logging abstraction.

## Disclaimer

This code is for educational/mock purposes only and omits many production safety, security, and robustness features.
