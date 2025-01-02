#pragma once

#include "guest_room.h"
#include <map>
#include <string>
#include <memory>

class GuestRoomManager {
public:
    const std::map<std::string, std::shared_ptr<GuestRoom>>& get_rooms() const;
    std::map<std::string, std::shared_ptr<GuestRoom>>& get_rooms(); // 新增的非 const 方法
    GuestRoom* find_room_by_identifier(const std::string& identifier);
    GuestRoomManager();
    bool check_in(const std::string& room_number);
    void add_room(std::shared_ptr<GuestRoom> room); // 声明带参数
    bool remove_data(const std::string& identifier);
    void set_room_state(const std::string& identifier, ROOM_STATE state);
    bool save_data();
    static std::string get_state_string(ROOM_STATE state);
    const std::string ROOM_DATA_FILE = "rooms.csv";
    void ensureFileExists(const std::string& filename); // 添加 ensureFileExists 函数
private:
    std::map<std::string, std::shared_ptr<GuestRoom>> m_room_list;
    const std::string room_data_file = "rooms.csv";
};