#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>
#include <conio.h>
using namespace std;

// Cấu trúc ngày tháng
struct Date {
    int day, month, year;
};

// Cấu trúc sinh viên
struct Student {
    string maLop;
    string maSV;
    string hoTen;
    Date ngaySinh;
    float diemTBTL;
};

// Khai báo hàm
void addStudent(vector<Student>& students);
void printStudents(const vector<Student>& students);
void sortStudents(vector<Student>& students);
void searchStudents(const vector<Student>& students);
void statisticsMenu(const vector<Student>& students);
void saveToFile(const vector<Student>& students);
void loadFromFile(vector<Student>& students);
bool isValidDate(const Date& date);
string formatName(string name);
int showMenu();

int main() {
    vector<Student> students;
    int choice;
    
    // Load dữ liệu từ file
    loadFromFile(students);
    
    do {
        choice = showMenu();
        
        switch(choice) {
            case 1:
                addStudent(students);
                break;
            case 2:
                printStudents(students);
                break;
            case 3:
                sortStudents(students);
                break;
            case 4:
                searchStudents(students);
                break;
            case 5:
                statisticsMenu(students);
                break;
            case 6:
                saveToFile(students);
                cout << "\nCam on ban da su dung chuong trinh!\n";
                break;
            default:
                cout << "\nLua chon khong hop le! Vui long chon lai.\n";
        }
        
        if(choice != 6) {
            cout << "\nNhan phim bat ky de tiep tuc...";
            _getch();
        }
        
    } while(choice != 6);
    
    return 0;
}

// Hiển thị menu chính
int showMenu() {
    system("cls");
    cout << "========================================\n";
    cout << "    CHUONG TRINH QUAN LY SINH VIEN     \n";
    cout << "========================================\n";
    cout << "1. Them moi ho so sinh vien\n";
    cout << "2. In danh sach sinh vien\n";
    cout << "3. Sap xep danh sach\n";
    cout << "4. Tim kiem sinh vien\n";
    cout << "5. Thong ke bao cao\n";
    cout << "6. Thoat chuong trinh\n";
    cout << "========================================\n";
    cout << "Nhap lua chon cua ban (1-6): ";
    
    int choice;
    cin >> choice;
    return choice;
}

// Thêm sinh viên mới
void addStudent(vector<Student>& students) {
    system("cls");
    cout << "==============================\n";
    cout << "    THEM MOI SINH VIEN       \n";
    cout << "==============================\n";
    
    Student sv;
    
    cout << "Nhap ma lop: ";
    cin.ignore();
    getline(cin, sv.maLop);
    
    // Kiểm tra mã sinh viên (8 số)
    bool validMaSV = false;
    do {
        cout << "Nhap ma sinh vien (8 so): ";
        getline(cin, sv.maSV);
        
        validMaSV = (sv.maSV.length() == 8);
        for(char c : sv.maSV) {
            if(!isdigit(c)) {
                validMaSV = false;
                break;
            }
        }
        
        if(!validMaSV) {
            cout << "Ma sinh vien phai la 8 chu so! Vui long nhap lai.\n";
        }
    } while(!validMaSV);
    
    cout << "Nhap ho va ten: ";
    getline(cin, sv.hoTen);
    sv.hoTen = formatName(sv.hoTen);
    
    // Nhập ngày sinh
    bool validDate = false;
    do {
        cout << "Nhap ngay sinh (dd mm yyyy): ";
        cin >> sv.ngaySinh.day >> sv.ngaySinh.month >> sv.ngaySinh.year;
        
        validDate = isValidDate(sv.ngaySinh);
        if(!validDate) {
            cout << "Ngay sinh khong hop le! Vui long nhap lai.\n";
        }
    } while(!validDate);
    
    // Nhập điểm
    bool validGPA = false;
    do {
        cout << "Nhap diem trung binh tich luy (0.0-10.0): ";
        cin >> sv.diemTBTL;
        
        validGPA = (sv.diemTBTL >= 0.0 && sv.diemTBTL <= 10.0);
        if(!validGPA) {
            cout << "Diem phai trong khoang 0.0-10.0! Vui long nhap lai.\n";
        }
    } while(!validGPA);
    
    students.push_back(sv);
    saveToFile(students); // Lưu tự động
    
    cout << "\nThem sinh vien thanh cong!\n";
}

// In danh sách sinh viên
void printStudents(const vector<Student>& students) {
    system("cls");
    cout << "==============================\n";
    cout << "    DANH SACH SINH VIEN      \n";
    cout << "==============================\n";
    
    if(students.empty()) {
        cout << "Danh sach sinh vien trong!\n";
        return;
    }
    
    cout << "Tong so sinh vien: " << students.size() << "\n\n";
    
    for(int i = 0; i < students.size(); i++) {
        const Student& sv = students[i];
        cout << "STT: " << (i + 1) << "\n";
        cout << "Ma lop: " << sv.maLop << "\n";
        cout << "Ma SV: " << sv.maSV << "\n";
        cout << "Ho ten: " << sv.hoTen << "\n";
        cout << "Ngay sinh: " << setfill('0') << setw(2) << sv.ngaySinh.day 
             << "/" << setw(2) << sv.ngaySinh.month 
             << "/" << sv.ngaySinh.year << "\n";
        cout << "Diem TB: " << fixed << setprecision(2) << sv.diemTBTL << "\n";
        cout << "------------------------------\n";
    }
}

// Sắp xếp danh sách
void sortStudents(vector<Student>& students) {
    if(students.empty()) {
        cout << "Danh sach sinh vien trong!\n";
        return;
    }
    
    system("cls");
    cout << "==============================\n";
    cout << "       SAP XEP DANH SACH     \n";
    cout << "==============================\n";
    cout << "1. Sap xep theo ma sinh vien\n";
    cout << "2. Sap xep theo ho ten\n";
    cout << "3. Sap xep theo ngay sinh\n";
    cout << "4. Sap xep theo diem TB\n";
    cout << "5. Sap xep theo ma lop\n";
    cout << "==============================\n";
    cout << "Chon tieu chi sap xep (1-5): ";
    
    int choice;
    cin >> choice;
    
    switch(choice) {
        case 1: // Mã SV
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.maSV < b.maSV;
                 });
            break;
        case 2: // Họ tên
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.hoTen < b.hoTen;
                 });
            break;
        case 3: // Ngày sinh
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     if(a.ngaySinh.year != b.ngaySinh.year)
                         return a.ngaySinh.year < b.ngaySinh.year;
                     if(a.ngaySinh.month != b.ngaySinh.month)
                         return a.ngaySinh.month < b.ngaySinh.month;
                     return a.ngaySinh.day < b.ngaySinh.day;
                 });
            break;
        case 4: // Điểm TB
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.diemTBTL > b.diemTBTL;
                 });
            break;
        case 5: // Mã lớp
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.maLop < b.maLop;
                 });
            break;
        default:
            cout << "Lua chon khong hop le!\n";
            return;
    }
    
    saveToFile(students); // Lưu sau khi sắp xếp
    cout << "\nSap xep thanh cong!\n";
}

// Tìm kiếm sinh viên
void searchStudents(const vector<Student>& students) {
    if(students.empty()) {
        cout << "Danh sach sinh vien trong!\n";
        return;
    }
    
    system("cls");
    cout << "==============================\n";
    cout << "      TIM KIEM SINH VIEN     \n";
    cout << "==============================\n";
    cout << "1. Tim theo ma sinh vien\n";
    cout << "2. Tim theo ho ten\n";
    cout << "3. Tim theo ma lop\n";
    cout << "4. Tim theo nam sinh\n";
    cout << "==============================\n";
    cout << "Chon tieu chi tim kiem (1-4): ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string keyword;
    cout << "Nhap tu khoa tim kiem: ";
    getline(cin, keyword);
    
    vector<Student> result;
    
    // Tìm kiếm tuần tự
    for(const auto& sv : students) {
        bool found = false;
        switch(choice) {
            case 1: // Mã SV
                if(sv.maSV.find(keyword) != string::npos) found = true;
                break;
            case 2: // Họ tên
                if(sv.hoTen.find(keyword) != string::npos) found = true;
                break;
            case 3: // Mã lớp
                if(sv.maLop.find(keyword) != string::npos) found = true;
                break;
            case 4: // Năm sinh
                if(to_string(sv.ngaySinh.year).find(keyword) != string::npos) found = true;
                break;
        }
        if(found) result.push_back(sv);
    }
    
    if(result.empty()) {
        cout << "\nKhong tim thay sinh vien nao!\n";
    } else {
        cout << "\nTim thay " << result.size() << " ket qua:\n\n";
        for(int i = 0; i < result.size(); i++) {
            const Student& sv = result[i];
            cout << "STT: " << (i + 1) << "\n";
            cout << "Ma lop: " << sv.maLop << "\n";
            cout << "Ma SV: " << sv.maSV << "\n";
            cout << "Ho ten: " << sv.hoTen << "\n";
            cout << "Ngay sinh: " << setfill('0') << setw(2) << sv.ngaySinh.day 
                 << "/" << setw(2) << sv.ngaySinh.month 
                 << "/" << sv.ngaySinh.year << "\n";
            cout << "Diem TB: " << fixed << setprecision(2) << sv.diemTBTL << "\n";
            cout << "------------------------------\n";
        }
    }
}

// Menu thống kê
void statisticsMenu(const vector<Student>& students) {
    if(students.empty()) {
        cout << "Danh sach sinh vien trong!\n";
        return;
    }
    
    system("cls");
    cout << "==============================\n";
    cout << "       THONG KE BAO CAO      \n";
    cout << "==============================\n";
    cout << "1. Bao cao so luong SV theo lop\n";
    cout << "2. Ty le phan loai ket qua hoc tap\n";
    cout << "==============================\n";
    cout << "Chon loai bao cao (1-2): ";
    
    int choice;
    cin >> choice;
    
    if(choice == 1) {
        // Báo cáo số lượng SV theo lớp
        map<string, int> classCount;
        for(const auto& sv : students) {
            classCount[sv.maLop]++;
        }
        
        cout << "\nBAO CAO SO LUONG SINH VIEN THEO LOP\n";
        cout << "=====================================\n";
        cout << "Tong so sinh vien: " << students.size() << "\n";
        cout << "So lop: " << classCount.size() << "\n\n";
        
        for(const auto& pair : classCount) {
            float percentage = (float)pair.second * 100 / students.size();
            cout << "Lop " << pair.first << ": " << pair.second 
                 << " SV (" << fixed << setprecision(1) << percentage << "%)\n";
        }
    }
    else if(choice == 2) {
        // Tỷ lệ phân loại kết quả học tập
        int xuatSac = 0, gioi = 0, kha = 0, trungBinh = 0, yeu = 0;
        
        for(const auto& sv : students) {
            if(sv.diemTBTL >= 9.0) xuatSac++;
            else if(sv.diemTBTL >= 8.0) gioi++;
            else if(sv.diemTBTL >= 6.5) kha++;
            else if(sv.diemTBTL >= 5.0) trungBinh++;
            else yeu++;
        }
        
        int total = students.size();
        cout << "\nBAO CAO TY LE PHAN LOAI KET QUA HOC TAP\n";
        cout << "=======================================\n";
        cout << "Tong so sinh vien: " << total << "\n\n";
        
        cout << "Xuat sac (>= 9.0): " << xuatSac 
             << " (" << fixed << setprecision(1) << (float)xuatSac*100/total << "%)\n";
        cout << "Gioi (8.0-8.9): " << gioi 
             << " (" << fixed << setprecision(1) << (float)gioi*100/total << "%)\n";
        cout << "Kha (6.5-7.9): " << kha 
             << " (" << fixed << setprecision(1) << (float)kha*100/total << "%)\n";
        cout << "Trung binh (5.0-6.4): " << trungBinh 
             << " (" << fixed << setprecision(1) << (float)trungBinh*100/total << "%)\n";
        cout << "Yeu (< 5.0): " << yeu 
             << " (" << fixed << setprecision(1) << (float)yeu*100/total << "%)\n";
    }
    else {
        cout << "Lua chon khong hop le!\n";
    }
}

// Lưu dữ liệu vào file
void saveToFile(const vector<Student>& students) {
    ofstream file("students_data.txt");
    if(!file) {
        cout << "Khong the mo file de ghi!\n";
        return;
    }
    
    file << students.size() << "\n";
    
    for(const auto& sv : students) {
        file << sv.maLop << "\n";
        file << sv.maSV << "\n";
        file << sv.hoTen << "\n";
        file << sv.ngaySinh.day << " " << sv.ngaySinh.month << " " << sv.ngaySinh.year << "\n";
        file << fixed << setprecision(2) << sv.diemTBTL << "\n";
    }
    
    file.close();
}

// Đọc dữ liệu từ file
void loadFromFile(vector<Student>& students) {
    ifstream file("students_data.txt");
    if(!file) {
        students.clear();
        return;
    }
    
    size_t size;
    file >> size;
    file.ignore();
    
    students.clear();
    students.reserve(size);
    
    for(size_t i = 0; i < size; i++) {
        Student sv;
        
        getline(file, sv.maLop);
        getline(file, sv.maSV);
        getline(file, sv.hoTen);
        
        file >> sv.ngaySinh.day >> sv.ngaySinh.month >> sv.ngaySinh.year;
        file >> sv.diemTBTL;
        file.ignore();
        
        students.push_back(sv);
    }
    
    file.close();
}

// Kiểm tra tính hợp lệ của ngày
bool isValidDate(const Date& date) {
    if(date.year < 1900 || date.year > 2024) return false;
    if(date.month < 1 || date.month > 12) return false;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Kiểm tra năm nhuận
    if(date.year % 4 == 0 && (date.year % 100 != 0 || date.year % 400 == 0)) {
        daysInMonth[2] = 29;
    }
    
    return (date.day >= 1 && date.day <= daysInMonth[date.month]);
}

// Chuẩn hóa tên
string formatName(string name) {
    if(name.empty()) return name;
    
    // Xóa khoảng trắng thừa
    size_t start = name.find_first_not_of(' ');
    size_t end = name.find_last_not_of(' ');
    if(start == string::npos) return "";
    name = name.substr(start, end - start + 1);
    
    // Viết hoa chữ cái đầu mỗi từ
    bool capitalizeNext = true;
    for(char& c : name) {
        if(c == ' ') {
            capitalizeNext = true;
        } else if(capitalizeNext) {
            c = toupper(c);
            capitalizeNext = false;
        } else {
            c = tolower(c);
        }
    }
    
    return name;
}