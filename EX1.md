# CISC vs RISC — Tổng hợp chi tiết (kèm ví dụ)

---

## 1) Khái niệm
- **CISC** (Complex Instruction Set Computer): Bộ lệnh **phức tạp**, **nhiều** lệnh; một lệnh có thể thực hiện **nhiều thao tác** (load + tính toán + lưu).
- **RISC** (Reduced Instruction Set Computer): Bộ lệnh **tối giản**, **ít** lệnh; mỗi lệnh làm **một việc nhỏ**, tốc độ/chu kỳ **đều** và dễ **pipeline**.

> MCU họ **ARM Cortex‑M (STM32, NXP i.MX RT, TI MSP432, v.v.)** là **RISC**. PC/laptop dùng **x86/x86‑64** (Intel/AMD) là **CISC**.

---

## 2) Điểm giống nhau
- Đều là kiến trúc tập lệnh của CPU/MCU.
- Cùng mục tiêu: thực thi chương trình, xử lý dữ liệu, giao tiếp bộ nhớ/ngoại vi.
- Cùng có các khối: bộ giải mã lệnh, ALU/FPU, thanh ghi, bus, bộ nhớ lệnh & dữ liệu.
- Đều phát triển để đạt **hiệu năng/điện năng/chi phí** tốt hơn.

---

## 3) Điểm khác nhau (bảng tổng hợp)

| Tiêu chí | **CISC** | **RISC** |
|---|---|---|
| **Kích thước & độ phức tạp lệnh** | Nhiều lệnh, lệnh **dài/biến độ dài**, giải mã **phức tạp** | Ít lệnh, lệnh **ngắn/fixed hoặc gần fixed**, giải mã **đơn giản** |
| **Cách truy cập bộ nhớ** | Nhiều lệnh có thể **đọc/ghi trực tiếp** bộ nhớ khi tính toán | **Load/Store** thuần: chỉ lệnh load/store truy cập bộ nhớ, ALU làm việc trên **thanh ghi** |
| **Mật độ mã (code density)** | **Cao** (ít lệnh hơn để làm cùng việc) | Thường **thấp hơn**; được cải thiện bằng **Thumb/Thumb‑2 (ARM)**, **compressed ISA (RISC‑V C)** |
| **Chu kỳ/1 lệnh** | Thường **nhiều chu kỳ/1 lệnh** | Hướng tới **1 chu kỳ/1 lệnh** (không tuyệt đối) |
| **Thiết kế pipeline** | Khó pipeline đều do lệnh biến độ dài & vi mã | **Dễ pipeline** nhờ lệnh đơn giản/độ dài cố định |
| **Vi mã (microcode)** | Phổ biến (x86 dùng nhiều) | Ít hoặc không cần |
| **Phần cứng** | Phức tạp hơn → tốn transistor, điện năng | Đơn giản hơn → dễ mở rộng lõi/điện năng thấp |
| **Ví dụ kiến trúc** | x86/x86‑64 | ARM, RISC‑V, MIPS, SPARC (lịch sử), PowerPC (nhiều biến thể) |
| **Miền ứng dụng** | PC, server, desktop HĐH phức tạp | Nhúng, MCU, di động, SoC tiết kiệm năng lượng |

---

## 4) Ví dụ lệnh: Nhân hai số trong bộ nhớ

### 4.1 Cách “CISC” (ý tưởng khái quát, kiểu x86)
Một lệnh có thể vừa **load** vừa **nhân** và có thể **store** kết quả:
```asm
; Giả định địa chỉ A và B ở bộ nhớ, đích ở C
MUL_MEM C, A, B     ; (Giả sử) đọc A, B từ RAM, nhân, ghi C về RAM
```
> Trên thực tế x86 có nhiều biến thể lệnh; ý tưởng là **ít lệnh hơn** để hoàn thành công việc.

### 4.2 Cách “RISC” (ARM Cortex‑M / RISC‑V)
Phải **tách** thành lệnh load → tính toán → store:
```asm
; r0, r1, r2 là thanh ghi
LDR   r0, [A]       ; load A từ RAM vào r0
LDR   r1, [B]       ; load B từ RAM vào r1
MUL   r2, r0, r1    ; r2 = r0 * r1
STR   r2, [C]       ; store r2 về RAM
```
> Lệnh **đơn giản**, pipeline **đều đặn**, dễ đạt throughput cao.

---

## 5) Pipeline minh họa (ASCII)

### RISC (5 stage điển hình)
```
IF → ID → EX → MEM → WB
 |     |     |     |____ Ghi kết quả
 |     |     |___________ Truy cập bộ nhớ (chỉ LOAD/STORE)
 |     |__________________ Thực thi ALU
 |________________________ Giải mã/đọc thanh ghi
__________________________ Nạp lệnh
```

### CISC (khái quát)
```
IF → Decode (phức tạp/vi mã) → {nhiều vi bước EX/MEM} → WB
- Độ dài lệnh biến đổi → khó canh hàng pipeline
- Thường cần vi mã chuyển 1 lệnh “to” thành nhiều micro‑ops
```

---

## 6) Code density & thực tế hiện đại
- **ARM Thumb/Thumb‑2** và **RISC‑V “C” extension** cho phép **mã lệnh ngắn hơn**, giúp **tiết kiệm Flash/RAM** trên MCU → thu hẹp khoảng cách so với CISC.
- Nhiều CPU CISC (x86) thực thi bằng cách **dịch lệnh phức tạp thành micro‑ops** nội bộ (gần giống RISC) trước khi pipeline.

---

## 7) Ánh xạ sang MCU thực tế
- **RISC/ARM Cortex‑M**: STM32 (ST), LPC & i.MX RT (NXP), MSP432 (TI), GD32 (GigaDevice), Nordic nRF52 (BLE), v.v.
- **RISC‑V MCU**: ESP32‑C3/C6 (Espressif), GD32VF103, SiFive‑based SoC, v.v.
- **TI C2000 (DSP‑enabled MCU)**: kiến trúc tối ưu **điều khiển động cơ & xử lý tín hiệu**, có **MAC**, pipeline sâu; thiên về **RISC‑like** nhưng bộ lệnh **chuyên biệt DSP** (khó xếp thuần CISC/RISC).

---

## 8) Khi nào chọn cái nào (góc nhìn embedded)
- **MCU RISC (ARM/RISC‑V)**: tối ưu **tiêu thụ điện**, **độ trễ thấp**, **ngoại vi phong phú**, hệ sinh thái RTOS/driver đồ sộ → phù hợp **STM32, NXP, TI MCU**.
- **CISC (x86/x64)**: phù hợp **máy tính/IPC** cần HĐH nặng (Windows/Linux đầy đủ), **điện năng cao hơn**.

---

## 9) Câu hỏi phỏng vấn thường gặp (và gợi ý trả lời ngắn)
1) **RISC có luôn nhanh hơn CISC?** — *Không*. Nhanh chậm phụ thuộc **vi kiến trúc**, xung nhịp, cache, compiler, bộ nhớ, v.v.
2) **Load/Store nghĩa là gì?** — Chỉ **LOAD/STORE** được phép truy cập RAM; lệnh ALU chỉ thao tác trên **thanh ghi**.
3) **Vì sao RISC dễ pipeline?** — Lệnh **đơn giản, độ dài cố định**, đường dữ liệu **đều** → chia stage rõ ràng.
4) **Code RISC dài hơn, có tệ không?** — Thumb/Thumb‑2 và nén lệnh giúp **giảm kích thước**; bù lại được **điện năng thấp & throughput tốt**.

---

## 10) Tài liệu gợi ý tự đọc thêm
- ARM Architecture Reference Manual (ARMv7‑M, ARMv8‑M)
- RISC‑V Unprivileged/Privileged Spec
- Agner Fog microarchitecture & optimization guides (x86)
- “Computer Architecture: A Quantitative Approach” — Hennessy & Patterson

---

### Ghi chú nhanh để bỏ túi
- **CISC**: *ít lệnh, lệnh to, code gọn, decode khó*.
- **RISC**: *nhiều lệnh nhỏ, code có thể dài hơn, pipeline dễ, tiết kiệm điện*.
