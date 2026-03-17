# Tên file thực thi
TARGET = timer_app

# Trình biên dịch và các cờ (flags)
CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread

# Đường dẫn thư mục
SRC_DIR = src
LOG_DIR = logs

# Các file nguồn
SOURCES = $(SRC_DIR)/main.c
OBJECTS = $(SOURCES:.c=.o)

# Target mặc định
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Target dọn dẹp các file đã build
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)
	rm -f $(LOG_DIR)/freq.txt $(LOG_DIR)/time_and_interval.txt $(LOG_DIR)/*.png

.PHONY: all clean
