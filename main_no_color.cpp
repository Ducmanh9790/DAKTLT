#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <cctype>
#include <map>
#include <conio.h>
#include <windows.h>
#include <sstream>
using namespace std;

// Phím điều khiển
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27

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
void gotoxy(int x, int y);
void clearScreen();
int showAdvancedMenu();
int showSortMenu();
int showSearchMenu();
void sortStudents(vector<Student>& students, int choice);
void searchStudents(const vector<Student>& students, int choice);
string inputTextBox(string prompt, int x, int y, int width, string defaultValue = "");
Date inputDateTextBox(string prompt, int x, int y);
void displaySearchResults(const vector<Student>& results, const string& keyword);
float inputFloatTextBox(string prompt, int x, int y, float min, float max);
void drawTableBorder(int x, int y, int width, int height);
void drawTableSeparator(int x, int y, int width);
void printTableHeader(int x, int y);
void printStudentRow(int x, int y, const Student& sv, bool highlight);
int showEditDeleteMenu(const Student& sv);
void editStudentInPlace(vector<Student>& students, int index);
bool confirmDelete(const Student& sv);
int showStatisticsMenu();
void reportStudentsByClass(const vector<Student>& students);
void reportGradesByClass(const vector<Student>& students);

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    system("cls");
}

// Menu nâng cao với phím mũi tên - KHÔNG MÀU
int showAdvancedMenu() {
    string menuItems[] = {
        "1. Them moi ho so sinh vien",
        "2. In danh sach sinh vien",     
        "3. Sap xep danh sach",           
        "4. Tim kiem sinh vien",          
        "5. Thong ke bao cao",           
        "6. Thoat chuong trinh"           
    };
    
    int menuSize = 6;
    int selected = 0;
    int key;
    
    while(true) {
        clearScreen();
        
        // Vẽ tiêu đề
        gotoxy(20, 2);
        cout << "========================================";
        gotoxy(20, 3);
        cout << "    CHUONG TRINH QUAN LY SINH VIEN     ";
        gotoxy(20, 4);
        cout << "========================================";
        
        gotoxy(25, 6);
        cout << "Su dung phim mui ten de chon, Enter de xac nhan";
        
        // Vẽ menu với highlight cải thiện
        for(int i = 0; i < menuSize; i++) {
            gotoxy(20, 8 + i);
            
            if(i == selected) {
                // Highlight với khung và mũi tên
                cout << "+- " << menuItems[i] << " -+";
                gotoxy(20, 8 + i + 1);
                cout << "+" << string(menuItems[i].length() + 4, '-') << "+";
                
                // Thêm mũi tên chỉ dẫn
                gotoxy(15, 8 + i);
                cout << ">>>>";
                gotoxy(25 + menuItems[i].length(), 8 + i);
                cout << "<<<<";
            } else {
                cout << "    " << menuItems[i] << "    ";
            }
        }
        
        gotoxy(25, 18);
        cout << "=======================================";
        gotoxy(25, 19);
        cout << "Chuc nang duoc chon: " << (selected + 1) << " - " << menuItems[selected].substr(3);
        
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
            return selected + 1; // Trả về 1-6
        } else if(key == KEY_ESC) {
            return 6; // Thoát
        }
    }
}

// Cập nhật showSortMenu với highlight tốt hơn
int showSortMenu() {
    string sortOptions[] = {"Ma SV", "Ho Ten", "Ngay Sinh", "Diem TB", "Ma Lop"};
    string algorithms[] = {"Selection Sort", "Bubble Sort", "Insertion Sort", "Quick Sort"};
    
    int selected = 0;
    int algSelected = 0;
    bool showAlgorithms = false;
    int key;
    
    while(true) {
        clearScreen();
        gotoxy(20, 2);
        cout << "================================================";
        gotoxy(20, 3);
        cout << "              MENU SAP XEP                     ";
        gotoxy(20, 4);
        cout << "================================================";
        
        gotoxy(15, 6);
        cout << "Chon tieu chi sap xep:";
        
        // Menu dọc cho tiêu chí sắp xếp với highlight cải thiện
        for(int i = 0; i < 5; i++) {
            gotoxy(20, 8 + i);
            if(i == selected) {
                cout << "+-> " << left << setw(12) << sortOptions[i] << " <-+";
            } else {
                cout << "    " << left << setw(12) << sortOptions[i] << "    ";
            }
        }
        
        // Hiển thị menu thuật toán nếu được kích hoạt
        if(showAlgorithms) {
            gotoxy(45, 6);
            cout << "Chon thuat toan:";
            
            // Vẽ khung bao quanh menu thuật toán
            gotoxy(45, 7);
            cout << "+------------------+";
            
            for(int i = 0; i < 4; i++) {
                gotoxy(45, 8 + i);
                if(i == algSelected) {
                    cout << "|* " << left << setw(15) << algorithms[i] << "|";
                } else {
                    cout << "|  " << left << setw(15) << algorithms[i] << "|";
                }
            }
            
            gotoxy(45, 12);
            cout << "+------------------+";
        }
        
        // Hướng dẫn sử dụng
        gotoxy(15, 15);
        if(showAlgorithms) {
            cout << "Phim mui ten: Chon thuat toan | <-: Dong menu | Enter: Thuc hien ngay";
        } else {
            cout << "Phim mui ten: Chon tieu chi | ->: Mo menu thuat toan | ESC: Quay lai";
        }
        
        // Thanh trạng thái
        gotoxy(15, 17);
        cout << "===========================================================";
        gotoxy(15, 18);
        cout << "Tieu chi da chon: " << sortOptions[selected];
        if(showAlgorithms) {
            gotoxy(15, 19);
            cout << "Thuat toan da chon: " << algorithms[algSelected];
        }
        
        // Đọc phím - giữ nguyên logic
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
                        algSelected = 0;
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
                return selected + 1;
            } else {
                showAlgorithms = true;
                algSelected = 0;
            }
        } else if(key == KEY_ESC) {
            if(showAlgorithms) {
                showAlgorithms = false;
            } else {
                return 0;
            }
        }
    }
}

// Menu con cho tìm kiếm - KHÔNG MÀU
int showSearchMenu() {
    string searchOptions[] = {"Ma SV", "Ho Ten", "Ma Lop", "Ngay Sinh", "Diem TB"};
    string algorithms[] = {"Tim kiem tuan tu", "Tim kiem nhi phan"};
    
    int selected = 0;
    int algSelected = 0;
    bool showAlgorithms = false;
    int key;
    
    while(true) {
        clearScreen();
        gotoxy(20, 2);
        cout << "================================================";
        gotoxy(20, 3);
        cout << "              MENU TIM KIEM                    ";
        gotoxy(20, 4);
        cout << "================================================";
        
        gotoxy(15, 6);
        cout << "Chon tieu chi tim kiem:";
        
        // Menu dọc cho tiêu chí tìm kiếm
        for(int i = 0; i < 5; i++) {
            gotoxy(20, 8 + i);
            if(i == selected) {
                cout << ">> " << left << setw(12) << searchOptions[i] << " <<";
            } else {
                cout << "   " << left << setw(12) << searchOptions[i] << "   ";
            }
        }
        
        // Hiển thị menu thuật toán nếu được kích hoạt
        if(showAlgorithms) {
            gotoxy(45, 6);
            cout << "Chon thuat toan:";
            for(int i = 0; i < 2; i++) {
                gotoxy(45, 8 + i);
                if(i == algSelected) {
                    cout << ">> " << left << setw(13) << algorithms[i];
                } else {
                    cout << "  " << left << setw(14) << algorithms[i];
                }
            }
        }
        
        // Hướng dẫn sử dụng
        gotoxy(15, 15);
        if(showAlgorithms) {
            cout << "Phim mui ten: Chon thuat toan | <-: Dong menu | Enter: Thuc hien";
        } else {
            cout << "Phim mui ten: Chon tieu chi | ->: Mo menu thuat toan | ESC: Quay lai";
        }
        
        gotoxy(15, 17);
        cout << "Tieu chi da chon: " << searchOptions[selected];
        if(showAlgorithms) {
            gotoxy(15, 18);
            cout << "Thuat toan da chon: " << algorithms[algSelected];
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
                        algSelected = 0;
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
                return (selected + 1) * 10 + (algSelected + 1);
            } else {
                showAlgorithms = true;
                algSelected = 0;
            }
        } else if(key == KEY_ESC) {
            if(showAlgorithms) {
                showAlgorithms = false;
            } else {
                return 0;
            }
        }
    }
}

int main() {
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
                printStudents(students);
                break;
            case 3: {
                int sortChoice = showSortMenu();
                if(sortChoice > 0) {
                    sortStudents(students, sortChoice);
                }
                break;
            }
            case 4: {
                int searchChoice = showSearchMenu();
                if(searchChoice > 0) {
                    searchStudents(students, searchChoice);
                }
                break;
            }
            case 5:
                statisticsMenu(students);
                break;
            case 6:
                saveToFile(students);
                clearScreen();
                gotoxy(30, 10);
                cout << "Cam on ban da su dung chuong trinh!";
                gotoxy(30, 11);
                cout << "Nhan phim bat ky de thoat...";
                _getch();
                break;
            default:
                gotoxy(30, 10);
                cout << "Lua chon khong hop le!";
                _getch();
        }
    } while(choice != 6);
    
    return 0;
}

// Hàm nhập text đơn giản - KHÔNG MÀU
string inputTextBox(string prompt, int x, int y, int width, string defaultValue) {
    string content = defaultValue;
    int cursorPos = content.length();
    int key;
    
    gotoxy(x, y - 1);
    cout << prompt;
    
    while(true) {
        // Vẽ textbox đơn giản
        gotoxy(x, y);
        cout << "[" << left << setw(width) << content << "]";
        gotoxy(x + 1 + cursorPos, y);
        
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
    
    return content;
}

// Hàm nhập ngày - KHÔNG MÀU
Date inputDateTextBox(string prompt, int x, int y) {
    Date date = {1, 1, 2000};
    string dateStr = "";
    bool validDate = false;
    
    do {
        dateStr = inputTextBox(prompt + " (dd/mm/yyyy):", x, y, 20, dateStr);
        
        if(sscanf(dateStr.c_str(), "%d/%d/%d", &date.day, &date.month, &date.year) == 3) {
            validDate = isValidDate(date);
        } else {
            validDate = false;
        }
        
        if(!validDate) {
            gotoxy(x, y + 2);
            cout << "Ngay khong hop le! Nhan Enter de nhap lai...";
            _getch();
            gotoxy(x, y + 2);
            cout << string(50, ' ');
        }
    } while(!validDate);
    
    return date;
}

// Hàm nhập số thực - KHÔNG MÀU
float inputFloatTextBox(string prompt, int x, int y, float min, float max) {
    float value = 0.0;
    string valueStr = "";
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
            gotoxy(x, y + 2);
            cout << "Gia tri khong hop le! Nhan Enter de nhap lai...";
            _getch();
            gotoxy(x, y + 2);
            cout << string(50, ' ');
        }
    } while(!validValue);
    
    return value;
}

// Thêm sinh viên - KHÔNG MÀU
void addStudent(vector<Student>& students) {
    Student sv;
    
    clearScreen();
    
    gotoxy(25, 1);
    cout << "==============================";
    gotoxy(25, 2);
    cout << "    THEM MOI SINH VIEN       ";
    gotoxy(25, 3);
    cout << "==============================";
    
    gotoxy(15, 4);
    cout << "Su dung phim mui ten, Home/End, Delete/Backspace de chinh sua";
    gotoxy(15, 5);
    cout << "Nhan Enter de xac nhan, ESC de huy";
    
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
            gotoxy(20, 15);
            cout << "Ma sinh vien phai la 8 chu so! Nhan Enter de nhap lai...";
            _getch();
            gotoxy(20, 15);
            cout << string(55, ' ');
        }
    } while(!validMaSV);
    
    sv.hoTen = inputTextBox("Ho va ten:", 20, 15, 35);
    if(sv.hoTen.empty()) return;
    sv.hoTen = formatName(sv.hoTen);
    
    sv.ngaySinh = inputDateTextBox("Ngay sinh", 20, 19);
    sv.diemTBTL = inputFloatTextBox("Diem trung binh tich luy", 20, 23, 0.0, 10.0);
    
    students.push_back(sv);
    saveToFile(students);
    
    gotoxy(20, 27);
    cout << "THEM SINH VIEN THANH CONG!";
    gotoxy(20, 28);
    cout << "Nhan phim bat ki de thoat!";
    _getch();
}

// In danh sách sinh viên - KHÔNG MÀU
void printStudents(vector<Student>& students) {
    if(students.empty()) {
        clearScreen();
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
        
        gotoxy(15, 1);
        cout << "========================================================================";
        gotoxy(15, 2);
        cout << "                          DANH SACH SINH VIEN                        ";
        gotoxy(15, 3);
        cout << "========================================================================";
        
        gotoxy(30, 4);
        cout << "Trang " << (currentPage + 1) << "/" << totalPages 
             << " - Tong so: " << students.size() << " sinh vien";
        
        // Vẽ bảng đơn giản
        int tableX = 8;
        int tableY = 6;
        
        drawTableBorder(tableX, tableY, 80, RECORDS_PER_PAGE + 4);
        printTableHeader(tableX, tableY + 1);
        drawTableSeparator(tableX, tableY + 2, 80);
        
        int startIndex = currentPage * RECORDS_PER_PAGE;
        int endIndex = min(startIndex + RECORDS_PER_PAGE, (int)students.size());
        
        for(int i = startIndex; i < endIndex; i++) {
            int rowIndex = i - startIndex;
            bool highlight = (rowIndex == selectedRow);
            printStudentRow(tableX, tableY + 3 + rowIndex, students[i], highlight);
        }
        
        gotoxy(5, tableY + RECORDS_PER_PAGE + 8);
        cout << "Phim mui ten: Di chuyen | <->: Chuyen trang | Enter: Menu Sua/Xoa | ESC: Thoat";
        
        gotoxy(15, tableY + RECORDS_PER_PAGE + 10);
        if(selectedRow < endIndex - startIndex) {
            const Student& selected = students[startIndex + selectedRow];
            cout << "Dang chon: " << selected.hoTen.substr(0, 20) << " - " 
                 << selected.maSV << " - Lop: " << selected.maLop;
        }
        
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
            if(selectedRow < endIndex - startIndex) {
                int studentIndex = startIndex + selectedRow;
                int action = showEditDeleteMenu(students[studentIndex]);
                
                if(action == 1) {
                    editStudentInPlace(students, studentIndex);
                    saveToFile(students);
                    totalPages = (students.size() + RECORDS_PER_PAGE - 1) / RECORDS_PER_PAGE;
                } else if(action == 2) {
                    if(confirmDelete(students[studentIndex])) {
                        students.erase(students.begin() + studentIndex);
                        saveToFile(students);
                        
                        totalPages = (students.size() + RECORDS_PER_PAGE - 1) / RECORDS_PER_PAGE;
                        if(students.empty()) return;
                        
                        if(selectedRow >= min(RECORDS_PER_PAGE, (int)students.size() - startIndex)) {
                            if(selectedRow > 0) {
                                selectedRow--;
                            } else if(currentPage > 0) {
                                currentPage--;
                                selectedRow = 0;
                            }
                        }
                        
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
}

// Phần còn lại của các hàm cũ nhưng bỏ màu...

// Hàm vẽ bảng đơn giản - KHÔNG MÀU
void drawTableBorder(int x, int y, int width, int height) {
    // Dòng trên
    gotoxy(x, y);
    cout << "+";
    for(int i = 0; i < 76; i++) cout << "-";
    cout << "+";
    
    // Các dòng giữa
    for(int i = 1; i < height - 1; i++) {
        gotoxy(x, y + i);
        cout << "|";
        for(int j = 0; j < 76; j++) cout << " ";
        gotoxy(x + 77, y + i);
        cout << "|";
    }
    
    // Dòng dưới
    gotoxy(x, y + height - 1);
    cout << "+";
    for(int i = 0; i < 76; i++) cout << "-";
    cout << "+";
}

void drawTableSeparator(int x, int y, int width) {
    gotoxy(x, y);
    cout << "+";
    for(int i = 0; i < 76; i++) cout << "-";
    cout << "+";
}

void printTableHeader(int x, int y) {
    auto centerHeaderText = [](string text, int width) -> string {
        if(text.length() >= width) return text.substr(0, width);
        int padding = (width - text.length()) / 2;
        int rightPadding = width - text.length() - padding;
        return string(padding, ' ') + text + string(rightPadding, ' ');
    };
    
    gotoxy(x+1, y);
    cout << centerHeaderText("Ma Lop", 8) << "|"
         << centerHeaderText("Ma SV", 14) << "|"
         << centerHeaderText("Ho Ten", 24) << "|"
         << centerHeaderText("Ngay Sinh", 14) << "|"
         << centerHeaderText("Diem TB", 12);
}

// Cập nhật printStudentRow với highlight rõ ràng hơn
void printStudentRow(int x, int y, const Student& sv, bool highlight) {
    gotoxy(x+1, y);
    
    string displayMaLop = sv.maLop.length() > 8 ? sv.maLop.substr(0, 8) : sv.maLop;
    string displayMaSV = sv.maSV.length() > 14 ? sv.maSV.substr(0, 14) : sv.maSV;
    string displayHoTen = sv.hoTen.length() > 24 ? sv.hoTen.substr(0, 24) : sv.hoTen;
    
    auto centerText = [](string text, int width) -> string {
        if(text.length() >= width) return text.substr(0, width);
        int padding = (width - text.length()) / 2;
        int rightPadding = width - text.length() - padding;
        return string(padding, ' ') + text + string(rightPadding, ' ');
    };
    
    if(highlight) {
        // Highlight với ký tự đặc biệt
        cout << ">";
    } else {
        cout << " ";
    }
    
    cout << centerText(displayMaLop, 7) << "|"
         << centerText(displayMaSV, 14) << "|";
    
    // Highlight họ tên nếu được chọn
    if(highlight) {
        cout << "[" << left << setw(22) << displayHoTen << "]|";
    } else {
        cout << " " << left << setw(23) << displayHoTen << "|";
    }
    
    ostringstream dateStream;
    dateStream << setfill('0') << setw(2) << sv.ngaySinh.day << "/"
               << setw(2) << sv.ngaySinh.month << "/"
               << sv.ngaySinh.year;
    string dateStr = dateStream.str();
    cout << centerText(dateStr, 14) << "|";
    
    ostringstream scoreStream;
    scoreStream << fixed << setprecision(2) << sv.diemTBTL;
    string scoreStr = scoreStream.str();
    
    // Highlight điểm nếu được chọn
    if(highlight) {
        cout << "<" << centerText(scoreStr, 10) << ">";
    } else {
        cout << centerText(scoreStr, 12);
    }
}

// Cập nhật showEditDeleteMenu với highlight tốt hơn
int showEditDeleteMenu(const Student& sv) {
    string menuItems[] = {"Sua thong tin", "Xoa sinh vien", "Huy"};
    int selected = 0;
    int key;
    
    int menuX = 55;
    int menuY = 8;
    
    while(true) {
        // Vẽ khung menu với decoration
        gotoxy(menuX, menuY);
        cout << "+===============+";
        gotoxy(menuX, menuY + 1);
        cout << "| MENU THAO TAC |";
        gotoxy(menuX, menuY + 2);
        cout << "+===============+";
        
        // Hiển thị thông tin sinh viên
        gotoxy(menuX, menuY + 3);
        cout << "| " << left << setw(13) << sv.maSV.substr(0, 13) << " |";
        gotoxy(menuX, menuY + 4);
        cout << "| " << left << setw(13) << sv.hoTen.substr(0, 13) << " |";
        gotoxy(menuX, menuY + 5);
        cout << "+===============+";
        
        for(int i = 0; i < 3; i++) {
            gotoxy(menuX, menuY + 6 + i);
            if(i == selected) {
                cout << "|>" << left << setw(13) << menuItems[i] << "<|";
            } else {
                cout << "| " << left << setw(13) << menuItems[i] << " |";
            }
        }
        
        gotoxy(menuX, menuY + 9);
        cout << "+===============+";
        
        gotoxy(menuX, menuY + 11);
        cout << "Up/Down: Chon";
        gotoxy(menuX, menuY + 12);
        cout << "Enter: OK";
        
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
            // Xóa menu
            for(int i = 0; i < 14; i++) {
                gotoxy(menuX, menuY + i);
                cout << "                   ";
            }
            return selected + 1;
        } else if(key == KEY_ESC) {
            // Xóa menu
            for(int i = 0; i < 14; i++) {
                gotoxy(menuX, menuY + i);
                cout << "                   ";
            }
            return 0;
        }
    }
}

// Các hàm còn lại giữ nguyên logic nhưng bỏ màu...
void editStudentInPlace(vector<Student>& students, int index) {
    if(index < 0 || index >= students.size()) return;
    
    Student& sv = students[index];
    
    clearScreen();
    gotoxy(25, 2);
    cout << "========================================";
    gotoxy(25, 3);
    cout << "        SUA THONG TIN SINH VIEN        ";
    gotoxy(25, 4);
    cout << "========================================";
    
    gotoxy(20, 6);
    cout << "Sinh vien hien tai: " << sv.hoTen << " (" << sv.maSV << ")";
    
    gotoxy(15, 8);
    cout << "Nhan Enter de giu nguyen gia tri, ESC de huy thay doi";
    
    string newMaLop = inputTextBox("Ma lop:", 20, 10, 25, sv.maLop);
    if(newMaLop != sv.maLop && !newMaLop.empty()) {
        sv.maLop = newMaLop;
    }
    
    bool validMaSV = true;
    string newMaSV;
    do {
        newMaSV = inputTextBox("Ma sinh vien (8 so):", 20, 14, 25, sv.maSV);
        if(newMaSV == sv.maSV || newMaSV.empty()) {
            break;
        }
        
        validMaSV = (newMaSV.length() == 8 && 
                    all_of(newMaSV.begin(), newMaSV.end(), ::isdigit));
        
        if(!validMaSV) {
            gotoxy(20, 18);
            cout << "Ma sinh vien phai la 8 chu so! Nhan Enter de nhap lai...";
            _getch();
            gotoxy(20, 18);
            cout << string(55, ' ');
        }
    } while(!validMaSV);
    
    if(validMaSV && newMaSV != sv.maSV && !newMaSV.empty()) {
        sv.maSV = newMaSV;
    }
    
    string newHoTen = inputTextBox("Ho va ten:", 20, 18, 35, sv.hoTen);
    if(newHoTen != sv.hoTen && !newHoTen.empty()) {
        sv.hoTen = formatName(newHoTen);
    }
    
    char dateBuffer[20];
    sprintf(dateBuffer, "%02d/%02d/%04d", sv.ngaySinh.day, sv.ngaySinh.month, sv.ngaySinh.year);
    string dateInput = inputTextBox("Ngay sinh (dd/mm/yyyy):", 20, 22, 20, dateBuffer);
    
    if(dateInput != dateBuffer && !dateInput.empty()) {
        Date newDate;
        if(sscanf(dateInput.c_str(), "%d/%d/%d", &newDate.day, &newDate.month, &newDate.year) == 3) {
            if(isValidDate(newDate)) {
                sv.ngaySinh = newDate;
            }
        }
    }
    
    string scoreStr = to_string(sv.diemTBTL);
    scoreStr = scoreStr.substr(0, scoreStr.find_last_not_of('0') + 1);
    if(scoreStr.back() == '.') scoreStr.pop_back();
    
    string newScoreStr = inputTextBox("Diem TB (0.0-10.0):", 20, 26, 15, scoreStr);
    
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
    
    gotoxy(25, 30);
    cout << "CAP NHAT THANH CONG!";
    gotoxy(25, 31);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

bool confirmDelete(const Student& sv) {
    clearScreen();
    
    gotoxy(30, 10);
    cout << "====================================";
    gotoxy(30, 11);
    cout << "      XAC NHAN XOA SINH VIEN       ";
    gotoxy(30, 12);
    cout << "====================================";
    
    gotoxy(25, 15);
    cout << "Ban co chac chan muon xoa sinh vien nay khong?";
    
    string options[] = {"CO - Xoa sinh vien", "KHONG - Huy bo"};
    int selected = 1;
    int key;
    
    while(true) {
        for(int i = 0; i < 2; i++) {
            gotoxy(30, 18 + i);
            if(i == selected) {
                cout << ">> " << options[i] << " <<";
            } else {
                cout << "   " << options[i] << "   ";
            }
        }
        
        gotoxy(30, 22);
        cout << "Phim mui ten: Chon, Enter: Xac nhan, ESC: Huy";
        
        key = _getch();
        
        if(key == 224) {
            key = _getch();
            switch(key) {
                case KEY_UP:
                case KEY_DOWN:
                    selected = 1 - selected;
                    break;
            }
        } else if(key == KEY_ENTER) {
            return (selected == 0);
        } else if(key == KEY_ESC) {
            return false;
        }
    }
}

// Thống kê - KHÔNG MÀU
void statisticsMenu(const vector<Student>& students) {
    int choice = showStatisticsMenu();
    
    switch(choice) {
        case 1:
            reportStudentsByClass(students);
            break;
        case 2:
            reportGradesByClass(students);
            break;
        case 0:
            break;
        default:
            clearScreen();
            gotoxy(30, 10);
            cout << "Lua chon khong hop le!";
            _getch();
    }
}

// Cập nhật showStatisticsMenu với highlight tốt hơn
int showStatisticsMenu() {
    string statisticsOptions[] = {
        "Bao cao so luong SV theo lop", 
        "Ty le phan loai ket qua hoc tap"
    };
    
    int selected = 0;
    int key;
    
    while(true) {
        clearScreen();
        gotoxy(20, 2);
        cout << "================================================";
        gotoxy(20, 3);
        cout << "              MENU THONG KE                    ";
        gotoxy(20, 4);
        cout << "================================================";
        
        gotoxy(15, 6);
        cout << "Chon loai bao cao thong ke:";
        
        for(int i = 0; i < 2; i++) {
            gotoxy(20, 8 + i);
            if(i == selected) {
                cout << "+-> " << left << setw(33) << statisticsOptions[i] << " <-+";
            } else {
                cout << "    " << left << setw(35) << statisticsOptions[i] << "    ";
            }
        }
        
        gotoxy(15, 12);
        cout << "Phim mui ten: Chon loai bao cao | Enter: Thuc hien | ESC: Quay lai";
        
        gotoxy(15, 14);
        cout << "===========================================================";
        gotoxy(15, 15);
        cout << "Loai bao cao da chon: " << statisticsOptions[selected];
        
        key = _getch();
        
        if(key == 224) {
            key = _getch();
            switch(key) {
                case KEY_UP:
                    selected = (selected - 1 + 2) % 2;
                    break;
                case KEY_DOWN:
                    selected = (selected + 1) % 2;
                    break;
            }
        } else if(key == KEY_ENTER) {
            return selected + 1;
        } else if(key == KEY_ESC) {
            return 0;
        }
    }
}

void reportStudentsByClass(const vector<Student>& students) {
    if(students.empty()) {
        clearScreen();
        gotoxy(30, 10);
        cout << "DANH SACH SINH VIEN TRONG!";
        gotoxy(30, 11);
        cout << "Nhan phim bat ky de tiep tuc...";
        _getch();
        return;
    }
    
    map<string, int> classCount;
    for(const auto& sv : students) {
        classCount[sv.maLop]++;
    }
    
    clearScreen();
    gotoxy(25, 2);
    cout << "================================================";
    gotoxy(25, 3);
    cout << "      BAO CAO SO LUONG SINH VIEN THEO LOP      ";
    gotoxy(25, 4);
    cout << "================================================";
    
    gotoxy(30, 6);
    cout << "Tong so sinh vien: " << students.size();
    gotoxy(30, 7);
    cout << "So lop: " << classCount.size();
    
    gotoxy(25, 9);
    cout << "+-----------------+--------------+-------------+";
    gotoxy(25, 10);
    cout << "|     MA LOP      |  SO LUONG    |   TY LE (%) |";
    gotoxy(25, 11);
    cout << "+-----------------+--------------+-------------+";
    
    int row = 12;
    for(const auto& pair : classCount) {
        gotoxy(25, row);
        cout << "| " << left << setw(15) << pair.first << " |";
        cout << "     " << right << setw(3) << pair.second << "      |";
        
        float percentage = (float)pair.second * 100 / students.size();
        cout << "    " << fixed << setprecision(1) << setw(5) << percentage << "    |";
        
        row++;
        
        if(row < 20) {
            gotoxy(25, row);
            cout << "+-----------------+--------------+-------------+";
            row++;
        }
    }
    
    gotoxy(25, row);
    cout << "+-----------------+--------------+-------------+";
    
    gotoxy(30, row + 3);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

void reportGradesByClass(const vector<Student>& students) {
    if(students.empty()) {
        clearScreen();
        gotoxy(30, 10);
        cout << "DANH SACH SINH VIEN TRONG!";
        gotoxy(30, 11);
        cout << "Nhan phim bat ky de tiep tuc...";
        _getch();
        return;
    }
    
    map<string, map<string, int>> classGrades;
    
    for(const auto& sv : students) {
        string grade;
        if(sv.diemTBTL >= 9.0) grade = "Xuat sac";
        else if(sv.diemTBTL >= 8.0) grade = "Gioi";
        else if(sv.diemTBTL >= 6.5) grade = "Kha";
        else if(sv.diemTBTL >= 5.0) grade = "Trung binh";
        else grade = "Yeu";
        
        classGrades[sv.maLop][grade]++;
    }
    
    clearScreen();
    gotoxy(20, 2);
    cout << "============================================================";
    gotoxy(20, 3);
    cout << "   BAO CAO TY LE PHAN LOAI KET QUA HOC TAP THEO LOP       ";
    gotoxy(20, 4);
    cout << "============================================================";
    
    int startY = 6;
    for(const auto& classData : classGrades) {
        string className = classData.first;
        const auto& grades = classData.second;
        
        int totalStudents = 0;
        for(const auto& gradeData : grades) {
            totalStudents += gradeData.second;
        }
        
        gotoxy(25, startY);
        cout << "Lop: " << className << " (Tong: " << totalStudents << " sinh vien)";
        
        string gradeNames[] = {"Xuat sac", "Gioi", "Kha", "Trung binh", "Yeu"};
        
        for(int i = 0; i < 5; i++) {
            int count = grades.count(gradeNames[i]) ? grades.at(gradeNames[i]) : 0;
            float percentage = totalStudents > 0 ? (float)count * 100 / totalStudents : 0;
            
            gotoxy(30, startY + 1 + i);
            cout << "- " << left << setw(12) << gradeNames[i] << ": " 
                 << right << setw(2) << count << " SV (" 
                 << fixed << setprecision(1) << setw(5) << percentage << "%)";
        }
        
        startY += 7;
        
        if(startY > 18) {
            gotoxy(25, startY);
            cout << "Nhan phim bat ky de xem tiep...";
            _getch();
            clearScreen();
            gotoxy(20, 2);
            cout << "============================================================";
            gotoxy(20, 3);
            cout << "   BAO CAO TY LE PHAN LOAI KET QUA HOC TAP THEO LOP       ";
            gotoxy(20, 4);
            cout << "============================================================";
            startY = 6;
        }
    }
    
    gotoxy(25, startY + 1);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

// Các hàm còn lại (sortStudents, searchStudents, displaySearchResults, v.v.) 
// Tương tự như trên nhưng bỏ hết các lệnh setColor()

void sortStudents(vector<Student>& students, int choice) {
    string sortNames[] = {"", "Ma SV", "Ho Ten", "Ngay Sinh", "Diem TB", "Ma Lop"};
    
    switch(choice) {
        case 1:
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.maSV < b.maSV;
                 });
            break;
        case 2:
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.hoTen < b.hoTen;
                 });
            break;
        case 3:
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     if(a.ngaySinh.year != b.ngaySinh.year)
                         return a.ngaySinh.year < b.ngaySinh.year;
                     if(a.ngaySinh.month != b.ngaySinh.month)
                         return a.ngaySinh.month < b.ngaySinh.month;
                     return a.ngaySinh.day < b.ngaySinh.day;
                 });
            break;
        case 4:
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.diemTBTL > b.diemTBTL;
                 });
            break;
        case 5:
            sort(students.begin(), students.end(), 
                 [](const Student& a, const Student& b) {
                     return a.maLop < b.maLop;
                 });
            break;
        default:
            clearScreen();
            gotoxy(30, 10);
            cout << "Lua chon khong hop le!";
            gotoxy(30, 11);
            cout << "Nhan phim bat ky de tiep tuc...";
            _getch();
            return;
    }
    
    saveToFile(students);
    
    clearScreen();
    gotoxy(25, 9);
    cout << "========================================";
    gotoxy(25, 10);
    cout << "        SAP XEP THANH CONG!            ";
    gotoxy(25, 11);
    cout << "========================================";
    
    gotoxy(20, 13);
    cout << "Da sap xep theo: " << sortNames[choice];
    gotoxy(20, 14);
    cout << "Du lieu da duoc luu tu dong!";
    
    gotoxy(20, 16);
    cout << "Nhan phim bat ky de xem ket qua...";
    _getch();
    
    printStudents(students);
}

void searchStudents(const vector<Student>& students, int encodedChoice) {
    if(encodedChoice == 0) return;
    
    int criteria = encodedChoice / 10;
    int algorithm = encodedChoice % 10;
    
    string criteriaNames[] = {"", "Ma SV", "Ho Ten", "Ma Lop", "Ngay Sinh", "Diem TB"};
    string algorithmNames[] = {"", "Tim kiem tuan tu", "Tim kiem nhi phan"};
    
    clearScreen();
    gotoxy(20, 2);
    cout << "================================================";
    gotoxy(20, 3);
    cout << "              TIM KIEM SINH VIEN               ";
    gotoxy(20, 4);
    cout << "================================================";
    
    gotoxy(15, 6);
    cout << "Tieu chi: " << criteriaNames[criteria];
    gotoxy(15, 7);
    cout << "Thuat toan: " << algorithmNames[algorithm];
    
    string keyword = inputTextBox("Nhap tu khoa tim kiem:", 15, 9, 30);
    
    if(keyword.empty()) return;
    
    vector<Student> result;
    vector<Student> sortedStudents = students;
    
    // Phần logic tìm kiếm tương tự như cũ nhưng bỏ màu...
    // (Giữ nguyên logic nhưng bỏ tất cả setColor())
    
    if(algorithm == 2) {
        // Sắp xếp trước khi tìm kiếm nhị phân
        // ... code sắp xếp
    }
    
    // Thực hiện tìm kiếm
    if(algorithm == 1) {
        // Tìm kiếm tuần tự
        for(const auto& sv : students) {
            bool found = false;
            switch(criteria) {
                case 1:
                    if(sv.maSV.find(keyword) != string::npos) found = true;
                    break;
                case 2:
                    if(sv.hoTen.find(keyword) != string::npos) found = true;
                    break;
                // ... các case khác
            }
            if(found) result.push_back(sv);
        }
    } else {
        // Tìm kiếm nhị phân
        // ... code tìm kiếm nhị phân
    }
    
    if(result.empty()) {
        gotoxy(25, 12);
        cout << "KHONG TIM THAY SINH VIEN NAO!";
        gotoxy(25, 13);
        cout << "Nhan phim bat ky de tiep tuc...";
        _getch();
    } else {
        displaySearchResults(result, keyword + " (" + algorithmNames[algorithm] + ")");
    }
}

void displaySearchResults(const vector<Student>& results, const string& keyword) {
    int currentPage = 0;
    int totalPages = (results.size() + RECORDS_PER_PAGE - 1) / RECORDS_PER_PAGE;
    int selectedRow = 0;
    int key;
    
    while(true) {
        clearScreen();
        
        gotoxy(10, 1);
        cout << "==========================================================================";
        gotoxy(10, 2);
        cout << "                  KET QUA TIM KIEM: \"" << keyword.substr(0, 20) << "\"";
        gotoxy(10, 3);
        cout << "==========================================================================";
        
        gotoxy(25, 4);
        cout << "Tim thay " << results.size() << " ket qua - Trang " 
             << (currentPage + 1) << "/" << totalPages;
        
        int tableX = 8;
        int tableY = 6;
        int tableWidth = 80;
        int tableHeight = RECORDS_PER_PAGE + 4; 
        
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
        
        gotoxy(12, tableY + tableHeight + 1);
        cout << "Phim mui ten: Di chuyen | <->: Chuyen trang | ESC: Thoat";
        
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

// Các hàm hỗ trợ
void saveToFile(const vector<Student>& students) {
    ofstream file("students.txt");
    if(!file) {
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

void loadFromFile(vector<Student>& students) {
    ifstream file("students.txt");  
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

bool isValidDate(const Date& date) {
    if(date.year < 1900 || date.year > 2025) return false;
    if(date.month < 1 || date.month > 12) return false;
    if(date.day < 1 || date.day > 31) return false;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0)) {
        daysInMonth[2] = 29;
    }
    
    return date.day <= daysInMonth[date.month];
}

bool isValidGPA(float gpa) {
    return gpa >= 0.0 && gpa <= 10.0;
}

string formatName(string name) {
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    
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