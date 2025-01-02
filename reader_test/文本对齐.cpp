#include <iostream>
#include <iomanip>  // 包含 iomanip 头文件
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// 定义一个结构体来存储客房信息
struct GuestRoom {
    string number;
    string name;
    int area;
    int price;
    int bed_num;
    string state;

    // 构造函数
    GuestRoom(const string& num, const string& n, int a, int p, int b, const string& s)
        : number(num), name(n), area(a), price(p), bed_num(b), state(s) {}
};

// 打印表头
void print_header() {
    cout << "-----------------所有客房信息-----------------" << endl;
    cout << left << setw(8) << "编号" 
         << setw(12) << "名称" 
         << setw(8) << "面积" 
         << setw(8) << "价格" 
         << setw(12) << "床位数量" 
         << setw(8) << "状态" 
         << endl;
    cout << "---------------------------------------------" << endl;
}

// 打印一条客房信息
void print_room(const GuestRoom& room) {
    cout << left << setw(8) << room.number
         << setw(12) << room.name
         << setw(8) << room.area
         << setw(8) << room.price
         << setw(12) << room.bed_num
         << setw(8) << room.state
         << endl;
}

// 打印所有客房信息
void print_all_rooms(const vector<GuestRoom>& rooms) {
    if (rooms.empty()) {
        cout << "请添加客房信息后再进行操作!" << endl;
        return;
    }

    print_header();
    for (const auto& room : rooms) {
        print_room(room);
    }
    cout << "---------------------------------------------" << endl;
}

// 模拟从文件读取客房信息（假设文件格式为逗号分隔）
vector<GuestRoom> load_rooms_from_file(const string& filename) {
    vector<GuestRoom> rooms;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string number, name, state;
        int area, price, bed_num;

        // 使用逗号分隔符读取每一行的数据
        if (getline(ss, number, ',') &&
            getline(ss, name, ',') &&
            (ss >> area) && (ss.ignore()) &&  // 忽略逗号
            (ss >> price) && (ss.ignore()) &&
            (ss >> bed_num) && (ss.ignore()) &&
            getline(ss, state)) {
            // 验证数据有效性
            if (number.empty() || name.empty() || state.empty() ||
                area <= 0 || price <= 0 || bed_num <= 0) {
                cout << "Invalid data format in file: " << line << endl;
                continue;
            }

            // 添加有效的客房信息到向量中
            rooms.emplace_back(number, name, area, price, bed_num, state);
        } else {
            cout << "Invalid data format in file: " << line << endl;
        }
    }

    return rooms;
}

int main() {
    // 模拟从文件加载客房信息
    vector<GuestRoom> rooms = load_rooms_from_file("rooms.txt");

    // 打印所有客房信息
    print_all_rooms(rooms);

    // 显示菜单
    cout << "-----------------酒店管理系统-----------------" << endl;
    cout << "*********  1. 显示客房信息   *********" << endl;
    cout << "*********  2. 设置客房状态   *********" << endl;
    cout << "*********  3. 删除客房信息   *********" << endl;
    cout << "*********  4. 添加客房信息   *********" << endl;
    cout << "*********  5. 退出管理系统   *********" << endl;
    cout << "请输入您的操作：";

    return 0;
}