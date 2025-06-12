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
o	Khi chọn M3 chương trình cho phép chọn thuật toán sắp xếp (chọn, chèn, nổi bọt, quicksort) và khóa để sắp xếp (mã sinh viên, họ và tên, ngày sinh, điểm trung bình tích lũy). Có thể xây dựng các mục chọn này dạng menu (cấp 2).
o   Khi chọn M4 chương trình cho phép chọn thuật toán tìm kiếm (tuần tự, nhị phân), khóa cần tìm kiếm (mã lớp, mã sinh viên, Họ và tên, ngày sinh, điểm trung bình tích lũy) và giá trị của khóa cần tìm. Có thể xây dựng các mục chọn này dạng menu (cấp 2).
o   Khi chọn M5 chương trình cho phép chọn báo cáo số lượng SV theo lớp hoặc tỷ lệ phân loại kết quả học tập (xuất sắc, giỏi, khá, trung bình, yếu) theo lớp. Có thể xây dựng các mục chọn này dạng menu (cấp 2).
o   Khi chọn M6 chương trình kết thúc.

	Kiến thức liên quan: 
o	Vào ra bàn phím/màn hình
o	Điều khiển chọn, điều khiển lặp
	Kết quả: 
o	Chương trình chạy và cho phép NSD chọn lựa được các chức năng theo yêu cầu đặt ra;
o	Tổ chức chương trình gồm: Chương trình chính, khung các chương trình con (hàm) để thực hiện từng chức năng tương ứng với hệ thống menu đặt ra.
-	Nội dung 2: Xây dựng cấu trúc dữ liệu và các hàm nhập/xuất dữ liệu
	Yêu cầu: Xây dựng các cấu trúc dữ liệu phù hợp để quản lý đối tượng của bài toán, viết các hàm thực hiện việc cập nhật hồ sơ, đọc/ghi dữ liệu từ file.
	Kiến thức liên quan: 
o	Các cấu trúc dữ liệu do người dùng định nghĩa;
o	Thao tác đọc/ghi file;
o	Viết hàm thực hiện các chức năng.
	Kết quả: 
o	Chương trình chạy và cho phép NSD cập nhật được hồ sơ, in được danh sách (toàn bộ);
o	Hồ sơ đối tượng được định nghĩa theo cấu trúc và được lưu trữ trên file.
 
-  Nội dung 3: Thực hiện các thuật toán sắp xếp
	Yêu cầu: Thực hiện các thuật toán xử lý, sắp xếp danh sách đối tượng của bài toán. Khóa được dùng để sắp xếp là số, xâu ký tự, ngày tháng …
	Kiến thức liên quan: Xử lý xâu ký tự, các thuật toán sắp xếp.
	Kết quả:
o	Chương trình chạy và cho phép NSD lựa chọn thuật toán sắp xếp.
o	Chương trình chạy và cho phép NSD lựa chọn khóa để sắp xếp.
o	In danh sách sau khi đã sắp xếp.
IV. YÊU CẦU CHI TIẾT
1. Xây dựng menu
1.1	Mức đơn giản: C
a.	Có menu để lựa chọn, chọn số thứ tự menu
1.2.	Mức trung bình: B
a.	Menu chạy, thay đổi màu sắc nổi bật của chức năng được chọn
1.3.	Mức nâng cao: A
a.	Cho phép di chuyển dùng phím mũi tên
b.	Menu con xổ dọc hoặc xổ ngang
2. Nhập/xuất dữ liệu:
2.1.	Xây dựng file có cấu trúc
2.2.	Nhập dữ liệu
a.	Mức đơn giản C:
	Dữ liệu nhập vào được ghi vào file
b.	Mức trung bình B 
	Chuẩn hóa, kiểm tra tính đúng đắn của dữ liệu.
	VD: Mã SV phải đúng 8 số
	Ngày tháng năm sinh đúng chuẩn
	Cho phép nhập tên file dữ liệu
c.	Mức nâng cao A
	Có hộp textbox cho phép nhập và chỉnh sửa dữ liệu.
2.3.	Xuất dữ liệu:
a.	Mức đơn giản C: Đọc dữ liệu ra màn hình
b.	Mức trung bình B: 
-	Vẽ bảng,
-	Dữ liệu được canh lề trái, phải, giữa, tùy thuộc loại dữ liệu in ra.
c.	Mức nâng cao A:
	Có thể dùng phím mũi tên, trượt lên xuống để xem dữ liệu hoặc lật trang.
3. Cài đặt các thuật toán sắp xếp
-	Các thuật toán: Sắp chọn; chèn; nổi bọt, nhanh (Quicksort);
-	Trường khóa (key) sắp xếp
o	Mã sinh viên
o	Lớp
o	Họ tên
o	Ngày sinh
o	ĐIểm trung bình
a)	Yêu cầu mức đơn giản C:
-	Chạy được thuật toán sắp xếp theo một vài tiêu chí
b)	Yêu cầu mức trung bình B:
-	Chạy được các thuật toán sắp xếp trên tất cả các trường (key)
c)	Yêu cầu mức nâng cao A:
-	Như yêu cầu mức Trung bình và phần hiển thị ra màn hình cho phép dịch chuyển con trỏ, lật trang xem.
V. TRIỂN KHAI CỤ THỂ
Xây dựng khung chương trình và giao diện dạng menu với nội dung như sau:
o Giao diện chính gồm các mục chọn:
1. Thêm mới hồ sơ (M1)
2. In danh sách (M2)
3. Sắp xếp (M3)
4. Tìm kiếm (M4)
5. Thống kê (M5)
6. Thoát (M5)
o Khi chọn M1, chương trình cho phép nhập vào hồ sơ sinh viên gồm các
thông tin:
 Mã lớp
 Mã sinh viên
 Họ và tên
 Ngày sinh
 Điểm trung bình tích lũy2
o Khi chọn M2 chương trình cho phép in ra danh sách sinh viên theo thứ
tự đã sắp xếp (khi chọn M3) và tìm kiếm (khi chọn M4)
o Khi chọn M3 chương trình cho phép chọn thuật toán sắp xếp(chọn, chèn,
quicksort, mergersort) và khóa để sắp xếp (mã sinh viên, họ và tên, ngày
sinh, điểm trung bình tích lũy). Có thể xây dựng các mục chọn này dạng
menu (cấp 2).
o Khi chọn M4 chương trình cho phép chọn thuật toán tìm kiếm (tuần tự,
nhị phân), khóa cần tìm kiếm (mã lớp, mã sinh viên, Họ và tên, ngày
sinh, điểm trung bình tích lũy) và giá trị của khóa cần tìm. Có thể xây
dựng các mục chọn này dạng menu (cấp 2).
o Khi chọn M5 chương trình cho phép chọn báo cáo số lượng SV theo lớp
hoặc tỷ lệ phân loại kết quả học tập (xuất sắc, giỏi, khá, trung bình, yếu)
theo lớp. Có thể xây dựng các mục chọn này dạng menu (cấp 2).
o Khi chọn M6 chương trình kết thúc.
VI. CÔNG CỤ SỬ DỤNG
- Lệnh If
- Lệnh Switch - case
- Lệnh For
- Lệnh While, do -while
- Lệnh break và continue
* Cấu trúc chương trình
Một chương trình hoàn chỉnh trong C/C++ có 6 phần chính (nhưng không bắt buộc)
theo thứ tự như sau:
- Chỉ thị tiền xử lý;
- Định nghĩa kiểu dữ liệu;
- Khái báo prototype;
- Khai báo biến ngoài;
- Chương trình chính và
- Cài đặt hàm.
VII. CẤU TRÚC DỮ LIỆU VÀ CHỨC NĂNG NHẬP -XUẤT
1. Yêu cầu:
- Xây dựng cấu trúc dữ liệu phù hợp để quản lý đối tượng sinh viên,
gồm các thông tin:
 Mã lớp
 Mã sinh viên
 Họ và tên
 Ngày sinh
 Điểm trung bình tích lũy
- Dữ liệu (hồ sơ sinh viên) được ghi trên file.
- Xây dựng các chức năng cho phép nhập hồ sơ, in danh sách đã
nhập.
- Tự động chỉnh sửa chính tả khi nhập họ tên sinh viên.
- Kiểm tra tính hợp lệ của ngày sinh khi nhập. Ngày sinh có dạng
dd/mm/yyyy, dd là ngày có giá trị trong khoảng từ 1 đến 31, mm là
tháng có giá trị trọng khoảng 1 đến 12 và yyyy là năm có giá trị từ
1900 đến 2016; và dd/mm/yyyy phải là ngày hợp hệ (có trên lịch),
ví dụ ngày 30/2/2016 là không hợp lệ.
- Kiểm tra tính hợp lệ của điểm trung bình tích lũy, điểm trung bình
tích lũy >=0 và <=10.
- Các chức năng này được kết hợp trong chương trình đã xây dựng
ở các tuần trước.
2. Công cu sử dụng
- Kiểu struct
- Con trỏ kiểu struct
- Cấu trúc với thành phần kiểu bit
- Câu lệnh typedef
- Hàm sizeof()
- Làm việc với file: khai báo, mở, đóng, kiểm tra cuối tệp, rewind(), fputc(),  fputs(), fprintf(), fgetc, fgets, fscanf, 
- truy cập tệp nhị phân: size_t fwrite, size_t fread
VIII. THỰC HIỆN CÁC CHỨC NĂNG SẮP XẾP
1. Yêu cầu:
Với dữ liệu sinh viên gồm các thông tin:
 Mã lớp
 Mã sinh viên
 Họ và tên
 Ngày sinh
 Điểm trung bình tích lũy
đã nhập và lưu trữ trên file.
Hãy thực hiện các các yêu cầu sau:
- Sắp xếp danh sách sinh viên theo Mã lớp. 
- Sắp xếp danh sách sinh viên theo Mã sinh viên (như sanh sách đã
cho).
- Sắp xếp danh sách sinh viên theo Họ và tên. Họ và tên sinh viên
được so sánh theo tên rồi đến họ đệm. 
- Sắp xếp danh sách sinh viên theo Ngày sinh. Ngày sinh được so
sánh theo năm, đến tháng, và cuối là đến ngày.
- Sắp xếp danh sách sinh viên theo mã lớp, cùng mã lớp sắp xếp
theo họ tên, cùng họ tên thì sắp xếp theo ngày sinh.
2. Công cụ sử dụng
- Selection Sort
- Bubble Sort
- Insertion Sort
3. Công cụ xử lý xâu
- khai báo và nhập xuất
- Chuyển đổi kiểu chữ, chuyển đổi thứ tự: strlwr, strupr, strrev
- Xác định chiều dài: strlen
- sao chép, ghép xâu: strcpy, strcat, strdup
- So sánh 2 xâu: strcmp
- tìm xuất hiện kí tự trong xâu: strchr
- Tìm kiếm xâu con xuất hiện trong xâu khác: strstr
- Biến đổi kiểu xâu thành số: atoi, atol, atof