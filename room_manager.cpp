#include "room_manager.h"
#include "guest_room.h"
#include <iostream>
#include <fstream>
#include <memory.h>
#include <limits>

using namespace std;


// 实现 get_rooms 成员函数
const std::map<std::string, std::shared_ptr<GuestRoom>>& GuestRoomManager::get_rooms() const {
    return m_room_list;
}
std::map<std::string, std::shared_ptr<GuestRoom>>& GuestRoomManager::get_rooms() {
    return m_room_list;
}
// 实现构造函数
GuestRoomManager::GuestRoomManager() {

    // 加载之前保存的客房数据，确保返回的是智能指针
    m_room_list = GuestRoom::load_from_csv(ROOM_DATA_FILE.c_str());
}

// 实现静态成员函数
std::string GuestRoomManager::get_state_string(ROOM_STATE state) {
    switch (state) {
        case ROOM_STATE::FREE:
            return "空闲";
        case ROOM_STATE::CHECK_IN:
            return "入住";
        case ROOM_STATE::BOOKED:
            return "已预订";
        case ROOM_STATE::MAINTENANCE:
            return "维修";
        default:
            return "未知状态";
    }
}

void GuestRoomManager::ensureFileExists(const std::string& filename) {
    std::ifstream file_check(filename);
    if (!file_check.good()) {
        std::ofstream file_create(filename);
        if (file_create) {
            file_create << "Room Number,Name,Price,Bed Number,Area,State,Registration Time\n";
            std::cout << "文件 " << filename << " 已创建。\n";
        } else {
            std::cerr << "无法创建文件 " << filename << "。\n";
        }
    } else {
        std::cout << "文件 " << filename << " 已存在。\n";
    }
}
// 实现 find_room_by_identifier 函数
GuestRoom* GuestRoomManager::find_room_by_identifier(const std::string& identifier) {
    for (auto& pair : m_room_list) {
        auto& room = pair.second; // 获取智能指针
        if (room->get_number() == identifier || room->get_name() == identifier) {
            return room.get(); // 返回原始指针
        }
    }
    return nullptr;
}

// 保存数据 (map 容器，key：客房编号，value：客房对象)
bool GuestRoomManager::save_data() {
    std::ofstream file(room_data_file, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Failed to open " << room_data_file << " for writing.\n";
        return false;
    }

    // 写入 CSV 文件的表头，包含“登记时间”字段
    file << "Room Number,Name,Price,Bed Number,Area,State,Registration Time\n";

    // 将每个房间的数据写入文件，包含“登记时间”字段
    for (const auto& pair : m_room_list) {
        const auto& room = pair.second; // 获取智能指针
        file << room->get_number() << ","
             << room->get_name() << ","
             << room->get_price() << ","
             << room->get_bed_number() << ","
             << room->get_area() << ","
             << static_cast<int>(room->get_state()) << ","
             << room->get_registration_time() << "\n";
    }

    file.close();
    return true;
}

// 实现 check_in 成员函数
bool GuestRoomManager::check_in(const std::string& number) {
    return m_room_list.find(number) != m_room_list.end();
}

// 实现 add_room 成员函数
void GuestRoomManager::add_room(std::shared_ptr<GuestRoom> room) {
    m_room_list[room->get_number()] = room;
    save_data(); // 保存更改
}

// 实现 remove_data 成员函数
bool GuestRoomManager::remove_data(const std::string& identifier) {
    // 使用 find_room_by_identifier 查找客房
    GuestRoom* room = find_room_by_identifier(identifier);
    if (!room) {
        std::cout << "未找到编号或名称为 " << identifier << " 的客房。\n";
        return false;  // 如果未找到客房，返回 false
    }

    // 删除客房
    m_room_list.erase(room->get_number());

    // 更新文件
    if (save_data()) {
        std::cout << "客房已成功删除。\n";
        return true;  // 如果保存成功，返回 true
    } else {
        std::cerr << "保存更改失败。\n";
        return false;  // 如果保存失败，返回 false
    }
}

// 实现 set_room_state 成员函数
void GuestRoomManager::set_room_state(const std::string& number, ROOM_STATE state) {
    // 进入函数时的日志
    std::cout << "进入 set_room_state 函数，房间号: " << number
              << ", 新状态: " << get_state_string(state) << std::endl;

    if (!check_in(number)) {
        std::cout << "错误: 房间编号 " << number << " 不存在" << std::endl;
        return;
    }

    ROOM_STATE old_state = m_room_list[number]->get_state();
    m_room_list[number]->set_state(state);

    // 成功设置状态后的日志
    std::cout << "成功将房间号 " << number
              << " 的状态从 " << get_state_string(old_state)
              << " 更改为 " << get_state_string(state) << std::endl;

    if (save_data()) {
        std::cout << "成功保存房间数据到文件" << std::endl;
    } else {
        std::cout << "警告: 保存房间数据到文件失败" << std::endl;
    }
}
