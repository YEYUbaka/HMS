#pragma once

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <memory>

// 定义全局常量
const std::string ROOM_DATA_FILE = "rooms.csv";

// 定义枚举类型，表示客房状态：空闲、入住、已预订、维修
enum class ROOM_STATE {
    FREE = 1,       // 空闲
    CHECK_IN = 2,   // 入住
    BOOKED = 3,     // 已预订
    MAINTENANCE = 4 // 维修
};

// 客房类
class GuestRoom {
public:
    GuestRoom(const std::string& number, ROOM_STATE state);
    GuestRoom(const std::string& name, int price, int bed_num, int area, ROOM_STATE state);
    GuestRoom();

    std::string generate_number(int floor); 
    std::string show_state() const;

    static std::map<std::string, std::shared_ptr<GuestRoom>> load_from_csv(const char* filename = ROOM_DATA_FILE.c_str());
    static void save_to_csv(const std::map<std::string, GuestRoom>& room_list, const char* filename = ROOM_DATA_FILE.c_str());

    const std::string& get_number() const;
    const std::string& get_name() const;
    const std::string& get_registration_time() const; 
    int get_price() const;
    int get_area() const;
    int get_bed_number() const;
    void set_state(ROOM_STATE state);
    void set_number(const std::string& number);
    ROOM_STATE get_state() const;

private:
    std::string m_number;
    std::string m_name;
    int m_price;
    int m_bed_number;
    int m_area;
    ROOM_STATE m_state;
    std::string m_registration_time; 
};