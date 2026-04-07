# Hệ thống Giám sát Cảm biến Thời gian thực

Mini project giả lập hệ thống đọc tín hiệu cảm biến (Nhiệt độ, Độ ẩm)

## 1. Cấu trúc Hệ thống
* `main.c`: Chạy vòng lặp giám sát liên tục 1s/lần.
* `sensor`: Đọc cấu hình và giả lập tín hiệu.
* `buffer` & `processing`: Dùng bộ đệm vòng lọcrác và tính Trung bình trượt.
* `stats`: Thống kê số lượng lỗi.
* `config.txt`: File thiết lập thiết bị (ID, Loại, Ngưỡng, Chu kỳ, Phòng).

## 2. Ghi Log 
Hệ thống tự động lưu song song ra 2 file:
* `activity_log.txt`: Ghi chi tiết mọi kết quả đo đạc theo từng giây.
* `system_log.txt`: Chỉ ghi lại các sự cố (Lỗi rác, Mất kết nối, Vượt ngưỡng).

## 3. Cách chạy
1. Tải toàn bộ mã nguồn về máy 
2. Đặt file `config.txt` cùng thư mục với các file code.
3. Biên dịch toàn bộ code và chạy file thực thi `.exe`.