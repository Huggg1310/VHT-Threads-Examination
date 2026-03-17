# Linux Programming Assignment: Timer Analysis & Multithreading

[cite_start]Dự án thực hành lập trình Linux mô phỏng hệ thống quản lý đa luồng (Multithreading) và đánh giá độ trễ thời gian (Timer Jitter) trên hệ điều hành Linux[cite: 1, 4, 10]. [cite_start]Dự án thuộc khuôn khổ chương trình đào tạo Nhóm thực tập Nhúng – dự án gNodeB 5G VHT[cite: 2].

---

## 1. Mô tả dự án

[cite_start]Chương trình C bao gồm 3 threads hoạt động song song và được đồng bộ hóa chặt chẽ bằng `pthread_mutex` và `pthread_cond` theo chuẩn POSIX[cite: 4]:

* [cite_start]**SAMPLE Thread:** Thực hiện lặp vô hạn việc đọc thời gian hệ thống hiện tại (chính xác đến nanosecond) vào biến `T`[cite: 5, 6]. [cite_start]Luồng này sử dụng `nanosleep` để nghỉ một khoảng thời gian bằng chu kỳ `X` ns[cite: 5].
* [cite_start]**INPUT Thread:** Định kỳ kiểm tra file `freq.txt`[cite: 7]. [cite_start]Nếu phát hiện giá trị chu kỳ `X` thay đổi, luồng sẽ cập nhật ngay lập tức vào hệ thống[cite: 7]. [cite_start]Người dùng hoặc kịch bản tự động có thể `echo` giá trị vào file này[cite: 7].
* [cite_start]**LOGGING Thread:** Chờ đợi tín hiệu cập nhật từ luồng SAMPLE[cite: 7]. [cite_start]Khi `T` có giá trị mới, luồng sẽ tính toán `interval` (khoảng thời gian giữa biến `T` hiện tại và lần ghi trước) và ghi dữ liệu ra file `time_and_interval.txt` để phục vụ phân tích[cite: 7].

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

[cite_start]Sử dụng Shell Script để tự động dọn dẹp, biên dịch và chạy chương trình trong 5 phút[cite: 10]. [cite_start]Chu kỳ `X` sẽ tự động chuyển đổi qua các mức: 1000000 ns, 100000 ns, 10000 ns, 1000 ns, 100 ns sau mỗi 1 phút[cite: 8, 9].

\`\`\`bash
chmod +x scripts/run_test.sh
./scripts/run_test.sh
\`\`\`

### 3.3. Vẽ đồ thị đánh giá (Khảo sát)

[cite_start]Sau khi quá trình thu thập hoàn tất, tiến hành chạy script Python để trực quan hóa độ trễ (interval) đối với mỗi giá trị chu kỳ `X`[cite: 10]:

\`\`\`bash
python3 scripts/plot_data.py
\`\`\`
[cite_start]Kết quả khảo sát sẽ được lưu tại: `logs/interval_evaluation.png`[cite: 10].
