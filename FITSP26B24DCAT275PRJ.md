# BÁO CÁO BÀI TẬP LỚN: LẬP TỪ MÃ HỆ THỐNG CỦA BẢN TIN CHO TRƯỚC

## Nhóm 6
- B24DCAT264 
- B24DCAT269
- B24DCAT077
- B24DCAT275


## 1. Mô tả chung

Chương trình mô phỏng quá trình lập từ mã hệ thống với bản tin cho trước trên trường nhị phân (GF(2)).

Dự án được chia làm 2 phiên bản để đáp ứng trọn vẹn yêu cầu cơ bản và điểm Bonus của môn học:
-`App.cpp`: Ứng dụng hoàn chỉnh, nhập liệu thân thiện (dạng đa thức hoặc chuỗi bit), hiển thị chi tiết 4 bước giải tự luận (với bảng chia đa thức trực quan) và tích hợp hệ thống sinh câu hỏi trắc nghiệm ôn tập có tính năng random.
- `icpcformat.cpp`: Phiên bản tối giản luồng I/O theo chuẩn ICPC, không hiển thị các chuỗi thông báo thừa, chỉ nhận mảng hệ số và in ra từ mã cuối cùng.


## 2. Cách biên dịch và khởi chạy

Sử dụng trình biên dịch C++ (G++) để tạo file thực thi:
+ Chạy bản tương tác (App):
./App
Input mẫu:
Nhap l k: 7 4
Nhap da thuc sinh g(x): 1 + x + x^3
Nhap ban tin m: 1011
(Hệ thống sẽ hỏi bạn có muốn xem bảng tính chia và làm quiz hay không)

+ Chạy bản ICPC (icpcformat):


./icpcformat
Input mẫu: (Nhập các hệ số cách nhau bằng khoảng trắng từ bậc thấp tới cao)
7 4
1 1 0 1
1 0 1 1

## 3. Thiết kế
Chương trình áp dụng tư duy lập trình hướng đối tượng (OOP)
Dự án mô hình hóa các khái niệm toán học thông qua các Lớp (Class), cụ thể được thay đổi theo sát code thực tế:

Bit: Quản lý giá trị 0/1, nạp chồng toán tử + thành phép XOR và * thành phép AND.

BiVec: Quản lý một mảng các đối tượng Bit (đại diện cho vector nhị phân).

Da_thuc: Kế thừa BiVec, triển khai thuật toán chia lấy dư Mod_da_thuc và các hàm định dạng hiển thị align_dathuc, format_dathuc.

CyclicCode: Chứa tham số chiều dài l, k và đa thức sinh g_x. Triển khai quy trình lập mã vòng 4 bước theo giáo trình.

## 4. Lưu đồ và Logic hoạt động (Bản App.cpp)
4.1. Lưu đồ tổng quát chương trình
// với bản App, bản icpc cơ bản vẫn cùng 1 logic.
+----------------------+
|      BẮT ĐẦU         |
+----------------------+
           |
           v
+----------------------+
| Nhập l, k, g(x), m   |
+----------------------+
           |
           v
+-------------------------------+
| Kiểm tra tính hợp lệ (Error)  |
| 1. deg(g(x)) == l - k         |
| 2. len(m) <= k                |
| 3. (x^l + 1) % g(x) == 0      |
+-------------------------------+
        | YES             | NO
        v                 v
+-------------------+    +-------+
| Hỏi in Tự luận?   |--->| ERROR |
+-------------------+    +-------+
        |
        v
+-------------------+
| Thực hiện mã hóa  |
| (In 4 bước + chia)|
+-------------------+
        |
        v
+-------------------+
| Hỏi làm Quiz?     |
+-------------------+
        |
        v
+-------------------+
| Random câu hỏi    |
+-------------------+
        |
        v
+----------------------+
|      KẾT THÚC        |
+----------------------+
4.2. Lưu đồ thuật toán chia đa thức (Mod_da_thuc)       
+----------------------+
|    Mod_da_thuc()     |
+----------------------+
           |
           v
+----------------------+
| Duyệt từ bậc cao     |
| của Số bị chia       |
+----------------------+
           |
           v
+----------------------+
| Bit hiện tại = 1 ?   |
+----------------------+
       | YES      | NO
       v          |
+----------------------+
| XOR với Số chia      |
| đã dịch (Shift)      |
| (Kèm In giao diện)   |
+----------------------+
           |
           v
+----------------------+
| Lặp tới bậc l-k      |
+----------------------+
           |
           v
+----------------------+
| Trả về phần dư p(x)  |
+----------------------+
## 5. Kiểm tra tính đúng đắn
+ Chương trình triển khai 3 chốt chặn kiểm tra chặt chẽ tính chuẩn xác của mã vòng. Nếu vi phạm bất kỳ điều kiện nào, chương trình lập tức in ra Error và dừng lại:
Bậc của đa thức sinh g(x) phải đúng bằng l - k.

Độ dài bản tin m nhập vào không được vượt quá k.

Đa thức sinh g(x) phải là một ước của x^l + 1 (Thực hiện phép chia (x^l + 1) chia g(x)$ có phần dư bằng 0).
## 6. Hướng dẫn cách thức cài đặt
Chương trình được viết bằng C++ chuẩn, không yêu cầu cài đặt thêm bất kỳ thư viện ngoài nào.
Người dùng chỉ cần cài đặt trình biên dịch C++ (ví dụ: GCC, MinGW).
Mở terminal/cmd tại thư mục chứa source code và chạy lệnh biên dịch để sinh file thực thi.

## Link git https://github.com/NgToanz8/pj_lttt