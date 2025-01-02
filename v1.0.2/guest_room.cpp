#define _CRT_SECURE_NO_WARNINGS
#include "guest_room.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;

// 定义字符指针，指向一个文件
const char* const room_data_file = "rooms.txt";

// GuestRoom 构造函数实现
GuestRoom::GuestRoom(string name, int price, int bed_num, int area, ROOM_STATE state)
    : m_name(name), m_price(price), m_bed_number(bed_num), m_area(area), m_state(state) {
    this->m_number = generate_number();
}

// 生成客房编号
string GuestRoom::generate_number() {
    // 本地时间转换字符串
    time_t my_time = time(NULL);
    struct tm* my_tm = localtime(&my_time);
    char tim_buff[128] = { 0 };
    sprintf(tim_buff, "%d%d", my_tm->tm_yday, my_tm->tm_sec);

    // 生成随机数
    static bool is_seeded = false;
    if (!is_seeded) {
        srand(static_cast<unsigned>(time(NULL)));
        is_seeded = true;
    }
    int rand_num = rand() % 50;
    char buf[128] = { 0 };
    sprintf(buf, "%d", rand_num);

    // 拼接字符串作为房间编号
    return string(tim_buff) + string(buf);
}

// 客房状态显示
string GuestRoom::show_state() {
    if (m_state == ROOM_STATE::FREE) {
        return "空闲";
    } else if (m_state == ROOM_STATE::CHECK_IN) {
        return "入住";
    } else {
        return "未知状态";
    }
}

// 保存数据 (map 容器，key：客房编号，value：客房对象)
bool GuestRoom::save_data(const map<string, GuestRoom>& room_list) {
    ofstream ofs(room_data_file);
    if (!ofs.is_open()) {
        cerr << "无法打开文件: " << room_data_file << endl;
        return false;
    }

    for (const auto& pair : room_list) {
        const GuestRoom& room = pair.second;
        ofs << room.get_number() << "," 
            << room.get_name() << "," 
            << room.get_price() << "," 
            << room.get_bed_number() << "," 
            << room.get_area() << "," 
            << static_cast<int>(room.get_state()) << "\n";

        if (ofs.fail()) {  // 检查每次写入是否成功
            cerr << "写入文件时出错: " << room_data_file << endl;
            ofs.close();
            return false;
        }
    }

    ofs.flush();  // 确保所有数据都被写入文件
    ofs.close();
    return true;
}

// 读取数据
map<string, GuestRoom> GuestRoom::read_data() {
    map<string, GuestRoom> room_list;
    ifstream ifs(room_data_file);
    if (!ifs.is_open()) {
        cerr << "无法打开文件: " << room_data_file << endl;
        return room_list;
    }

    string line;
    while (getline(ifs, line)) {
        stringstream ss(line);
        string number, name;
        int price, bed_num, area, state;

        if (getline(ss, number, ',') && 
            getline(ss, name, ',') && 
            (ss >> price) && 
            (ss >> bed_num) && 
            (ss >> area) && 
            (ss >> state)) {

            // 忽略剩余的逗号和可能存在的空格
            ss.ignore(numeric_limits<streamsize>::max(), '\n');

            GuestRoom room(name, price, bed_num, area, static_cast<ROOM_STATE>(state));
            room.set_number(number);  // 使用 set_number 设置房间编号
            room_list.insert(make_pair(number, room));

            cout << "Read room: " << number << " - " << name << endl;  // 调试输出
        } else {
            cerr << "Invalid data format in file: " << line << endl;
        }
    }

    ifs.close();
    return room_list;
}

// 获取客房编号
const std::string& GuestRoom::get_number() const {
    return m_number;
}

// 设置客房状态
void GuestRoom::set_state() {
    m_state = ROOM_STATE::CHECK_IN;
}

// 获取客房名称
const std::string& GuestRoom::get_name() const {
    return m_name;
}
// 获取客房价格
int GuestRoom::get_price() const {
    return m_price;
}

// 获取客房面积
int GuestRoom::get_area() const {
    return m_area;
}

// 获取客房床位数量
int GuestRoom::get_bed_number() const {
    return m_bed_number;
}

//设置房间编号
void GuestRoom::set_number(const std::string& number) {
    m_number = number;
}


//获取客房状态
ROOM_STATE GuestRoom::get_state() const {
    return m_state;
}