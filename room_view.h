#pragma once
#include "room_view.h"
#include "room_manager.h"

class RoomView {
public:
    void clear_screen(); // 清空屏幕
    void refresh_interface(); // 刷新界面
    void show_menu();
    void add_room(bool is_admin = false);
    void set_room_state_for_backend();
    void add_room_for_frontend(); // 前台添加客房信息
    void set_room_state_for_frontend();
    void frontend_menu();
    void display_rooms(bool show_prompt = true); // 新增：通用的显示客房信息方法
    void remove_data();
    void set_room_state(GuestRoom& room, bool is_admin = false); // 增加 is_admin 参数
    void show_all();
    void show_all_rooms();
    void manage_backend();
    void show_available_rooms();
    void run();
    void set_room();
    
    bool authenticate_admin(); // 管理员身份验证
    std::string get_password(); // 获取密码并显示 *
private:
    GuestRoomManager m_grm;
    const std::string ADMIN_PASSWORD = "admin123"; // 管理员密码
};