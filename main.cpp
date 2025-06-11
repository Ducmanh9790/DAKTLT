#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <cctype>
#include <map>
#include <conio.h>  // Để sử dụng getch() và kbhit()
#include <windows.h> // Để điều khiển màu sắc và console
#include <sstream>  // THÊM DÒNG NÀY ĐỂ SỬ DỤNG ostringstream
using namespace std;

// Màu sắc console
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

#ifndef BACKGROUND_LIGHTGRAY
#define BACKGROUND_LIGHTGRAY (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#endif

// Phím điều khiển
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27

// Thêm vào đầu file main.cpp
#define RECORDS_PER_PAGE 8

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

// Khai báo các hàm
void addStudent(vector<Student>& students);
void printStudents(const vector<Student>& students);
void statisticsMenu(const vector<Student>& students);
void saveToFile(const vector<Student>& students);
void loadFromFile(vector<Student>& students);
bool isValidDate(const Date& date);
bool isValidGPA(float gpa);
string formatName(string name);
void setColor(int color);
void gotoxy(int x, int y);
void clearScreen();
int showAdvancedMenu();
int showSortMenu();
int showSearchMenu();
void sortStudents(vector<Student>& students, int choice);
void searchStudents(const vector<Student>& students, int choice);
string inputTextBox(string prompt, int x, int y, int width, string defaultValue = "");
void drawTextBox(int x, int y, int width, string content, int cursorPos, bool active);
Date inputDateTextBox(string prompt, int x, int y);
// Thêm vào phần khai báo hàm (sau dòng 76):
void displaySearchResults(const vector<Student>& results, const string& keyword);
float inputFloatTextBox(string prompt, int x, int y, float min, float max);
void editStudent(vector<Student>& students);
void drawTableBorder(int x, int y, int width, int height);
void drawTableSeparator(int x, int y, int width);
void printTableHeader(int x, int y);
void printStudentRow(int x, int y, const Student& sv, bool highlight);
void showStudentDetail(const Student& sv);
void drawStatBar(string label, int count, int total, int x, int y, int color);

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    system("cls");
}

// Menu nâng cao với phím mũi tên
int showAdvancedMenu() {
    string menuItems[] = {
        "1. Them moi ho so sinh vien",
        "2. Sua thong tin sinh vien",
        "3. In danh sach sinh vien", 
        "4. Sap xep danh sach",
        "5. Tim kiem sinh vien",
        "6. Thong ke bao cao",
        "7. Thoat chuong trinh"
    };
    
    int menuSize = 7;
    int selected = 0;
    int key;
    
    while(true) {
        clearScreen();
        
        // Vẽ tiêu đề
        setColor(LIGHTCYAN);
        gotoxy(20, 2);
        cout << "========================================";
        gotoxy(20, 3);
        cout << "    CHUONG TRINH QUAN LY SINH VIEN     ";
        gotoxy(20, 4);
        cout << "========================================";
        
        setColor(YELLOW);
        gotoxy(25, 6);
        cout << "Su dung phim mui ten de chon, Enter de xac nhan";
        
        // Vẽ menu
        for(int i = 0; i < menuSize; i++) {
            gotoxy(25, 8 + i);
            
            if(i == selected) {
                setColor(BLACK);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                      BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                cout << ">> " << menuItems[i] << " <<";
            } else {
                setColor(LIGHTGRAY);
                cout << "   " << menuItems[i] << "   ";
            }
        }
        
        setColor(LIGHTGREEN);  
        gotoxy(25, 16);
        cout << "Chuc nang duoc chon: " << (selected + 1);
        
        // Đọc phím
        key = _getch();
        
        if(key == 224) { // Phím đặc biệt
            key = _getch();
            switch(key) {
                case KEY_UP:
                    selected = (selected - 1 + menuSize) % menuSize;
                    break;
                case KEY_DOWN:
                    selected = (selected + 1) % menuSize;
                    break;
            }
        } else if(key == KEY_ENTER) {
            setColor(LIGHTGRAY);
            return selected + 1;
        } else if(key == KEY_ESC) {
            return 7; // Thoát
        }
    }
}

// Menu con cho sắp xếp (xổ ngang)
int showSortMenu() {
    string sortOptions[] = {"Ma SV", "Ho Ten", "Ngay Sinh", "Diem TB", "Ma Lop"};
    string algorithms[] = {"Selection", "Bubble", "Insertion", "Quick"};
    
    int selected = 0;
    int key;
    
    while(true) {
        clearScreen();
        setColor(LIGHTCYAN);
        gotoxy(25, 3);
        cout << "=== MENU SAP XEP ===";
        
        setColor(YELLOW);
        gotoxy(15, 5);
        cout << "Chon tieu chi sap xep (Phim mui ten trai/phai):";
        
        // Menu ngang cho tiêu chí
        for(int i = 0; i < 5; i++) {
            gotoxy(10 + i * 12, 7);
            if(i == selected) {
                setColor(BLACK);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                      BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                cout << sortOptions[i];
            } else {
                setColor(LIGHTGRAY);
                cout << sortOptions[i];
            }
        }
        
        setColor(LIGHTGREEN);
        gotoxy(20, 10);
        cout << "Nhan Enter de chon, ESC de quay lai";
        
        key = _getch();
        
        if(key == 224) {
            key = _getch();
            switch(key) {
                case KEY_LEFT:
                    selected = (selected - 1 + 5) % 5;
                    break;
                case KEY_RIGHT:
                    selected = (selected + 1) % 5;
                    break;
            }
        } else if(key == KEY_ENTER) {
            // Chọn thuật toán
            int algSelected = 0;
            while(true) {
                clearScreen();
                setColor(LIGHTCYAN);
                gotoxy(25, 3);
                cout << "=== CHON THUAT TOAN ===";
                
                setColor(YELLOW);
                gotoxy(15, 5);
                cout << "Da chon: " << sortOptions[selected];
                gotoxy(15, 6);
                cout << "Chon thuat toan sap xep:";
                
                for(int i = 0; i < 4; i++) {
                    gotoxy(20, 8 + i);
                    if(i == algSelected) {
                        setColor(BLACK);
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                              BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                        cout << ">> " << algorithms[i] << " Sort <<";
                    } else {
                        setColor(LIGHTGRAY);
                        cout << "   " << algorithms[i] << " Sort   ";
                    }
                }
                
                key = _getch();
                if(key == 224) {
                    key = _getch();
                    switch(key) {
                        case KEY_UP:
                            algSelected = (algSelected - 1 + 4) % 4;
                            break;
                        case KEY_DOWN:
                            algSelected = (algSelected + 1) % 4;
                            break;
                    }
                } else if(key == KEY_ENTER) {
                    setColor(LIGHTGRAY);
                    return selected + 1; // Trả về tiêu chí được chọn
                } else if(key == KEY_ESC) {
                    break;
                }
            }
        } else if(key == KEY_ESC) {
            return 0;
        }
    }
    return 0;
}

// Menu con cho tìm kiếm
int showSearchMenu() {
    string searchOptions[] = {"Ma sinh vien", "Ho va ten", "Ma lop"};
    int selected = 0;
    int key;
    
    while(true) {
        clearScreen();
        setColor(LIGHTCYAN);
        gotoxy(25, 3);
        cout << "=== MENU TIM KIEM ===";
        
        for(int i = 0; i < 3; i++) {
            gotoxy(25, 6 + i);
            if(i == selected) {
                setColor(BLACK);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                      BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                cout << ">> " << searchOptions[i] << " <<";
            } else {
                setColor(LIGHTGRAY);
                cout << "   " << searchOptions[i] << "   ";
            }
        }
        
        key = _getch();
        if(key == 224) {
            key = _getch();
            switch(key) {
                case KEY_UP:
                    selected = (selected - 1 + 3) % 3;
                    break;
                case KEY_DOWN:
                    selected = (selected + 1) % 3;
                    break;
            }
        } else if(key == KEY_ENTER) {
            setColor(LIGHTGRAY);
            return selected + 1;
        } else if(key == KEY_ESC) {
            return 0;
        }
    }
}

// Cập nhật hàm main để sử dụng menu nâng cao
int main() {
    // Thiết lập console để hiển thị Unicode
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    vector<Student> students;
    int choice;
    
    // Load dữ liệu từ file
    loadFromFile(students);
    
    do {
        choice = showAdvancedMenu();
        
        switch(choice) {
            case 1:
                addStudent(students);
                break;
            case 2:
                editStudent(students);
                break;
            case 3:
                printStudents(students);
                break;
            case 4: {
                int sortChoice = showSortMenu();
                if(sortChoice > 0) {
                    // Thực hiện sắp xếp dựa trên lựa chọn
                    sortStudents(students, sortChoice);
                }
                break;
            }
            case 5: {
                int searchChoice = showSearchMenu();
                if(searchChoice > 0) {
                    searchStudents(students, searchChoice);
                }
                break;
            }
            case 6:
                statisticsMenu(students);
                break;
            case 7:
                saveToFile(students);
                setColor(LIGHTGREEN);
                clearScreen();
                gotoxy(30, 10);
                cout << "Cam on ban da su dung chuong trinh!";
                gotoxy(30, 11);
                cout << "Nhan phim bat ky de thoat...";
                _getch();
                break;
            default:
                setColor(LIGHTRED);
                gotoxy(30, 10);
                cout << "Lua chon khong hop le!";
                _getch();
        }
        
        if(choice != 7) {
            setColor(YELLOW);
            gotoxy(25, 20);
            cout << "Nhan phim bat ky de tiep tuc...";
            _getch();
        }
    } while(choice != 7);
    
    setColor(LIGHTGRAY);
    return 0;
}

// Hàm vẽ textbox liền mạch với Unicode box drawing
void drawTextBox(int x, int y, int width, string content, int cursorPos, bool active) {
    // Thiết lập console để hỗ trợ Unicode
    SetConsoleOutputCP(CP_UTF8);
    
    setColor(active ? YELLOW : LIGHTGRAY);
    
    // Dòng trên với các góc bo tròn
    gotoxy(x, y);
    cout << "┌";  // Top-left corner
    for(int i = 0; i < width; i++) cout << "─";  // Horizontal line
    cout << "┐";  // Top-right corner
    
    // Dòng giữa với nội dung
    gotoxy(x, y + 1);
    cout << "│";  // Vertical line
    
    // Hiển thị nội dung
    setColor(active ? WHITE : LIGHTGRAY);
    string displayText = content;
    if(displayText.length() > width) {
        displayText = displayText.substr(0, width);
    }
    cout << left << setw(width) << displayText;
    
    setColor(active ? YELLOW : LIGHTGRAY);
    cout << "│";  // Vertical line
    
    // Dòng dưới
    gotoxy(x, y + 2);
    cout << "└";  // Bottom-left corner
    for(int i = 0; i < width; i++) cout << "─";  // Horizontal line
    cout << "┘";  // Bottom-right corner
    
    // Vẽ cursor nếu textbox đang active
    if(active && cursorPos <= displayText.length()) {
        gotoxy(x + 1 + cursorPos, y + 1);
        setColor(BLACK);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                      BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
        if(cursorPos < displayText.length()) {
            cout << displayText[cursorPos];
        } else {
            cout << " ";
        }
    }
}

// Hàm nhập text với textbox
string inputTextBox(string prompt, int x, int y, int width, string defaultValue) {
    string content = defaultValue;
    int cursorPos = content.length();
    int key;
    bool active = true;
    
    // Hiển thị prompt
    setColor(LIGHTCYAN);
    gotoxy(x, y - 1);
    cout << prompt;
    
    while(true) {
        drawTextBox(x, y, width, content, cursorPos, active);
        
        key = _getch();
        
        if(key == KEY_ENTER) {
            break;
        } else if(key == KEY_ESC) {
            content = defaultValue;
            break;
        } else if(key == 8) { // Backspace
            if(cursorPos > 0) {
                content.erase(cursorPos - 1, 1);
                cursorPos--;
            }
        } else if(key == 224) { // Phím đặc biệt
            key = _getch();
            switch(key) {
                case KEY_LEFT:
                    if(cursorPos > 0) cursorPos--;
                    break;
                case KEY_RIGHT:
                    if(cursorPos < content.length()) cursorPos++;
                    break;
                case 71: // Home
                    cursorPos = 0;
                    break;
                case 79: // End
                    cursorPos = content.length();
                    break;
                case 83: // Delete
                    if(cursorPos < content.length()) {
                        content.erase(cursorPos, 1);
                    }
                    break;
            }
        } else if(key >= 32 && key <= 126) { // Ký tự có thể in
            if(content.length() < width - 2) {
                content.insert(cursorPos, 1, (char)key);
                cursorPos++;
            }
        }
    }
    
    setColor(LIGHTGRAY);
    return content;
}

// Hàm nhập ngày với textbox
Date inputDateTextBox(string prompt, int x, int y) {
    Date date = {1, 1, 2000};
    string dateStr = "";  // Bắt đầu với chuỗi rỗng
    bool validDate = false;
    
    do {
        dateStr = inputTextBox(prompt + " (dd/mm/yyyy):", x, y, 20, dateStr);
        
        // Parse ngày từ string
        if(sscanf(dateStr.c_str(), "%d/%d/%d", &date.day, &date.month, &date.year) == 3) {
            validDate = isValidDate(date);
        } else {
            validDate = false;
        }
        
        if(!validDate) {
            setColor(LIGHTRED);
            gotoxy(x, y + 4);
            cout << "Ngay khong hop le! Nhan Enter de nhap lai...";
            _getch();
            gotoxy(x, y + 4);
            cout << string(50, ' '); // Xóa thông báo lỗi
        }
    } while(!validDate);
    
    return date;
}

// Sửa lại hàm inputFloatTextBox
float inputFloatTextBox(string prompt, int x, int y, float min, float max) {
    float value = 0.0;
    string valueStr = "";  // Bắt đầu với chuỗi rỗng
    bool validValue = false;
    
    do {
        valueStr = inputTextBox(prompt + " (" + to_string(min) + "-" + to_string(max) + "):", 
                               x, y, 15, valueStr);
        
        try {
            value = stof(valueStr);
            validValue = (value >= min && value <= max);
        } catch(...) {
            validValue = false;
        }
        
        if(!validValue) {
            setColor(LIGHTRED);
            gotoxy(x, y + 4);
            cout << "Gia tri khong hop le! Nhan Enter de nhap lai...";
            _getch();
            gotoxy(x, y + 4);
            cout << string(50, ' '); // Xóa thông báo lỗi
        }
    } while(!validValue);
    
    return value;
}

// Cập nhật hàm addStudent với textbox nâng cao
void addStudent(vector<Student>& students) {
    Student sv;
    
    clearScreen();
    
    // Vẽ tiêu đề
    setColor(LIGHTCYAN);
    gotoxy(25, 2);
    cout << "==============================";
    gotoxy(25, 3);
    cout << "    THEM MOI SINH VIEN       ";
    gotoxy(25, 4);
    cout << "==============================";
    
    // Hướng dẫn sử dụng
    setColor(YELLOW);
    gotoxy(15, 6);
    cout << "Su dung phim mui ten, Home/End, Delete/Backspace de chinh sua";
    gotoxy(15, 7);
    cout << "Nhan Enter de xac nhan, ESC de huy";
    
    // Nhập dữ liệu với textbox
    sv.maLop = inputTextBox("Ma lop:", 20, 10, 25);
    if(sv.maLop.empty()) return;
    
    // Kiểm tra mã sinh viên (8 số)
    bool validMaSV = false;
    do {
        sv.maSV = inputTextBox("Ma sinh vien (8 so):", 20, 14, 25, sv.maSV);
        if(sv.maSV.empty()) return;
        
        validMaSV = (sv.maSV.length() == 8 && 
                    all_of(sv.maSV.begin(), sv.maSV.end(), ::isdigit));
        
        if(!validMaSV) {
            setColor(LIGHTRED);
            gotoxy(20, 18);
            cout << "Ma sinh vien phai la 8 chu so! Nhan Enter de nhap lai...";
            _getch();
            gotoxy(20, 18);
            cout << string(55, ' ');
        }
    } while(!validMaSV);
    
    sv.hoTen = inputTextBox("Ho va ten:", 20, 18, 35);
    if(sv.hoTen.empty()) return;
    sv.hoTen = formatName(sv.hoTen);
    
    // Nhập ngày sinh với validation
    sv.ngaySinh = inputDateTextBox("Ngay sinh", 20, 22);
    
    // Nhập điểm với validation
    sv.diemTBTL = inputFloatTextBox("Diem trung binh tich luy", 20, 27, 0.0, 10.0);
    
    students.push_back(sv);
    
    // LƯU TỰ ĐỘNG VÀO FILE
    saveToFile(students);
    
    // Thông báo thành công
    setColor(LIGHTGREEN);
    gotoxy(30, 40);
    cout << "THEM SINH VIEN THANH CONG!";
    gotoxy(30, 41);
    cout << "Du lieu da duoc luu tu dong!";
    gotoxy(30, 42);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

// Hàm sửa thông tin sinh viên
void editStudent(vector<Student>& students) {
    if(students.empty()) {
        setColor(LIGHTRED);
        clearScreen();
        gotoxy(30, 10);
        cout << "Danh sach sinh vien trong!";
        gotoxy(30, 11);
        cout << "Nhan phim bat ky de quay lai...";
        _getch();
        return;
    }
    
    clearScreen();
    setColor(LIGHTCYAN);
    gotoxy(25, 2);
    cout << "==============================";
    gotoxy(25, 3);
    cout << "    SUA THONG TIN SINH VIEN   ";
    gotoxy(25, 4);
    cout << "==============================";
    
    // Hiển thị danh sách sinh viên
    setColor(LIGHTGRAY);
    gotoxy(5, 6);
    cout << left << setw(5) << "STT" << setw(12) << "Ma SV" << setw(25) << "Ho Ten" << setw(12) << "Ma Lop";
    
    for(int i = 0; i < students.size() && i < 10; i++) { // Chỉ hiển thị 10 sinh viên đầu
        gotoxy(5, 7 + i);
        cout << left << setw(5) << (i + 1) 
             << setw(12) << students[i].maSV.substr(0, 11)    // Giới hạn độ dài
             << setw(25) << students[i].hoTen.substr(0, 24)   // Giới hạn độ dài
             << setw(12) << students[i].maLop.substr(0, 11);  // Giới hạn độ dài
    }
    
    // Chọn sinh viên cần sửa
    string indexStr = inputTextBox("Chon so thu tu sinh vien can sua:", 20, 18, 5, ""); 
    
    try {
        int index = stoi(indexStr) - 1;
        if(index >= 0 && index < students.size()) {
            Student& sv = students[index];
            
            clearScreen();
            setColor(LIGHTCYAN);
            gotoxy(25, 2);
            cout << "SUA THONG TIN: " << sv.hoTen;
            
            // Sửa từng trường với giá trị mặc định là giá trị hiện tại
            sv.maLop = inputTextBox("Ma lop:", 20, 6, 25, sv.maLop);
            sv.maSV = inputTextBox("Ma sinh vien:", 20, 10, 25, sv.maSV);
            sv.hoTen = inputTextBox("Ho va ten:", 20, 14, 35, sv.hoTen);
            sv.hoTen = formatName(sv.hoTen);
            
            // Ngày sinh
            char dateBuffer[20];
            sprintf(dateBuffer, "%02d/%02d/%04d", sv.ngaySinh.day, sv.ngaySinh.month, sv.ngaySinh.year);
            sv.ngaySinh = inputDateTextBox("Ngay sinh", 20, 18);
            
            // Điểm
            sv.diemTBTL = inputFloatTextBox("Diem TB", 20, 23, 0.0, 10.0);
            
            setColor(LIGHTGREEN);
            gotoxy(25, 28);
            cout << "CAP NHAT THANH CONG!";
        } else {
            setColor(LIGHTRED);
            gotoxy(25, 28);
            cout << "So thu tu khong hop le!";
        }
    } catch(...) {
        setColor(LIGHTRED);
        gotoxy(25, 28);
        cout << "Vui long nhap so hop le!";
    }
    
    gotoxy(25, 29);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

// Sửa lại hàm printStudents với bảng cao hơn
void printStudents(const vector<Student>& students) {
    if(students.empty()) {
        clearScreen();
        setColor(LIGHTRED);
        gotoxy(30, 10);
        cout << "DANH SACH SINH VIEN TRONG!";
        gotoxy(30, 11);
        cout << "Nhan phim bat ky de quay lai...";
        _getch();
        return;
    }
    
    int currentPage = 0;
    int totalPages = (students.size() + RECORDS_PER_PAGE - 1) / RECORDS_PER_PAGE;
    int selectedRow = 0;
    int key;
    
    while(true) {
        clearScreen();
        
        // Vẽ tiêu đề căn giữa
        setColor(LIGHTCYAN);
        gotoxy(15, 1);
        cout << "══════════════════════════════════════════════════════════════════════";
        gotoxy(15, 2);
        cout << "                          DANH SACH SINH VIEN                        ";
        gotoxy(15, 3);
        cout << "══════════════════════════════════════════════════════════════════════";
        
        // Thông tin phân trang căn giữa
        setColor(YELLOW);
        gotoxy(30, 4);
        cout << "Trang " << (currentPage + 1) << "/" << totalPages 
             << " - Tong so: " << students.size() << " sinh vien";
        
        // Vẽ bảng cao hơn để chứa đủ dữ liệu
        int tableX = 8;
        int tableY = 6;
        int tableWidth = 80;
        int tableHeight = RECORDS_PER_PAGE + 4; // TĂNG CHIỀU CAO TỪ +3 LÊN +4
        
        drawTableBorder(tableX, tableY, tableWidth, tableHeight);
        
        // Header bảng
        printTableHeader(tableX, tableY + 1);
        drawTableSeparator(tableX, tableY + 2, tableWidth);
        
        // Dữ liệu
        int startIndex = currentPage * RECORDS_PER_PAGE;
        int endIndex = min(startIndex + RECORDS_PER_PAGE, (int)students.size());
        
        for(int i = startIndex; i < endIndex; i++) {
            int rowIndex = i - startIndex;
            bool highlight = (rowIndex == selectedRow);
            printStudentRow(tableX, tableY + 3 + rowIndex, students[i], highlight);
        }
        
        // Hướng dẫn sử dụng DỊCH CHUYỂN XUỐNG ĐỂ TRÁNH BỊ CHE
        setColor(LIGHTGREEN);
        gotoxy(12, tableY + tableHeight + 1);
        cout << "↑↓: Di chuyển  │  ←→: Chuyển trang  │  Enter: Xem chi tiet  │  ESC: Thoat";
        
        // Thanh trạng thái DỊCH CHUYỂN XUỐNG
        setColor(LIGHTMAGENTA);
        gotoxy(15, tableY + tableHeight + 3);
        if(selectedRow < endIndex - startIndex) {
            const Student& selected = students[startIndex + selectedRow];
            cout << "Chi tiet: " << selected.hoTen.substr(0, 20) << " - " 
                 << selected.maSV << " - Lop: " << selected.maLop;
        }
        
        // Xử lý phím (giữ nguyên)
        key = _getch();
        
        if(key == 224) {
            key = _getch();
            switch(key) {
                case KEY_UP:
                    if(selectedRow > 0) {
                        selectedRow--;
                    } else if(currentPage > 0) {
                        currentPage--;
                        selectedRow = min(RECORDS_PER_PAGE - 1, 
                                        (int)students.size() - currentPage * RECORDS_PER_PAGE - 1);
                    }
                    break;
                case KEY_DOWN:
                    if(selectedRow < min(RECORDS_PER_PAGE - 1, endIndex - startIndex - 1)) {
                        selectedRow++;
                    } else if(currentPage < totalPages - 1) {
                        currentPage++;
                        selectedRow = 0;
                    }
                    break;
                case KEY_LEFT:
                    if(currentPage > 0) {
                        currentPage--;
                        selectedRow = 0;
                    }
                    break;
                case KEY_RIGHT:
                    if(currentPage < totalPages - 1) {
                        currentPage++;
                        selectedRow = 0;
                    }
                    break;
            }
        } else if(key == KEY_ENTER) {
            showStudentDetail(students[startIndex + selectedRow]);
        } else if(key == KEY_ESC) {
            break;
        }
    }
    
    setColor(LIGHTGRAY);
}

// Cập nhật hàm statisticsMenu với giao diện nâng cao
void statisticsMenu(const vector<Student>& students) {
    if(students.empty()) {
        clearScreen();
        setColor(LIGHTRED);
        gotoxy(30, 10);
        cout << "KHONG CO DU LIEU DE THONG KE!";
        gotoxy(30, 11);
        cout << "Nhan phim bat ky de quay lai...";
        _getch();
        return;
    }
    
    clearScreen();
    
    setColor(LIGHTCYAN);
    gotoxy(25, 1);
    cout << "══════════════════════════════════════════";
    gotoxy(25, 2);
    cout << "           THONG KE BAO CAO               ";
    gotoxy(25, 3);
    cout << "══════════════════════════════════════════";
    
    // Thống kê theo lớp
    map<string, int> classList;
    for(const auto& sv : students) {
        classList[sv.maLop]++;
    }
    
    setColor(YELLOW);
    gotoxy(15, 5);
    cout << "THONG KE SO LUONG SINH VIEN THEO LOP:";
    
    int y = 7;
    setColor(WHITE);
    for(const auto& pair : classList) {
        gotoxy(20, y);
        cout << "Lop " << left << setw(10) << pair.first << ": " 
             << right << setw(3) << pair.second << " sinh vien";
        y++;
    }
    
    // Vẽ biểu đồ phần trăm xếp loại
    y += 2;
    setColor(YELLOW);
    gotoxy(15, y);
    cout << "THONG KE XEP LOAI HOC TAP:";
    y += 2;
    
    int xuatSac = 0, gioi = 0, kha = 0, trungBinh = 0, yeu = 0;
    
    for(const auto& sv : students) {
        if(sv.diemTBTL >= 9.0) xuatSac++;
        else if(sv.diemTBTL >= 8.0) gioi++;
        else if(sv.diemTBTL >= 6.5) kha++;
        else if(sv.diemTBTL >= 5.0) trungBinh++;
        else yeu++;
    }
    
    // Vẽ thanh biểu đồ
    drawStatBar("Xuat sac (>=9.0)", xuatSac, students.size(), 20, y, LIGHTGREEN);
    drawStatBar("Gioi (8.0-8.9)", gioi, students.size(), 20, y + 2, GREEN);
    drawStatBar("Kha (6.5-7.9)", kha, students.size(), 20, y + 4, YELLOW);
    drawStatBar("TB (5.0-6.4)", trungBinh, students.size(), 20, y + 6, LIGHTGRAY);
    drawStatBar("Yeu (<5.0)", yeu, students.size(), 20, y + 8, LIGHTRED);
    
    setColor(LIGHTGREEN);
    gotoxy(30, y + 12);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

// Hàm vẽ thanh biểu đồ thống kê
void drawStatBar(string label, int count, int total, int x, int y, int color) {
    setColor(WHITE);
    gotoxy(x, y);
    cout << left << setw(15) << label << ": " << right << setw(3) << count 
         << " (" << fixed << setprecision(1) << (float)count*100/total << "%)";
    
    setColor(color);
    gotoxy(x + 35, y);
    int barLength = (count * 20) / total;
    for(int i = 0; i < barLength; i++) {
        cout << "█";
    }
    cout << " " << count;
}

// Sửa lại hàm saveToFile - sử dụng text format thay vì binary
void saveToFile(const vector<Student>& students) {
    ofstream file("students.txt");  // Đổi từ .dat sang .txt
    if(!file) {
        setColor(LIGHTRED);
        gotoxy(30, 10);
        cout << "Khong the mo file de ghi!";
        return;
    }
    
    file << students.size() << endl;
    
    for(const auto& sv : students) {
        file << sv.maLop << endl;
        file << sv.maSV << endl;
        file << sv.hoTen << endl;
        file << sv.ngaySinh.day << " " << sv.ngaySinh.month << " " << sv.ngaySinh.year << endl;
        file << fixed << setprecision(2) << sv.diemTBTL << endl;
    }
    
    file.close();
}

// Sửa lại hàm loadFromFile - đọc text format
void loadFromFile(vector<Student>& students) {
    ifstream file("students.txt");  // Đổi từ .dat sang .txt
    if(!file) {
        students.clear();
        return;
    }
    
    size_t size;
    file >> size;
    file.ignore(); // Bỏ qua ký tự newline
    
    students.clear();
    students.reserve(size);
    
    for(size_t i = 0; i < size; i++) {
        Student sv;
        
        getline(file, sv.maLop);
        getline(file, sv.maSV);
        getline(file, sv.hoTen);
        
        file >> sv.ngaySinh.day >> sv.ngaySinh.month >> sv.ngaySinh.year;
        file >> sv.diemTBTL;
        file.ignore(); // Bỏ qua ký tự newline
        
        students.push_back(sv);
    }
    
    file.close();
}

// Kiểm tra tính hợp lệ của ngày
bool isValidDate(const Date& date) {
    if(date.year < 1900 || date.year > 2025) return false;
    if(date.month < 1 || date.month > 12) return false;
    if(date.day < 1 || date.day > 31) return false;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Kiểm tra năm nhuận
    if((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0)) {
        daysInMonth[2] = 29;
    }
    
    return date.day <= daysInMonth[date.month];
}

// Kiểm tra tính hợp lệ của điểm
bool isValidGPA(float gpa) {
    return gpa >= 0.0 && gpa <= 10.0;
}

// Chuẩn hóa tên
string formatName(string name) {
    // Chuyển về chữ thường
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    
    // Viết hoa chữ cái đầu mỗi từ
    bool capitalize = true;
    for(char& c : name) {
        if(isspace(c)) {
            capitalize = true;
        } else if(capitalize) {
            c = toupper(c);
            capitalize = false;
        }
    }
    
    return name;
}

// Sắp xếp sinh viên với thông báo thành công
void sortStudents(vector<Student>& students, int choice) {
    switch(choice) {
        case 1: // Sắp xếp theo mã SV
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.maSV < b.maSV;
                 });
            break;
        case 2: // Sắp xếp theo họ tên
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.hoTen < b.hoTen;
                 });
            break;
        case 3: // Sắp xếp theo ngày sinh
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     if(a.ngaySinh.year != b.ngaySinh.year)
                         return a.ngaySinh.year < b.ngaySinh.year;
                     if(a.ngaySinh.month != b.ngaySinh.month)
                         return a.ngaySinh.month < b.ngaySinh.month;
                     return a.ngaySinh.day < b.ngaySinh.day;
                 });
            break;
        case 4: // Sắp xếp theo điểm TB
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.diemTBTL > b.diemTBTL;
                 });
            break;
        case 5: // Sắp xếp theo mã lớp
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.maLop < b.maLop;
                 });
            break;
        default:
            clearScreen();
            setColor(LIGHTRED);
            gotoxy(30, 10);
            cout << "Lua chon khong hop le!";
            gotoxy(30, 11);
            cout << "Nhan phim bat ky de tiep tuc...";
            _getch();
            return;
    }
    
    // LƯU TỰ ĐỘNG SAU KHI SẮP XẾP
    saveToFile(students);
    
    clearScreen();
    setColor(LIGHTGREEN);
    gotoxy(30, 10);
    cout << "SAP XEP THANH CONG!";
    gotoxy(30, 11);
    cout << "Du lieu da duoc luu tu dong!";
    gotoxy(30, 12);
    cout << "Nhan phim bat ky de xem ket qua...";
    _getch();
    
    printStudents(students);
}

// Cập nhật hàm searchStudents với giao diện nâng cao
void searchStudents(const vector<Student>& students, int choice) {
    string keyword;
    
    clearScreen();
    setColor(LIGHTCYAN);
    gotoxy(25, 3);
    cout << "═══ TIM KIEM SINH VIEN ═══";
    
    setColor(YELLOW);
    gotoxy(15, 5);
    cout << "Nhap tu khoa tim kiem: ";
    
    cin.ignore();
    setColor(WHITE);
    getline(cin, keyword);
    
    vector<Student> result;
    
    for(const auto& sv : students) {
        bool found = false;
        switch(choice) {
            case 1:
                if(sv.maSV.find(keyword) != string::npos) found = true;
                break;
            case 2:
                if(sv.hoTen.find(keyword) != string::npos) found = true;
                break;
            case 3:
                if(sv.maLop.find(keyword) != string::npos) found = true;
                break;
        }
        if(found) result.push_back(sv);
    }
    
    if(result.empty()) {
        setColor(LIGHTRED);
        gotoxy(25, 10);
        cout << "KHONG TIM THAY SINH VIEN NAO!";
        gotoxy(25, 11);
        cout << "Nhan phim bat ky de tiep tuc...";
        _getch();
    } else {
        // Hiển thị kết quả với phân trang
        displaySearchResults(result, keyword);
    }
}

// Sửa lại displaySearchResults với chiều cao bảng tăng
void displaySearchResults(const vector<Student>& results, const string& keyword) {
    int currentPage = 0;
    int totalPages = (results.size() + RECORDS_PER_PAGE - 1) / RECORDS_PER_PAGE;
    int selectedRow = 0;
    int key;
    
    while(true) {
        clearScreen();
        
        setColor(LIGHTCYAN);
        gotoxy(10, 1);
        cout << "══════════════════════════════════════════════════════════════════════════";
        gotoxy(10, 2);
        cout << "                    KET QUA TIM KIEM: \"" << keyword.substr(0, 20) << "\"";
        gotoxy(10, 3);
        cout << "══════════════════════════════════════════════════════════════════════════";
        
        setColor(YELLOW);
        gotoxy(25, 4);
        cout << "Tim thay " << results.size() << " ket qua - Trang " 
             << (currentPage + 1) << "/" << totalPages;
        
        // Vẽ bảng với CHIỀU CAO TĂNG
        int tableX = 8;
        int tableY = 6;
        int tableWidth = 80;
        int tableHeight = RECORDS_PER_PAGE + 4; // TĂNG TỪ +3 LÊN +4
        
        drawTableBorder(tableX, tableY, tableWidth, tableHeight);
        printTableHeader(tableX, tableY + 1);
        drawTableSeparator(tableX, tableY + 2, tableWidth);
        
        int startIndex = currentPage * RECORDS_PER_PAGE;
        int endIndex = min(startIndex + RECORDS_PER_PAGE, (int)results.size());
        
        for(int i = startIndex; i < endIndex; i++) {
            int rowIndex = i - startIndex;
            bool highlight = (rowIndex == selectedRow);
            printStudentRow(tableX, tableY + 3 + rowIndex, results[i], highlight);
        }
        
        setColor(LIGHTGREEN);
        gotoxy(12, tableY + tableHeight + 1);
        cout << "↑↓: Di chuyển  │  ←→: Chuyển trang  │  Enter: Chi tiet  │  ESC: Thoat";
        
        // ... phần xử lý phím giữ nguyên
        key = _getch();
        
        if(key == 224) {
            key = _getch();
            switch(key) {
                case KEY_UP:
                    if(selectedRow > 0) selectedRow--;
                    else if(currentPage > 0) {
                        currentPage--;
                        selectedRow = min(RECORDS_PER_PAGE - 1, endIndex - startIndex - 1);
                    }
                    break;
                case KEY_DOWN:
                    if(selectedRow < min(RECORDS_PER_PAGE - 1, endIndex - startIndex - 1)) {
                        selectedRow++;
                    } else if(currentPage < totalPages - 1) {
                        currentPage++;
                        selectedRow = 0;
                    }
                    break;
                case KEY_LEFT:
                    if(currentPage > 0) {
                        currentPage--;
                        selectedRow = 0;
                    }
                    break;
                case KEY_RIGHT:
                    if(currentPage < totalPages - 1) {
                        currentPage++;
                        selectedRow = 0;
                    }
                    break;
            }
        } else if(key == KEY_ENTER) {
            showStudentDetail(results[startIndex + selectedRow]);
        } else if(key == KEY_ESC) {
            break;
        }
    }
}

// Sửa lại hàm drawTableBorder để đảm bảo chiều cao chính xác
void drawTableBorder(int x, int y, int width, int height) {
    setColor(LIGHTCYAN);
    
    // Dòng trên
    gotoxy(x, y);
    cout << "┌";
    for(int i = 0; i < 76; i++) cout << "─";
    cout << "┐";
    
    // Các dòng giữa - ĐẢM BẢO VẼ ĐỦ CHIỀU CAO
    for(int i = 1; i < height - 1; i++) {
        gotoxy(x, y + i);
        cout << "│";
        // Xóa nội dung cũ trong dòng
        for(int j = 0; j < 76; j++) cout << " ";
        gotoxy(x + 77, y + i);
        cout << "│";
    }
    
    // Dòng dưới
    gotoxy(x, y + height - 1);
    cout << "└";
    for(int i = 0; i < 76; i++) cout << "─";
    cout << "┘";
}

// Hàm vẽ dòng phân cách trong bảng
void drawTableSeparator(int x, int y, int width) {
    setColor(LIGHTCYAN);
    gotoxy(x, y);
    cout << "├";
    for(int i = 0; i < 76; i++) cout << "─";  // CỐ ĐỊNH CHIỀU RỘNG
    cout << "┤";
}

// Sửa lại header bảng với căn giữa các cột
void printTableHeader(int x, int y) {
    // Hàm helper để căn giữa text cho header
    auto centerHeaderText = [](string text, int width) -> string {
        if(text.length() >= width) return text.substr(0, width);
        int padding = (width - text.length()) / 2;
        int rightPadding = width - text.length() - padding;
        return string(padding, ' ') + text + string(rightPadding, ' ');
    };
    
    setColor(YELLOW);
    gotoxy(x+1, y);
    cout << centerHeaderText("Ma Lop", 8) << "│"     // 8 + 1 = 9
         << centerHeaderText("Ma SV", 14) << "│"     // 10 + 1 = 11  
         << centerHeaderText("Ho Ten", 24) << "│"      //24
         << centerHeaderText("Ngay Sinh", 14) << "│" // 10 + 1 = 11
         << centerHeaderText("Diem TB", 12);          // 8 = 8
    // Tổng: 9 + 11 + 21 + 11 + 8 = 60 (còn lại 16 ký tự cho khoảng trắng)
}

// Sửa lại hàm printStudentRow để căn giữa các cột
void printStudentRow(int x, int y, const Student& sv, bool highlight) {
    // XÁC ĐỊNH MÀU DỰA TRÊN ĐIỂM TB
    int rowColor;
    if(sv.diemTBTL >= 8.0) rowColor = LIGHTGREEN;
    else if(sv.diemTBTL >= 6.5) rowColor = YELLOW;
    else if(sv.diemTBTL >= 5.0) rowColor = LIGHTGRAY;
    else rowColor = LIGHTRED;
    
    gotoxy(x+1, y);
    
    // Giới hạn độ dài dữ liệu để không tràn
    string displayMaLop = sv.maLop.length() > 8 ? sv.maLop.substr(0, 8) : sv.maLop;
    string displayMaSV = sv.maSV.length() > 14 ? sv.maSV.substr(0, 14) : sv.maSV;
    string displayHoTen = sv.hoTen.length() > 24 ? sv.hoTen.substr(0, 24) : sv.hoTen; // Giảm từ 22 xuống 20
    
    // Hàm helper để căn giữa text
    auto centerText = [](string text, int width) -> string {
        if(text.length() >= width) return text.substr(0, width);
        int padding = (width - text.length()) / 2;
        int rightPadding = width - text.length() - padding;
        return string(padding, ' ') + text + string(rightPadding, ' ');
    };
    
    // Ma Lop - CĂN GIỮA (8 ký tự)
    if(highlight) {
        setColor(BLACK);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                              BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    } else {
        setColor(rowColor);
    }
    cout << centerText(displayMaLop, 8);
    
    setColor(highlight ? BLACK : LIGHTCYAN);
    cout << "│";
    
    // Ma SV - CĂN GIỮA (10 ký tự)
    if(highlight) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                              BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    } else {
        setColor(rowColor);
    }
    cout << centerText(displayMaSV, 14);
    
    setColor(highlight ? BLACK : LIGHTCYAN);
    cout << "│";
    
    // Ho Ten - CĂN TRÁI (20 ký tự)
    if(highlight) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                              BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    } else {
        setColor(rowColor);
    }
    cout << left << setw(24) << displayHoTen; // Giảm từ 22 xuống 20
    
    setColor(highlight ? BLACK : LIGHTCYAN);
    cout << "│";
    
    // Ngay Sinh - CĂN GIỮA (10 ký tự)
    if(highlight) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                              BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    } else {
        setColor(LIGHTMAGENTA);
    }
    
    // Format ngày sinh với căn giữa
    ostringstream dateStream;
    dateStream << setfill('0') << setw(2) << sv.ngaySinh.day << "/"
               << setw(2) << sv.ngaySinh.month << "/"
               << sv.ngaySinh.year;
    string dateStr = dateStream.str();
    
    cout << centerText(dateStr, 14);
    
    setColor(highlight ? BLACK : LIGHTCYAN);
    cout << "│";
    
    // Diem TB - CĂN GIỮA (8 ký tự, cột cuối)
    if(highlight) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                              BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    } else {
        setColor(rowColor);
    }
    
    // Format điểm với căn giữa
    ostringstream scoreStream;
    scoreStream << fixed << setprecision(2) << sv.diemTBTL;
    string scoreStr = scoreStream.str();
    cout << centerText(scoreStr, 12);
    
    setColor(LIGHTGRAY);
}

// Hàm hiển thị chi tiết một sinh viên
void showStudentDetail(const Student& sv) {
    clearScreen();
    
    setColor(LIGHTCYAN);
    gotoxy(25, 2);
    cout << "══════════════════════════════════════════";
    gotoxy(25, 3);
    cout << "           CHI TIET SINH VIEN             ";
    gotoxy(25, 4);
    cout << "══════════════════════════════════════════";
    
    // Vẽ khung thông tin
    int x = 20, y = 7;
    drawTableBorder(x, y, 50, 12);
    
    setColor(YELLOW);
    gotoxy(x + 2, y + 1);
    cout << "Thong tin chi tiet:";
    
    setColor(WHITE);
    gotoxy(x + 2, y + 3);
    cout << "Ma lop:              " << sv.maLop;
    
    gotoxy(x + 2, y + 4);
    cout << "Ma sinh vien:        " << sv.maSV;
    
    gotoxy(x + 2, y + 5);
    cout << "Ho va ten:           " << sv.hoTen;
    
    gotoxy(x + 2, y + 6);
    cout << "Ngay sinh:           " << setfill('0') << setw(2) << sv.ngaySinh.day 
         << "/" << setw(2) << sv.ngaySinh.month 
         << "/" << sv.ngaySinh.year;
    
    gotoxy(x + 2, y + 7);
    cout << "Diem TB tich luy:    " << fixed << setprecision(2) << sv.diemTBTL;
    
    // Xếp loại
    string xepLoai;
    if(sv.diemTBTL >= 9.0) xepLoai = "Xuat sac";
    else if(sv.diemTBTL >= 8.0) xepLoai = "Gioi";
    else if(sv.diemTBTL >= 6.5) xepLoai = "Kha";
    else if(sv.diemTBTL >= 5.0) xepLoai = "Trung binh";
    else xepLoai = "Yeu";
    
    gotoxy(x + 2, y + 8);
    cout << "Xep loai:            ";
    
    // Màu sắc theo xếp loại
    if(sv.diemTBTL >= 8.0) setColor(LIGHTGREEN);
    else if(sv.diemTBTL >= 6.5) setColor(YELLOW);
    else if(sv.diemTBTL >= 5.0) setColor(LIGHTGRAY);
    else setColor(LIGHTRED);
    
    cout << xepLoai;
    
    setColor(LIGHTGREEN);
    gotoxy(30, y + 15);
    cout << "Nhan phim bat ky de quay lai...";
    _getch();
}
