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
void printStudents(vector<Student>& students);
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
void displaySearchResults(const vector<Student>& results, const string& keyword);
float inputFloatTextBox(string prompt, int x, int y, float min, float max);
void drawTableBorder(int x, int y, int width, int height);
void drawTableSeparator(int x, int y, int width);
void printTableHeader(int x, int y);
void printStudentRow(int x, int y, const Student& sv, bool highlight);
void drawStatBar(string label, int count, int total, int x, int y, int color);
int showEditDeleteMenu(const Student& sv);
void editStudentInPlace(vector<Student>& students, int index);
bool confirmDelete(const Student& sv);

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
        "2. In danh sach sinh vien",      // Đẩy lên từ chức năng 3
        "3. Sap xep danh sach",           // Đẩy lên từ chức năng 4
        "4. Tim kiem sinh vien",          // Đẩy lên từ chức năng 5
        "5. Thong ke bao cao",            // Đẩy lên từ chức năng 6
        "6. Thoat chuong trinh"           // Đẩy lên từ chức năng 7
    };
    
    int menuSize = 6; // Giảm từ 7 xuống 6
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
        gotoxy(25, 15);
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
            return selected + 1; // Trả về 1-6
        } else if(key == KEY_ESC) {
            return 6; // Thoát (giảm từ 7 xuống 6)
        }
    }
}

// Menu con cho sắp xếp - Bỏ phần xác nhận
int showSortMenu() {
    string sortOptions[] = {"Ma SV", "Ho Ten", "Ngay Sinh", "Diem TB", "Ma Lop"};
    string algorithms[] = {"Selection Sort", "Bubble Sort", "Insertion Sort", "Quick Sort"};
    
    int selected = 0;
    int algSelected = 0;
    bool showAlgorithms = false;
    int key;
    
    while(true) {
        clearScreen();
        setColor(LIGHTCYAN);
        gotoxy(20, 2);
        cout << "════════════════════════════════════════════";
        gotoxy(20, 3);
        cout << "              MENU SAP XEP                  ";
        gotoxy(20, 4);
        cout << "════════════════════════════════════════════";
        
        setColor(YELLOW);
        gotoxy(15, 6);
        cout << "Chon tieu chi sap xep:";
        
        // Menu dọc cho tiêu chí sắp xếp
        for(int i = 0; i < 5; i++) {
            gotoxy(20, 8 + i);
            if(i == selected) {
                setColor(BLACK);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                      BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                cout << ">> " << left << setw(12) << sortOptions[i] << " <<";
            } else {
                setColor(LIGHTGRAY);
                cout << "   " << left << setw(12) << sortOptions[i] << "   ";
            }
        }
        
        // Hiển thị menu thuật toán nếu được kích hoạt
        if(showAlgorithms) {
            setColor(LIGHTCYAN);
            gotoxy(45, 6);
            cout << "Chon thuat toan:";
            
            // Vẽ khung cho menu thuật toán
            setColor(LIGHTCYAN);
            gotoxy(45, 7);
            for(int i = 0; i < 4; i++) {
                gotoxy(45, 8 + i);
                if(i == algSelected) {
                    setColor(BLACK);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                          BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                    cout << ">> " << left << setw(14) << algorithms[i];
                } else {
                    setColor(LIGHTCYAN);
                    cout << "  " << left << setw(15) << algorithms[i];
                }
            }
        }
        
        // Hướng dẫn sử dụng
        setColor(LIGHTGREEN);
        gotoxy(15, 15);
        if(showAlgorithms) {
            cout << "↑↓: Chọn thuật toán  │  ←: Đóng menu thuật toán  │  Enter: Thực hiện ngay";
        } else {
            cout << "↑↓: Chọn tiêu chí  │  →: Mở menu thuật toán  │  ESC: Quay lại";
        }
        
        setColor(LIGHTMAGENTA);
        gotoxy(15, 17);
        cout << "Tiêu chí đã chọn: " << sortOptions[selected];
        if(showAlgorithms) {
            gotoxy(15, 18);
            cout << "Thuật toán đã chọn: " << algorithms[algSelected];
        }
        
        // Đọc phím
        key = _getch();
        
        if(key == 224) { // Phím đặc biệt
            key = _getch();
            switch(key) {
                case KEY_UP:
                    if(showAlgorithms) {
                        algSelected = (algSelected - 1 + 4) % 4;
                    } else {
                        selected = (selected - 1 + 5) % 5;
                    }
                    break;
                case KEY_DOWN:
                    if(showAlgorithms) {
                        algSelected = (algSelected + 1) % 4;
                    } else {
                        selected = (selected + 1) % 5;
                    }
                    break;
                case KEY_RIGHT:
                    if(!showAlgorithms) {
                        showAlgorithms = true;
                        algSelected = 0; // Reset về thuật toán đầu tiên
                    }
                    break;
                case KEY_LEFT:
                    if(showAlgorithms) {
                        showAlgorithms = false;
                    }
                    break;
            }
        } else if(key == KEY_ENTER) {
            if(showAlgorithms) {
                // BỎ PHẦN XÁC NHẬN - THỰC HIỆN NGAY LẬP TỨC
                setColor(LIGHTGRAY);
                return selected + 1; // Trả về tiêu chí được chọn (1-5)
            } else {
                // Nếu chưa chọn thuật toán, tự động mở menu thuật toán
                showAlgorithms = true;
                algSelected = 0;
            }
        } else if(key == KEY_ESC) {
            if(showAlgorithms) {
                showAlgorithms = false;
            } else {
                setColor(LIGHTGRAY);
                return 0; // Thoát
            }
        }
    }
}

// Menu con cho tìm kiếm - Menu dọc với submenu thuật toán
int showSearchMenu() {
    string searchOptions[] = {"Ma SV", "Ho Ten", "Ma Lop", "Ngay Sinh", "Diem TB"};
    string algorithms[] = {"Tim kiem tuan tu", "Tim kiem nhi phan"};
    
    int selected = 0;
    int algSelected = 0;
    bool showAlgorithms = false;
    int key;
    
    while(true) {
        clearScreen();
        setColor(LIGHTCYAN);
        gotoxy(20, 2);
        cout << "════════════════════════════════════════════";
        gotoxy(20, 3);
        cout << "              MENU TIM KIEM                 ";
        gotoxy(20, 4);
        cout << "════════════════════════════════════════════";
        
        setColor(YELLOW);
        gotoxy(15, 6);
        cout << "Chon tieu chi tim kiem:";
        
        // Menu dọc cho tiêu chí tìm kiếm
        for(int i = 0; i < 5; i++) {
            gotoxy(20, 8 + i);
            if(i == selected) {
                setColor(BLACK);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                      BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                cout << ">> " << left << setw(12) << searchOptions[i] << " <<";
            } else {
                setColor(LIGHTGRAY);
                cout << "   " << left << setw(12) << searchOptions[i] << "   ";
            }
        }
        
        // Hiển thị menu thuật toán nếu được kích hoạt
        if(showAlgorithms) {
            setColor(LIGHTCYAN);
            gotoxy(45, 6);
            cout << "Chon thuat toan:";
            for(int i = 0; i < 2; i++) {
                gotoxy(45, 8 + i);
                if(i == algSelected) {
                    setColor(BLACK);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                          BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                    cout << ">> " << left << setw(13) << algorithms[i] ;
                } else {
                    setColor(LIGHTCYAN);
                    cout << "  " << left << setw(14) << algorithms[i] ;
                }
            }
        }
        
        // Hướng dẫn sử dụng
        setColor(LIGHTGREEN);
        gotoxy(15, 15);
        if(showAlgorithms) {
            cout << "↑↓: Chọn thuật toán  │  ←: Đóng menu thuật toán  │  Enter: Thực hiện";
        } else {
            cout << "↑↓: Chọn tiêu chí  │  →: Mở menu thuật toán  │  ESC: Quay lại";
        }
        
        setColor(LIGHTMAGENTA);
        gotoxy(15, 17);
        cout << "Tiêu chí đã chọn: " << searchOptions[selected];
        if(showAlgorithms) {
            gotoxy(15, 18);
            cout << "Thuật toán đã chọn: " << algorithms[algSelected];
        }
        
        // Đọc phím
        key = _getch();
        
        if(key == 224) { // Phím đặc biệt
            key = _getch();
            switch(key) {
                case KEY_UP:
                    if(showAlgorithms) {
                        algSelected = (algSelected - 1 + 2) % 2;
                    } else {
                        selected = (selected - 1 + 5) % 5;
                    }
                    break;
                case KEY_DOWN:
                    if(showAlgorithms) {
                        algSelected = (algSelected + 1) % 2;
                    } else {
                        selected = (selected + 1) % 5;
                    }
                    break;
                case KEY_RIGHT:
                    if(!showAlgorithms) {
                        showAlgorithms = true;
                        algSelected = 0; // Reset về thuật toán đầu tiên
                    }
                    break;
                case KEY_LEFT:
                    if(showAlgorithms) {
                        showAlgorithms = false;
                    }
                    break;
            }
        } else if(key == KEY_ENTER) {
            if(showAlgorithms) {
                // Trả về giá trị đã encode: tiêu chí (1-5) * 10 + thuật toán (1-2)
                setColor(LIGHTGRAY);
                return (selected + 1) * 10 + (algSelected + 1);
            } else {
                // Nếu chưa chọn thuật toán, tự động mở menu thuật toán
                showAlgorithms = true;
                algSelected = 0;
            }
        } else if(key == KEY_ESC) {
            if(showAlgorithms) {
                showAlgorithms = false;
            } else {
                setColor(LIGHTGRAY);
                return 0; // Thoát
            }
        }
    }
}

// Cập nhật hàm main - Bỏ chức năng 2 và đẩy các chức năng lên
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
                printStudents(students); // Chuyển từ case 3
                break;
            case 3: {
                int sortChoice = showSortMenu(); // Chuyển từ case 4
                if(sortChoice > 0) {
                    sortStudents(students, sortChoice);
                }
                break;
            }
            case 4: {
                int searchChoice = showSearchMenu(); // Chuyển từ case 5
                if(searchChoice > 0) {
                    searchStudents(students, searchChoice);
                }
                break;
            }
            case 5:
                statisticsMenu(students); // Chuyển từ case 6
                break;
            case 6:
                saveToFile(students); // Chuyển từ case 7
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
    } while(choice != 6); // Giảm từ choice != 7 xuống choice != 6
    
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
    gotoxy(25, 1);
    cout << "==============================";
    gotoxy(25, 2);
    cout << "    THEM MOI SINH VIEN       ";
    gotoxy(25, 3);
    cout << "==============================";
    
    // Hướng dẫn sử dụng
    setColor(YELLOW);
    gotoxy(15, 4);
    cout << "Su dung phim mui ten, Home/End, Delete/Backspace de chinh sua";
    gotoxy(15, 5);
    cout << "Nhan Enter de xac nhan, ESC de huy";
    
    // Nhập dữ liệu với textbox
    sv.maLop = inputTextBox("Ma lop:", 20, 7, 25);
    if(sv.maLop.empty()) return;
    
    // Kiểm tra mã sinh viên (8 số)
    bool validMaSV = false;
    do {
        sv.maSV = inputTextBox("Ma sinh vien (8 so):", 20, 11, 25, sv.maSV);
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
    
    sv.hoTen = inputTextBox("Ho va ten:", 20, 15, 35);
    if(sv.hoTen.empty()) return;
    sv.hoTen = formatName(sv.hoTen);
    
    // Nhập ngày sinh với validation
    sv.ngaySinh = inputDateTextBox("Ngay sinh", 20, 19);
    
    // Nhập điểm với validation
    sv.diemTBTL = inputFloatTextBox("Diem trung binh tich luy", 20, 23, 0.0, 10.0);
    
    students.push_back(sv);
    
    // LƯU TỰ ĐỘNG VÀO FILE
    saveToFile(students);
    // Thông báo thành công
    setColor(LIGHTGREEN);
    gotoxy(20, 27);
    cout << "THEM SINH VIEN THANH CONG!";
    gotoxy(20, 28);
    cout << "Nhan phim bat ki de thoat!";
    /*gotoxy(30, 42);
    cout << "Nhan phim bat ky de tiep tuc...";*/
    _getch();
}

// Cập nhật hàm printStudents - Enter để hiển thị menu Sửa/Xóa
void printStudents(vector<Student>& students) {
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
        
        // Vẽ bảng
        int tableX = 8;
        int tableY = 6;
        int tableWidth = 80;
        int tableHeight = RECORDS_PER_PAGE + 4;
        
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
        
        // Hướng dẫn sử dụng CẬP NHẬT
        setColor(LIGHTGREEN);
        gotoxy(5, tableY + tableHeight + 1);
        cout << "↑↓: Di chuyển  │  ←→: Chuyển trang  │  Enter: Menu Sửa/Xóa  │  ESC: Thoát";
        
        // Thanh trạng thái
        setColor(LIGHTMAGENTA);
        gotoxy(15, tableY + tableHeight + 3);
        if(selectedRow < endIndex - startIndex) {
            const Student& selected = students[startIndex + selectedRow];
            cout << "Đang chọn: " << selected.hoTen.substr(0, 20) << " - " 
                 << selected.maSV << " - Lớp: " << selected.maLop;
        }
        
        // Sửa lại phần xử lý phím trong hàm printStudents
        // Xử lý phím
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
            // HIỂN THỊ MENU CON SỬA/XÓA KHI NHẤN ENTER
            if(selectedRow < endIndex - startIndex) {
                int studentIndex = startIndex + selectedRow;
                int action = showEditDeleteMenu(students[studentIndex]);
                
                if(action == 1) {
                    // Sửa sinh viên
                    editStudentInPlace(students, studentIndex);
                    saveToFile(students); // Lưu sau khi sửa
                    
                    // Cập nhật lại totalPages nếu cần
                    totalPages = (students.size() + RECORDS_PER_PAGE - 1) / RECORDS_PER_PAGE;
                } else if(action == 2) {
                    // Xóa sinh viên
                    if(confirmDelete(students[studentIndex])) {
                        students.erase(students.begin() + studentIndex);
                        saveToFile(students); // Lưu sau khi xóa
                        
                        // Cập nhật lại các chỉ số
                        totalPages = (students.size() + RECORDS_PER_PAGE - 1) / RECORDS_PER_PAGE;
                        if(students.empty()) return; // Nếu danh sách rỗng thì thoát
                        
                        // Điều chỉnh selectedRow và currentPage
                        if(selectedRow >= min(RECORDS_PER_PAGE, (int)students.size() - startIndex)) {
                            if(selectedRow > 0) {
                                selectedRow--;
                            } else if(currentPage > 0) {
                                currentPage--;
                                selectedRow = 0;
                            }
                        }
                        
                        // Đảm bảo currentPage không vượt quá
                        if(currentPage >= totalPages && totalPages > 0) {
                            currentPage = totalPages - 1;
                        }
                    }
                }
            }
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

// Cập nhật hàm sortStudents để hiển thị thông tin thuật toán
void sortStudents(vector<Student>& students, int choice) {
    string sortNames[] = {"", "Mã SV", "Họ Tên", "Ngày Sinh", "Điểm TB", "Mã Lớp"};
    
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
            cout << "Lựa chọn không hợp lệ!";
            gotoxy(30, 11);
            cout << "Nhấn phím bất kỳ để tiếp tục...";
            _getch();
            return;
    }
    
    // Lưu tự động sau khi sắp xếp
    saveToFile(students);
    
    clearScreen();
    setColor(LIGHTGREEN);
    gotoxy(25, 9);
    cout << "════════════════════════════════════════";
    gotoxy(25, 10);
    cout << "          SẮP XẾP THÀNH CÔNG!           ";
    gotoxy(25, 11);
    cout << "════════════════════════════════════════";
    
    setColor(YELLOW);
    gotoxy(20, 13);
    cout << "Đã sắp xếp theo: " << sortNames[choice];
    gotoxy(20, 14);
    cout << "Dữ liệu đã được lưu tự động!";
    
    setColor(WHITE);
    gotoxy(20, 16);
    cout << "Nhấn phím bất kỳ để xem kết quả...";
    _getch();
    
    printStudents(students);
}

// Sửa lại hàm searchStudents - Thêm khai báo sortedStudents
void searchStudents(const vector<Student>& students, int encodedChoice) {
    if(encodedChoice == 0) return;
    
    // Decode: tiêu chí = choice / 10, thuật toán = choice % 10
    int criteria = encodedChoice / 10;
    int algorithm = encodedChoice % 10;
    
    string criteriaNames[] = {"", "Mã SV", "Họ Tên", "Mã Lớp", "Ngày Sinh", "Điểm TB"};
    string algorithmNames[] = {"", "Tìm kiếm tuần tự", "Tìm kiếm nhị phân"};
    
    clearScreen();
    setColor(LIGHTCYAN);
    gotoxy(20, 2);
    cout << "══════════════════════════════════════════════";
    gotoxy(20, 3);
    cout << "              TIM KIEM SINH VIEN              ";
    gotoxy(20, 4);
    cout << "══════════════════════════════════════════════";
    
    setColor(YELLOW);
    gotoxy(15, 6);
    cout << "Tiêu chí: " << criteriaNames[criteria];
    gotoxy(15, 7);
    cout << "Thuật toán: " << algorithmNames[algorithm];
    
    // DÙNG TEXTBOX THAY VÌ getline()
    string keyword = inputTextBox("Nhập từ khóa tìm kiếm:", 15, 9, 30);
    
    if(keyword.empty()) return; // Nếu người dùng nhấn ESC hoặc không nhập gì
    
    vector<Student> result;
    
    // THÊM KHAI BÁO sortedStudents
    vector<Student> sortedStudents = students;
    
    // Nếu chọn tìm kiếm nhị phân, cần sắp xếp trước
    if(algorithm == 2) { // Tìm kiếm nhị phân
        switch(criteria) {
            case 1: // Mã SV
                sort(sortedStudents.begin(), sortedStudents.end(), 
                     [](const Student& a, const Student& b) {
                         return a.maSV < b.maSV;
                     });
                break;
            case 2: // Họ Tên
                sort(sortedStudents.begin(), sortedStudents.end(), 
                     [](const Student& a, const Student& b) {
                         return a.hoTen < b.hoTen;
                     });
                break;
            case 3: // Mã Lớp
                sort(sortedStudents.begin(), sortedStudents.end(), 
                     [](const Student& a, const Student& b) {
                         return a.maLop < b.maLop;
                     });
                break;
            case 4: // Ngày Sinh
                sort(sortedStudents.begin(), sortedStudents.end(), 
                     [](const Student& a, const Student& b) {
                         if(a.ngaySinh.year != b.ngaySinh.year)
                             return a.ngaySinh.year < b.ngaySinh.year;
                         if(a.ngaySinh.month != b.ngaySinh.month)
                             return a.ngaySinh.month < b.ngaySinh.month;
                         return a.ngaySinh.day < b.ngaySinh.day;
                     });
                break;
            case 5: // Điểm TB
                sort(sortedStudents.begin(), sortedStudents.end(), 
                     [](const Student& a, const Student& b) {
                         return a.diemTBTL < b.diemTBTL;
                     });
                break;
        }
    }
    
    // Thực hiện tìm kiếm
    if(algorithm == 1) { // Tìm kiếm tuần tự
        for(const auto& sv : students) {
            bool found = false;
            switch(criteria) {
                case 1: // Mã SV
                    if(sv.maSV.find(keyword) != string::npos) found = true;
                    break;
                case 2: // Họ Tên
                    if(sv.hoTen.find(keyword) != string::npos) found = true;
                    break;
                case 3: // Mã Lớp
                    if(sv.maLop.find(keyword) != string::npos) found = true;
                    break;
                case 4: // Ngày Sinh
                    {
                        char dateStr[20];
                        sprintf(dateStr, "%02d/%02d/%04d", sv.ngaySinh.day, sv.ngaySinh.month, sv.ngaySinh.year);
                        if(string(dateStr).find(keyword) != string::npos) found = true;
                    }
                    break;
                case 5: // Điểm TB
                    {
                        string scoreStr = to_string(sv.diemTBTL);
                        if(scoreStr.find(keyword) != string::npos) found = true;
                    }
                    break;
            }
            if(found) result.push_back(sv);
        }
    } else { // Tìm kiếm nhị phân
        // Tìm kiếm nhị phân cho exact match
        int left = 0, right = sortedStudents.size() - 1;
        bool found = false;
        
        while(left <= right) {
            int mid = (left + right) / 2;
            string compareValue;
            
            switch(criteria) {
                case 1: compareValue = sortedStudents[mid].maSV; break;
                case 2: compareValue = sortedStudents[mid].hoTen; break;
                case 3: compareValue = sortedStudents[mid].maLop; break;
                case 4: 
                    {
                        char dateStr[20];
                        sprintf(dateStr, "%02d/%02d/%04d", 
                               sortedStudents[mid].ngaySinh.day, 
                               sortedStudents[mid].ngaySinh.month, 
                               sortedStudents[mid].ngaySinh.year);
                        compareValue = dateStr;
                    }
                    break;
                case 5: compareValue = to_string(sortedStudents[mid].diemTBTL); break;
            }
            
            if(compareValue == keyword) {
                result.push_back(sortedStudents[mid]);
                found = true;
                
                // Tìm thêm các phần tử giống nhau bên trái và phải
                int leftIdx = mid - 1;
                while(leftIdx >= 0) {
                    string leftValue;
                    switch(criteria) {
                        case 1: leftValue = sortedStudents[leftIdx].maSV; break;
                        case 2: leftValue = sortedStudents[leftIdx].hoTen; break;
                        case 3: leftValue = sortedStudents[leftIdx].maLop; break;
                        case 4: 
                            {
                                char dateStr[20];
                                sprintf(dateStr, "%02d/%02d/%04d", 
                                       sortedStudents[leftIdx].ngaySinh.day, 
                                       sortedStudents[leftIdx].ngaySinh.month, 
                                       sortedStudents[leftIdx].ngaySinh.year);
                                leftValue = dateStr;
                            }
                            break;
                        case 5: leftValue = to_string(sortedStudents[leftIdx].diemTBTL); break;
                    }
                    if(leftValue == keyword) {
                        result.insert(result.begin(), sortedStudents[leftIdx]);
                        leftIdx--;
                    } else break;
                }
                
                int rightIdx = mid + 1;
                while(rightIdx < sortedStudents.size()) {
                    string rightValue;
                    switch(criteria) {
                        case 1: rightValue = sortedStudents[rightIdx].maSV; break;
                        case 2: rightValue = sortedStudents[rightIdx].hoTen; break;
                        case 3: rightValue = sortedStudents[rightIdx].maLop; break;
                        case 4: 
                            {
                                char dateStr[20];
                                sprintf(dateStr, "%02d/%02d/%04d", 
                                       sortedStudents[rightIdx].ngaySinh.day, 
                                       sortedStudents[rightIdx].ngaySinh.month, 
                                       sortedStudents[rightIdx].ngaySinh.year);
                                rightValue = dateStr;
                            }
                            break;
                        case 5: rightValue = to_string(sortedStudents[rightIdx].diemTBTL); break;
                    }
                    if(rightValue == keyword) {
                        result.push_back(sortedStudents[rightIdx]);
                        rightIdx++;
                    } else break;
                }
                break;
            } else if(compareValue < keyword) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    
    if(result.empty()) {
        setColor(LIGHTRED);
        gotoxy(25, 12);
        cout << "KHÔNG TÌM THẤY SINH VIÊN NÀO!";
        gotoxy(25, 13);
        cout << "Nhấn phím bất kỳ để tiếp tục...";
        _getch();
    } else {
        // Hiển thị kết quả với phân trang
        displaySearchResults(result, keyword + " (" + algorithmNames[algorithm] + ")");
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
        cout << "↑↓: Di chuyển  │  ←→: Chuyển trang  │  ESC: Thoat";
        
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

// Hàm hiển thị menu con sửa/xóa khi nhấn Enter
int showEditDeleteMenu(const Student& sv) {
    string menuItems[] = {"Sửa thông tin", "Xóa sinh viên", "Hủy"};
    int selected = 0;
    int key;
    
    // Vị trí menu con (góc phải màn hình)
    int menuX = 55;
    int menuY = 8;
    
    while(true) {
        // Vẽ khung menu con
        setColor(LIGHTCYAN);
        gotoxy(menuX, menuY);
        cout << "";
        
        setColor(LIGHTCYAN);
        gotoxy(menuX, menuY + 1);
        cout << "┌─────────────────┐";
        
        // Hiển thị các lựa chọn
        for(int i = 0; i < 3; i++) {
            gotoxy(menuX, menuY + 2 + i);
            if(i == selected) {
                if(i == 0) {
                    setColor(BLACK);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                BACKGROUND_RED | BACKGROUND_INTENSITY);
                } else {
                    setColor(BLACK);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                }
                cout << "│>" << left << setw(14) << menuItems[i] << "   │";
            } else {
                setColor(LIGHTCYAN);
                cout << "│ " << left << setw(15) << menuItems[i] << "   │";
            }
        }
        
        setColor(LIGHTCYAN);
        gotoxy(menuX, menuY + 5);
        cout << "└─────────────────┘";
        
        // Hướng dẫn
        setColor(LIGHTGREEN);
        gotoxy(menuX, menuY + 7);
        cout << "↑↓: Chọn, Enter: OK";
        
        // Đọc phím
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
            // Xóa menu con
            for(int i = 0; i < 13; i++) {
                gotoxy(menuX, menuY + i);
                cout << "                    ";
            }
            setColor(LIGHTGRAY);
            return selected + 1; // 1: Sửa, 2: Xóa, 3: Hủy
        } else if(key == KEY_ESC) {
            // Xóa menu con
            for(int i = 0; i < 13; i++) {
                gotoxy(menuX, menuY + i);
                cout << "                    ";
            }
            setColor(LIGHTGRAY);
            return 0; // Hủy
        }
    }
}

// Hàm sửa sinh viên tại vị trí cụ thể
void editStudentInPlace(vector<Student>& students, int index) {
    if(index < 0 || index >= students.size()) return;
    
    Student& sv = students[index];
    
    clearScreen();
    setColor(LIGHTCYAN);
    gotoxy(25, 2);
    cout << "══════════════════════════════════════════";
    gotoxy(25, 3);
    cout << "           SỬA THÔNG TIN SINH VIÊN         ";
    gotoxy(25, 4);
    cout << "══════════════════════════════════════════";
    
    setColor(YELLOW);
    gotoxy(20, 6);
    cout << "Sinh viên hiện tại: " << sv.hoTen << " (" << sv.maSV << ")";
    
    // Hướng dẫn
    setColor(LIGHTGREEN);
    gotoxy(15, 8);
    cout << "Nhấn Enter để giữ nguyên giá trị, ESC để hủy thay đổi";
    
    // Sửa từng trường với giá trị mặc định là giá trị hiện tại
    string newMaLop = inputTextBox("Mã lớp:", 20, 10, 25, sv.maLop);
    if(newMaLop != sv.maLop && !newMaLop.empty()) {
        sv.maLop = newMaLop;
    }
    
    // Kiểm tra mã sinh viên (8 số)
    bool validMaSV = true;
    string newMaSV;
    do {
        newMaSV = inputTextBox("Mã sinh viên (8 số):", 20, 14, 25, sv.maSV);
        if(newMaSV == sv.maSV || newMaSV.empty()) {
            break; // Giữ nguyên
        }
        
        validMaSV = (newMaSV.length() == 8 && 
                    all_of(newMaSV.begin(), newMaSV.end(), ::isdigit));
        
        if(!validMaSV) {
            setColor(LIGHTRED);
            gotoxy(20, 21);
            cout << "Mã sinh viên phải là 8 chữ số! Nhấn Enter để nhập lại...";
            _getch();
            gotoxy(20, 21);
            cout << string(55, ' ');
        }
    } while(!validMaSV);
    
    if(validMaSV && newMaSV != sv.maSV && !newMaSV.empty()) {
        sv.maSV = newMaSV;
    }
    
    string newHoTen = inputTextBox("Họ và tên:", 20, 18, 35, sv.hoTen);
    if(newHoTen != sv.hoTen && !newHoTen.empty()) {
        sv.hoTen = formatName(newHoTen);
    }
    
    // Ngày sinh
    char dateBuffer[20];
    sprintf(dateBuffer, "%02d/%02d/%04d", sv.ngaySinh.day, sv.ngaySinh.month, sv.ngaySinh.year);
    string dateInput = inputTextBox("Ngày sinh (dd/mm/yyyy):", 20, 22, 20, dateBuffer);
    
    if(dateInput != dateBuffer && !dateInput.empty()) {
        Date newDate;
        if(sscanf(dateInput.c_str(), "%d/%d/%d", &newDate.day, &newDate.month, &newDate.year) == 3) {
            if(isValidDate(newDate)) {
                sv.ngaySinh = newDate;
            }
        }
    }
    
    // Điểm
    string scoreStr = to_string(sv.diemTBTL);
    scoreStr = scoreStr.substr(0, scoreStr.find_last_not_of('0') + 1);
    if(scoreStr.back() == '.') scoreStr.pop_back();
    
    string newScoreStr = inputTextBox("Điểm TB (0.0-10.0):", 20, 26, 15, scoreStr);
    
    if(newScoreStr != scoreStr && !newScoreStr.empty()) {
        try {
            float newScore = stof(newScoreStr);
            if(newScore >= 0.0 && newScore <= 10.0) {
                sv.diemTBTL = newScore;
            }
        } catch(...) {
            // Giữ nguyên giá trị cũ nếu không hợp lệ
        }
    }
    
    // Thông báo thành công
    setColor(LIGHTGREEN);
    gotoxy(25, 30);
    cout << "CẬP NHẬT THÀNH CÔNG!";
    gotoxy(25, 31);
    cout << "Nhấn phím bất kỳ để tiếp tục...";
    _getch();
}

// Hàm xác nhận xóa sinh viên - Đơn giản hóa
bool confirmDelete(const Student& sv) {
    clearScreen();
    
    setColor(LIGHTRED);
    gotoxy(30, 10);
    cout << "══════════════════════════════════════";
    gotoxy(30, 11);
    cout << "         XÁC NHẬN XÓA SINH VIÊN       ";
    gotoxy(30, 12);
    cout << "══════════════════════════════════════";
    
    setColor(YELLOW);
    gotoxy(25, 15);
    cout << "Bạn có chắc chắn muốn xóa sinh viên này không?";
    
    string options[] = {"CÓ - Xóa sinh viên", "KHÔNG - Hủy bỏ"};
    int selected = 1; // Mặc định chọn "KHÔNG" (an toàn hơn)
    int key;
    
    while(true) {
        // Hiển thị 2 lựa chọn
        for(int i = 0; i < 2; i++) {
            gotoxy(30, 18 + i);
            if(i == selected) {
                if(i == 0) {
                    // Nút XÓA - màu đỏ khi được chọn
                    setColor(BLACK);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                          BACKGROUND_RED | BACKGROUND_INTENSITY);
                } else {
                    // Nút HỦY - màu xanh khi được chọn
                    setColor(BLACK);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                          BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                }
                cout << ">> " << options[i] << " <<";
            } else {
                setColor(LIGHTGRAY);
                cout << "   " << options[i] << "   ";
            }
        }
        
        // Hướng dẫn sử dụng
        setColor(LIGHTGREEN);
        gotoxy(30, 22);
        cout << "↑↓: Chọn, Enter: Xác nhận, ESC: Hủy";
        
        // Đọc phím
        key = _getch();
        
        if(key == 224) {
            key = _getch();
            switch(key) {
                case KEY_UP:
                case KEY_DOWN:
                    selected = 1 - selected; // Chuyển đổi giữa 0 và 1
                    break;
            }
        } else if(key == KEY_ENTER) {
            setColor(LIGHTGRAY);
            return (selected == 0); // True nếu chọn "CÓ" (index 0)
        } else if(key == KEY_ESC) {
            setColor(LIGHTGRAY);
            return false; // Hủy bỏ
        }
    }
}
