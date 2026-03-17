# Linux Programming Assignment: Timer Analysis & Multithreading

Dự án thực hành lập trình Linux mô phỏng hệ thống quản lý đa luồng (Multithreading) và đánh giá độ trễ thời gian (Timer Jitter) trên hệ điều hành Linux. Dự án thuộc khuôn khổ chương trình đào tạo Nhóm thực tập Nhúng – dự án gNodeB 5G VHT.

---

## 1. Mô tả dự án

Chương trình C bao gồm 3 threads hoạt động song song và được đồng bộ hóa chặt chẽ bằng `pthread_mutex` và `pthread_cond` theo chuẩn POSIX:

* **SAMPLE Thread:** Thực hiện lặp vô hạn việc đọc thời gian hệ thống hiện tại (chính xác đến nanosecond) vào biến `T`. Luồng này sử dụng `nanosleep` để nghỉ một khoảng thời gian bằng chu kỳ `X` ns.
* **INPUT Thread:** Định kỳ kiểm tra file `freq.txt`. Nếu phát hiện giá trị chu kỳ `X` thay đổi, luồng sẽ cập nhật ngay lập tức vào hệ thống. Người dùng hoặc kịch bản tự động có thể `echo` giá trị vào file này.
* **LOGGING Thread:** Chờ đợi tín hiệu cập nhật từ luồng SAMPLE. Khi `T` có giá trị mới, luồng sẽ tính toán `interval` (khoảng thời gian giữa biến `T` hiện tại và lần ghi trước) và ghi dữ liệu ra file `time_and_interval.txt` để phục vụ phân tích.

## 2. Cấu trúc thư mục

\`\`\`text
linux_timer_analysis/
├── Makefile                # Kịch bản biên dịch chương trình C tự động
├── README.md               # Tài liệu mô tả dự án
├── src/
│   └── main.c              # Mã nguồn C (Quản lý 3 threads chuẩn TLPI)
├── scripts/
│   ├── run_test.sh         # Shell script tự động hóa quá trình test (5 phút)
│   └── plot_data.py        # Script Python vẽ đồ thị đánh giá jitter
└── logs/                   # Thư mục chứa dữ liệu đầu ra và ảnh đồ thị
\`\`\`

## 3. Hướng dẫn sử dụng

### 3.1. Cài đặt môi trường

Đảm bảo hệ thống đã cài đặt `gcc`, `make` và thư viện Python cần thiết để vẽ đồ thị:

\`\`\`bash
pip3 install pandas matplotlib
\`\`\`

### 3.2. Chạy tự động thu thập dữ liệu

Sử dụng Shell Script để tự động dọn dẹp, biên dịch và chạy chương trình trong 5 phút. Chu kỳ `X` sẽ tự động chuyển đổi qua các mức: 1000000 ns, 100000 ns, 10000 ns, 1000 ns, 100 ns sau mỗi 1 phút.

\`\`\`bash
chmod +x scripts/run_test.sh
./scripts/run_test.sh
\`\`\`

### 3.3. Vẽ đồ thị đánh giá (Khảo sát)

Sau khi quá trình thu thập hoàn tất, tiến hành chạy script Python để trực quan hóa độ trễ (interval) đối với mỗi giá trị chu kỳ `X`:

\`\`\`bash
python3 scripts/plot_data.py
\`\`\`
Kết quả khảo sát sẽ được lưu tại: `logs/interval_evaluation.png`.
