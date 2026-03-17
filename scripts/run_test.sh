#!/bin/bash

cd "$(dirname "$0")/.." || exit

echo "1. Dọn dẹp và biên dịch mã nguồn"
make clean
make

echo "2. Khởi tạo thư mục logs"
mkdir -p logs
> logs/time_and_interval.txt
echo "1000000" > logs/freq.txt

echo "3. Khởi chạy timer_app trong nền"
./timer_app &
APP_PID=$!
echo "   PID của timer_app: $APP_PID"

X_VALUES=(1000000 100000 10000 1000 100)

for X in "${X_VALUES[@]}"; do
    echo "  Thiết lập chu kỳ X = $X ns..."
    echo $X > logs/freq.txt
    sleep 60 
done

echo "4. Đã chạy đủ 5 phút. Đang gửi tín hiệu dừng (SIGTERM)..."
kill -SIGTERM $APP_PID
wait $APP_PID 2>/dev/null

echo "Done"
