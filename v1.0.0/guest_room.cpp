#define _CRT_SECURE_NO_WARNINGS
#include "guest_room.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// 定义字符指针，指向一个文件
const char* const room_data_file = "room.dat";

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
    if (m_state == FREE) {
        return "空闲";
    } else if (m_state == CHECK_IN) {
        return "入住";
    } else {
        return "未知状态";
    }
}

// 保存数据 (map 容器，key：客房编号，value：客房对象)
bool GuestRoom::save_data(map<string, GuestRoom>& room_list) {
    // 1. 打开文件
    ofstream ofs(room_data_file, ios::binary | ios::trunc);
    if (!ofs) {
        cerr << "无法打开文件: " << room_data_file << endl;
        return false;
    }

    // 2. 写入对象数据
    for (const auto& pair : room_list) {
        const GuestRoom& room = pair.second;

        // 写入 m_name 成员变量所占内存大小及其值
        size_t name_len = room.m_name.size();
        ofs.write(reinterpret_cast<const char*>(&name_len), sizeof(size_t));
        ofs.write(room.m_name.c_str(), name_len);

        // 写入 m_price 成员变量
        ofs.write(reinterpret_cast<const char*>(&room.m_price), sizeof(int));

        // 写入 m_bed_number 成员变量
        ofs.write(reinterpret_cast<const char*>(&room.m_bed_number), sizeof(int));

        // 写入 m_area 成员变量
        ofs.write(reinterpret_cast<const char*>(&room.m_area), sizeof(int));

        // 写入 m_number 成员变量所占内存大小及其值
        size_t number_len = room.m_number.size();
        ofs.write(reinterpret_cast<const char*>(&number_len), sizeof(size_t));
        ofs.write(room.m_number.c_str(), number_len);

        // 写入 m_state 成员变量
        ofs.write(reinterpret_cast<const char*>(&room.m_state), sizeof(ROOM_STATE));
    }

    // 3. 关闭文件
    ofs.close();
    return true;
}

// 读取数据
map<string, GuestRoom> GuestRoom::read_data() {
    map<string, GuestRoom> room_list;
    ifstream ifs(room_data_file, ios::binary);
    if (!ifs) {
        cerr << "无法打开文件: " << room_data_file << endl;
        return room_list;
    }

    while (ifs.peek() != EOF) {
        GuestRoom room;

        // 读入 m_name 成员变量所占内存大小及其值
        size_t name_len = 0;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(size_t));
        if (ifs.eof()) break;  // 检查是否到达文件末尾
        vector<char> name_buffer(name_len + 1, '\0');  // 动态分配缓冲区
        ifs.read(&name_buffer[0], name_len);
        room.m_name.assign(&name_buffer[0], name_len);  // 使用 assign 而不是直接赋值

        // 读入 m_price 成员变量
        ifs.read(reinterpret_cast<char*>(&room.m_price), sizeof(int));

        // 读入 m_bed_number 成员变量
        ifs.read(reinterpret_cast<char*>(&room.m_bed_number), sizeof(int));

        // 读入 m_area 成员变量
        ifs.read(reinterpret_cast<char*>(&room.m_area), sizeof(int));

        // 读入 m_state 成员变量
        ifs.read(reinterpret_cast<char*>(&room.m_state), sizeof(ROOM_STATE));

        // 读入 m_number 成员变量所占内存大小及其值
        size_t number_len = 0;
        ifs.read(reinterpret_cast<char*>(&number_len), sizeof(size_t));
        if (ifs.eof()) break;  // 检查是否到达文件末尾
        vector<char> number_buffer(number_len + 1, '\0');  // 动态分配缓冲区
        ifs.read(&number_buffer[0], number_len);
        room.m_number.assign(&number_buffer[0], number_len);  // 使用 assign 而不是直接赋值

        // 将读取的房间信息插入到 map 中
        room_list.insert(make_pair(room.m_number, room));
    }

    // 3. 关闭文件
    ifs.close();
    return room_list;
}

// 获取客房编号
string GuestRoom::get_num() {
    return m_number;
}

// 设置客房状态
void GuestRoom::set_state() {
    m_state = CHECK_IN;
}

// 获取客房名称
string GuestRoom::get_name() {
    return m_name;
}

// 获取客房价格
int GuestRoom::get_price() {
    return m_price;
}

// 获取客房面积
int GuestRoom::get_area() {
    return m_area;
}

// 获取客房床位数量
int GuestRoom::get_bed_num() {
    return m_bed_number;
}