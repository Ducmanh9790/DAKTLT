I. Đề bài: Xây dựng chương trình quản lý sinh viên (QLSV) với các thông tin cần
quản lý của một sinh viên gồm: Mã lớp, Mã sinh viên, Họ và tên, Ngày sinh, Điểm
trung bình tích lũy (ĐTBTL). Các chức năng chính của chương trình gồm: Thêm,
sửa, xóa hồ sơ sinh viên; In danh sách sinh viên theo lớp hoặc toàn bộ; Sắp xếp
danh sách sinh viên theo một (hoặc nhiều) các tiêu chí: Họ tên, Ngày sinh, ĐTBTL
bằng các thuật toán sắp xếp chọn, chèn, quicksort, mergesort, heapsort; Tìm kiếm
sinh viên theo một (hoặc nhiều) các tiêu chí: Họ tên, Ngày sinh, ĐTBTL bằng các
thuật toán tìm kiếm tuần tự, tìm kiếm nhị phân; Thực hiện các báo cáo thống kê
phần trăm xếp loại học tập theo lớp, tổng số sinh viên theo lớp. Dữ liệu được lưu
trữ dạng file nhị phân có cấu trúc. Chương trình được viết trên C/C++.
II. Yêu cầu: 
- Sử dụng công cụ lập trình: C++
-	Có thể sử dụng cấu trúc mảng để giải quyết bài toán.
-	Yêu cầu nâng cao: Sử dụng cấu trúc danh sách liên kết để giải quyết bài toán.
III.NỘI DUNG THỰC HIỆN
-	Nội dung 1: Xây dựng khung chương trình và menu chọn
	Yêu cầu: Xây dựng khung chương trình và giao diện dạng menu với nội dung như sau:
o	Giao diện chính gồm các mục chọn:
1. Thêm mới hồ sơ (M1)
2. In danh sách (M2)
3. Sắp xếp (M3)
4. Tìm kiếm (M4)
5. Thống kê (M5)
6. Thoát (M6)
o	Khi chọn M1, chương trình cho phép nhập vào hồ sơ sinh viên gồm các thông tin:
	Mã lớp
	Mã sinh viên
	Họ và tên
	Ngày sinh
	Điểm trung bình tích lũy
o	Khi chọn M2 chương trình cho phép in ra danh sách sinh viên theo thứ tự đã sắp xếp (khi chọn M3).
o	Khi chọn M3 chương trình cho phép chọn thuật toán sắp xếp (chọn, chèn, nổi bọt, quicksort) và khóa để sắp xếp (mã sinh viên, họ và tên, ngày sinh, điểm trung bình tích lũy). 
o   Khi chọn M4 chương trình cho phép chọn thuật toán tìm kiếm (tuần tự, nhị phân), khóa cần tìm kiếm (mã lớp, mã sinh viên, Họ và tên, ngày sinh, điểm trung bình tích lũy) và giá trị của khóa cần tìm. 
o   Khi chọn M5 chương trình cho phép chọn báo cáo số lượng SV theo lớp hoặc tỷ lệ phân loại kết quả học tập (xuất sắc, giỏi, khá, trung bình, yếu) theo lớp. 
o   Khi chọn M6 chương trình kết thúc.

Yêu cầu chi tiết:
I. Xây dựng menu
	Có menu để lựa chọn, chọn số thứ tự menu
II. Nhập/xuất dữ liệu:
	Xây dựng file có cấu trúc
    Dữ liệu nhập vào được ghi vào file
    Xuất dữ liệu: Đọc dữ liệu ra màn hình
III. Cài đặt các thuật toán sắp xếp
    Các thuật toán: Sắp chọn; chèn; nổi bọt, nhanh (Quicksort);
    Trường khóa (key) sắp xếp
o	Mã sinh viên
o	Lớp
o	Họ tên
o	Ngày sinh
o	ĐIểm trung bình
    Yêu cầu mức đơn giản C:
    Chạy được thuật toán sắp xếp theo một vài tiêu chí

