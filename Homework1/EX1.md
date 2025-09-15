# So sánh kiến trúc CISC và RISC

## 1. Giới thiệu khái niệm cơ bản

### 1.1 CISC (Complex Instruction Set Computer)

CISC là kiến trúc vi xử lý với tập lệnh phức tạp. Mỗi lệnh trong CISC có thể thực hiện nhiều thao tác ở mức phần cứng, ví dụ: tải dữ liệu, thực hiện phép toán, và lưu kết quả chỉ trong một lệnh. Kiến trúc này hướng đến việc giảm số lệnh của chương trình bằng cách tăng độ phức tạp của mỗi lệnh.

### 1.2 RISC (Reduced Instruction Set Computer)

RISC là kiến trúc vi xử lý với tập lệnh rút gọn. Mỗi lệnh thường được thiết kế để thực hiện một tác vụ đơn giản và có thời gian thực thi gần như đồng nhất (thường trong một chu kỳ xung nhịp). RISC tập trung vào hiệu quả của pipeline và tốc độ xử lý.

---

## 2. Ưu điểm và nhược điểm

### 2.1 CISC

**Ưu điểm:**

* Ít dòng lệnh trong chương trình do mỗi lệnh phức tạp hơn.
* Dễ dàng hơn trong việc lập trình assembly ở mức thấp.
* Phù hợp với các ứng dụng truyền thống và PC (Intel x86).

**Nhược điểm:**

* Mạch giải mã phức tạp, tiêu tốn nhiều transistor.
* Khó tối ưu hóa pipeline.
* Tốc độ mỗi lệnh không đồng nhất, ảnh hưởng đến hiệu suất.

### 2.2 RISC

**Ưu điểm:**

* Thiết kế đơn giản, dễ tối ưu pipeline.
* Lệnh được thực hiện nhanh và đồng nhất.
* Tiêu tốn ít năng lượng, phù hợp thiết bị di động.

**Nhược điểm:**

* Chương trình dài hơn do cần nhiều lệnh để hoàn thành cùng một công việc.
* Có thể cần bộ nhớ lớn hơn.

---

## 3. So sánh CISC và RISC theo tiêu chí

### 3.1 Cấu trúc tập lệnh

* **CISC:** Tập lệnh phong phú, nhiều chế độ địa chỉ phức tạp.
* **RISC:** Tập lệnh rút gọn, đơn giản, thường dạng load/store.

### 3.2 Tốc độ xử lý

* **CISC:** Một lệnh có thể cần nhiều chu kỳ máy.
* **RISC:** Hầu hết các lệnh hoàn thành trong một chu kỳ máy.

### 3.3 Kích thước chương trình

* **CISC:** Kích thước chương trình nhỏ hơn do ít lệnh.
* **RISC:** Chương trình dài hơn vì cần nhiều lệnh hơn.

### 3.4 Độ phức tạp phần cứng

* **CISC:** Phần cứng phức tạp, bộ giải mã lệnh lớn.
* **RISC:** Phần cứng đơn giản, dễ mở rộng pipeline.

### 3.5 Ứng dụng thực tế

* **CISC:** Dùng phổ biến trong máy tính cá nhân, máy chủ (Intel x86, AMD64).
* **RISC:** Dùng trong thiết bị nhúng, di động, IoT (ARM, MIPS, RISC-V).

---

## 4. Quan điểm cá nhân

Trong bối cảnh hệ thống nhúng hiện nay, **RISC** phù hợp hơn vì ưu tiên hiệu suất năng lượng, đơn giản hóa phần cứng và khả năng mở rộng. Các vi xử lý RISC như ARM và RISC-V đang thống trị lĩnh vực thiết bị di động, IoT, và nhúng nhờ tính hiệu quả và linh hoạt. CISC vẫn giữ vai trò quan trọng trong PC và máy chủ, nhưng xu hướng hệ thống nhúng hiện đại nghiêng mạnh về RISC.

---

## 5. Kết luận

* CISC và RISC có các điểm mạnh riêng: CISC tối ưu về số lượng lệnh, RISC tối ưu về tốc độ và năng lượng.
* Lựa chọn kiến trúc tùy thuộc vào ứng dụng. Với các hệ thống nhúng cần tiết kiệm năng lượng, **RISC** là lựa chọn tối ưu.
