#define _CRT_SECURE_NO_WARNINGS
#include "guest_room.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <memory>
#include <ctime>
#include <iomanip>

using namespace std;

// 定义字符指针，指向一个文件
const char* const room_data_file = "rooms.csv";

// GuestRoom 构造函数实现
// 默认构造函数
GuestRoom::GuestRoom()
    : m_number("Unknown"), m_name("Unknown"), m_price(0), m_bed_number(0), m_area(0), m_state(ROOM_STATE::FREE) {}

// 带参数的构造函数（仅房间号和状态）
GuestRoom::GuestRoom(const std::string& number, ROOM_STATE state)
    : m_number(number), m_state(state) {}

// 完整的构造函数（包含房间名称、价格、床位数量、面积和状态）
GuestRoom::GuestRoom(const std::string& name, int price, int bed_num, int area, ROOM_STATE state)
    : m_name(name), m_price(price), m_bed_number(bed_num), m_area(area), m_state(state) {
    // 生成房间号，假设默认楼层为 1
    m_number = generate_number(1);
}

// 在 GuestRoom 类中添加 load_from_csv 静态方法
// 从 CSV 文件中加载客房数据，并返回智能指针管理的 map
std::map<std::string, std::shared_ptr<GuestRoom>> GuestRoom::load_from_csv(const char* filename) {
    std::map<std::string, std::shared_ptr<GuestRoom>> room_list;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "无法打开文件进行加载: " << filename << std::endl;
        return room_list; // 返回空的 map
    }

    std::string line;
    // 跳过第一行（表头）
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string room_number_str, name_str, price_str, bed_num_str, area_str, state_str, registration_time_str;

        // 用逗号分隔每一行的字段
        if (std::getline(ss, room_number_str, ',') &&
            std::getline(ss, name_str, ',') &&
            std::getline(ss, price_str, ',') &&
            std::getline(ss, bed_num_str, ',') &&
            std::getline(ss, area_str, ',') &&
            std::getline(ss, state_str, ',') &&
            std::getline(ss, registration_time_str, ',')) {
            try {
                // 解析房间信息
                int price = std::stoi(price_str);
                int bed_num = std::stoi(bed_num_str);
                int area = std::stoi(area_str);
                int state = std::stoi(state_str);
                ROOM_STATE room_state = static_cast<ROOM_STATE>(state);

                // 使用 make_shared 创建智能指针并加入 map
                auto room = std::make_shared<GuestRoom>(name_str, price, bed_num, area, room_state);
                room->set_number(room_number_str);
                room->m_registration_time = registration_time_str; // 设置登记时间
                room_list[room_number_str] = room;
            } catch (const std::exception& e) {
                std::cerr << "解析 CSV 文件时出错: " << e.what() << " (行: " << line << ")" << std::endl;
                continue;
            }
        } else {
            std::cerr << "无效的 CSV 行: " << line << std::endl;
            continue;
        }
    }

    file.close();
    std::cout << "客房数据已成功从 " << filename << " 加载" << std::endl;
    return room_list;
}

// 在 GuestRoom 类中添加 save_to_csv 静态方法
void GuestRoom::save_to_csv(const std::map<std::string, GuestRoom>& room_list, const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件进行保存: " << filename << std::endl;
        return;
    }

    // 写入 CSV 文件的表头
    file << "Room Number,Name,Price,Bed Number,Area,State,Registration Time\n";

    // 将每个房间的数据写入文件
    for (const auto& pair : room_list) {
        const GuestRoom& room = pair.second;
        file << room.get_number() << ","
             << room.get_name() << ","
             << room.get_price() << ","
             << room.get_bed_number() << ","
             << room.get_area() << ","
             << static_cast<int>(room.get_state()) << ","
             << room.get_registration_time() << "\n";
    }

    file.close();
    std::cout << "客房数据已成功保存到 " << filename << std::endl;
}
// 生成客房编号
// 生成客房编号（楼层加三位数字的顺序编号）
std::string GuestRoom::generate_number(int floor) {
    static std::map<int, int> floor_counter; // 记录每层的房间数量

    // 如果该楼层尚未记录，初始化计数器为 0
    if (floor_counter.find(floor) == floor_counter.end()) {
        floor_counter[floor] = 0;
    }

    // 生成编号（例如：01-001）
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << floor << "-"
        << std::setw(3) << std::setfill('0') << ++floor_counter[floor];

    // 获取当前时间作为登记时间
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    m_registration_time = buffer;

    return oss.str();
}

// 客房状态显示
std::string GuestRoom::show_state() const {
    switch (m_state) {
        case ROOM_STATE::FREE: return "空闲";
        case ROOM_STATE::BOOKED: return "已预订";
        case ROOM_STATE::CHECK_IN: return "入住";
        case ROOM_STATE::MAINTENANCE: return "维修";
        default: return "未知状态";
    }
}

// 获取客房编号
const std::string& GuestRoom::get_number() const {
    return m_number;
}

// 设置客房状态
void GuestRoom::set_state(ROOM_STATE state) {
    m_state = state;
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
//获取登记时间
const std::string& GuestRoom::get_registration_time() const {
    return m_registration_time;
}
