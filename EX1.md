# CISC vs RISC — Tổng hợp chi tiết (kèm ví dụ & minh họa)

---

## 1) Khái niệm
- **CISC** (Complex Instruction Set Computer): Bộ lệnh **phức tạp**, **nhiều** lệnh; một lệnh có thể thực hiện **nhiều thao tác** (load + tính toán + lưu).
- **RISC** (Reduced Instruction Set Computer): Bộ lệnh **tối giản**, **ít** lệnh; mỗi lệnh làm **một việc nhỏ**, tốc độ/chu kỳ **đều** và dễ **pipeline**.

> MCU họ **ARM Cortex-M (STM32, NXP i.MX RT, TI MSP432, v.v.)** là **RISC**. PC/laptop dùng **x86/x86-64** (Intel/AMD) là **CISC**.

---

## 2) Minh họa tổng quan
### Sơ đồ khối CISC vs RISC
![CISC vs RISC Datapath](sandbox:/mnt/data/cisc_risc_block.png)  
*Hình minh họa datapath & control trong kiến trúc CISC (trái) và RISC (phải).*

### So sánh cấu trúc bộ xử lý
![Cấu trúc RISC vs CISC](sandbox:/mnt/data/cpu_structure.png)  
*Hình mô tả đường dữ liệu và điều khiển trong hai kiến trúc.*

### Bảng so sánh trực quan
![So sánh bảng](sandbox:/mnt/data/comparison_table.png)  
*Bảng so sánh các tiêu chí cơ bản giữa RISC và CISC.*

### Kiến trúc Load/Store vs Direct Memory
![Sơ đồ load-store](sandbox:/mnt/data/load_store.png)  
*Sơ đồ nhấn mạnh nguyên tắc **Load/Store** của RISC và lệnh phức tạp của CISC.*

---

## 3) Điểm giống nhau
- Đều là kiến trúc tập lệnh của CPU/MCU.
- Cùng mục tiêu: thực thi chương trình, xử lý dữ liệu, giao tiếp bộ nhớ/ngoại vi.
- Đều có các khối: bộ giải mã lệnh, ALU/FPU, thanh ghi, bus, bộ nhớ lệnh & dữ liệu.
- Đều phát triển để đạt **hiệu năng/điện năng/chi phí** tốt hơn.

---

## 4) Điểm khác nhau (bảng tổng hợp)

| Tiêu chí | **CISC** | **RISC** |
|---|---|---|
| **Kích thước & độ phức tạp lệnh** | Nhiều lệnh, lệnh **dài/biến độ dài**, giải mã **phức tạp** | Ít lệnh, lệnh **ngắn/fixed hoặc gần fixed**, giải mã **đơn giản** |
| **Cách truy cập bộ nhớ** | Nhiều lệnh có thể **đọc/ghi trực tiếp** bộ nhớ khi tính toán | **Load/Store** thuần: chỉ lệnh load/store truy cập bộ nhớ, ALU làm việc trên **thanh ghi** |
| **Mật độ mã (code density)** | **Cao** (ít lệnh hơn để làm cùng việc) | Thường **thấp hơn**; được cải thiện bằng **Thumb/Thumb-2 (ARM)**, **compressed ISA (RISC-V C)** |
| **Chu kỳ/1 lệnh** | Thường **nhiều chu kỳ/1 lệnh** | Hướng tới **1 chu kỳ/1 lệnh** (không tuyệt đối) |
| **Thiết kế pipeline** | Khó pipeline đều do lệnh biến độ dài & vi mã | **Dễ pipeline** nhờ lệnh đơn giản/độ dài cố định |
| **Vi mã (microcode)** | Phổ biến (x86 dùng nhiều) | Ít hoặc không cần |
| **Phần cứng** | Phức tạp hơn → tốn transistor, điện năng | Đơn giản hơn → dễ mở rộng lõi/điện năng thấp |
| **Ví dụ kiến trúc** | x86/x86-64 | ARM, RISC-V, MIPS, SPARC (lịch sử), PowerPC |
| **Miền ứng dụng** | PC, server, desktop HĐH phức tạp | Nhúng, MCU, di động, SoC tiết kiệm năng lượng |

---

## 5) Ví dụ lệnh: Nhân hai số trong bộ nhớ

### 5.1 Cách “CISC” (x86)
```asm
; Giả định địa chỉ A và B ở bộ nhớ, đích ở C
MUL_MEM C, A, B     ; (Giả sử) đọc A, B từ RAM, nhân, ghi C về RAM
```

### 5.2 Cách “RISC” (ARM Cortex-M / RISC-V)
```asm
LDR   r0, [A]       ; load A từ RAM vào r0
LDR   r1, [B]       ; load B từ RAM vào r1
MUL   r2, r0, r1    ; r2 = r0 * r1
STR   r2, [C]       ; store r2 về RAM
```

---

## 6) Pipeline minh họa (ASCII)

### RISC (5 stage điển hình)
```
IF → ID → EX → MEM → WB
```
- IF: nạp lệnh  
- ID: giải mã/đọc thanh ghi  
- EX: thực thi ALU  
- MEM: truy cập bộ nhớ (chỉ load/store)  
- WB: ghi kết quả  

### CISC (khái quát)
```
IF → Decode (vi mã phức tạp) → nhiều vi bước EX/MEM → WB
```
- Độ dài lệnh biến đổi → khó pipeline đều.  
- Nhiều lệnh cần vi mã → tách thành micro-ops nội bộ.  

---

## 7) Ánh xạ sang MCU thực tế
- **RISC/ARM Cortex-M**: STM32 (ST), LPC & i.MX RT (NXP), MSP432 (TI), GD32 (GigaDevice), Nordic nRF52 (BLE), v.v.
- **RISC-V MCU**: ESP32-C3/C6 (Espressif), GD32VF103, SiFive-based SoC, v.v.
- **TI C2000 (DSP-enabled MCU)**: kiến trúc tối ưu **điều khiển động cơ & xử lý tín hiệu**, có **MAC**, pipeline sâu.

---

## 8) Khi nào chọn cái nào
- **RISC/ARM/RISC-V**: tối ưu **điện năng thấp**, **ngoại vi đa dạng**, phù hợp hệ thống nhúng (STM32, NXP, TI MSP432).  
- **CISC/x86**: phù hợp PC, server, IPC cần chạy OS nặng.  
- **TI C2000**: nhúng real-time, motor control, power electronics.

---

### Ghi chú nhanh
- **CISC**: *ít lệnh, lệnh to, code gọn, decode khó*.  
- **RISC**: *nhiều lệnh nhỏ, code có thể dài hơn, pipeline dễ, tiết kiệm điện*.  
