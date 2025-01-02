#include <iostream>
#include "room_view.h"
#include <iomanip>
#include <fstream>
#include <regex> // 用于正则表达式验证
#include <limits>// 用于 std::numeric_limits
#include <memory>
#include <cstdlib> // 用于 system 函数
#include <conio.h> // Windows 专用
#include <thread> // 用于 std::this_thread::sleep_for
#include <chrono> // 用于 std::chrono::seconds

using namespace std;


std::string RoomView::get_password() {
    std::string password;
    char ch;

    while ((ch = _getch()) != '\r') { // 读取字符直到按下回车
        if (ch == '\b') { // 处理退格键
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; // 删除一个 *
            }
        } else {
            password.push_back(ch);
            std::cout << '*'; // 显示 *
        }
    }

    std::cout << std::endl; // 换行
    return password;
}
void RoomView::clear_screen() {
#ifdef _WIN32
    system("cls"); // Windows 系统清屏
#else
    system("clear"); // Linux/Mac 系统清屏
#endif
}
void RoomView::refresh_interface() {
    std::cout.flush(); // 刷新输出缓冲区

#ifdef _WIN32
    system("cls"); // Windows 系统清屏
#else
    system("clear"); // Linux/Mac 系统清屏
#endif

    // 重新显示当前界面
    show_menu();
}
void RoomView::set_room_state_for_backend() {
    clear_screen(); // 清空屏幕
    show_all_rooms(); // 显示所有客房信息

    std::string input;
    std::cout << "请输入要设置的客房编号或名称（只含字母数字或其组合，或输入 'back' 返回）：";
    std::cin >> input;

    // 检查是否输入了 'back'
    if (input == "back") {
        return; // 返回到上一级菜单
    }

    // 验证输入是否只包含字母和数字
    if (!std::regex_match(input, std::regex("^[a-zA-Z0-9]+$"))) {
        std::cerr << "输入无效，请输入只含字母数字或其组合的编号或名称。\n";
        return;
    }

    // 查找匹配的房间
    std::vector<GuestRoom*> matched_rooms;
    for (auto& pair : m_grm.get_rooms()) {
        auto& room = pair.second;
        if (room->get_number() == input || room->get_name() == input) {
            matched_rooms.push_back(room.get());
        }
    }

    // 如果没有找到匹配的房间
    if (matched_rooms.empty()) {
        std::cerr << "未找到编号或名称为 " << input << " 的客房。\n";
        return;
    }

    // 如果找到多个匹配的房间（名称相同）
    if (matched_rooms.size() > 1) {
        std::cout << "找到多个名称为 " << input << " 的客房，请根据编号确认：\n";
        for (const auto& room : matched_rooms) {
            std::cout << "编号: " << room->get_number() << ", 名称: " << room->get_name()
                      << ", 状态: " << room->show_state() << "\n";
        }

        std::string room_number;
        std::cout << "请输入要设置的客房编号（或输入 'back' 返回）：";
        std::cin >> room_number;

        // 检查是否输入了 'back'
        if (room_number == "back") {
            return; // 返回到上一级菜单
        }

        // 根据编号再次查找
        auto it = std::find_if(matched_rooms.begin(), matched_rooms.end(),
                               [&room_number](const GuestRoom* room) {
                                   return room->get_number() == room_number;
                               });

        if (it == matched_rooms.end()) {
            std::cerr << "未找到编号为 " << room_number << " 的客房。\n";
            return;
        }

        // 设置房间状态
        set_room_state(**it, true); // 调用设置状态函数，is_admin 参数为 true
    } else {
        // 如果只有一个匹配的房间，直接设置状态
        set_room_state(*matched_rooms[0], true); // 调用设置状态函数，is_admin 参数为 true
    }
}
void RoomView::add_room_for_frontend() {
    std::string name;
    int price;
    int bed_num;
    int area;

    std::cout << "请输入客房名称（或输入 'back' 返回）：";
    std::cin >> name;

    // 检查是否输入了 'back'
    if (name == "back") {
        return; // 返回到上一级菜单
    }

    // 验证价格输入
    std::cout << "请输入客房价格（或输入 'back' 返回）：";
    while (!(std::cin >> price) || price <= 0) {
        std::cin.clear(); // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略无效输入
        std::cout << "输入无效，请输入正整数（或输入 'back' 返回）：";
        std::string input;
        std::cin >> input;
        if (input == "back") {
            return; // 返回到上一级菜单
        }
    }

    // 验证床数输入
    std::cout << "请输入客房床数（或输入 'back' 返回）：";
    while (!(std::cin >> bed_num) || bed_num <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效，请输入正整数（或输入 'back' 返回）：";
        std::string input;
        std::cin >> input;
        if (input == "back") {
            return; // 返回到上一级菜单
        }
    }

    // 验证面积输入
    std::cout << "请输入客房面积（或输入 'back' 返回）：";
    while (!(std::cin >> area) || area <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效，请输入正整数（或输入 'back' 返回）：";
        std::string input;
        std::cin >> input;
        if (input == "back") {
            return; // 返回到上一级菜单
        }
    }

    // 创建客房对象并使用智能指针管理，默认状态为“空闲”
    auto room = std::make_shared<GuestRoom>(name, price, bed_num, area, ROOM_STATE::FREE);

    // 添加客房
    m_grm.add_room(room); // 通过 m_grm 添加客房
    std::cout << "客房已成功添加。\n";
}
void RoomView::set_room_state_for_frontend() {
    clear_screen(); // 清空屏幕
    show_all_rooms(); // 显示所有客房信息

    std::string input;
    std::cout << "请输入要设置的客房编号或名称（只含字母数字或其组合，或输入 'back' 返回）：";
    std::cin >> input;

    // 检查是否输入了 'back'
    if (input == "back") {
        return; // 返回到上一级菜单
    }

    // 验证输入是否只包含字母和数字
    if (!std::regex_match(input, std::regex("^[a-zA-Z0-9]+$"))) {
        std::cerr << "输入无效，请输入只含字母数字或其组合的编号或名称。\n";
        std::cout << "按回车键返回...";
        std::cin.ignore(); // 忽略之前的输入
        std::cin.get();    // 等待用户按回车键
        return;
    }

    // 查找匹配的房间
    std::vector<GuestRoom*> matched_rooms;
    for (auto& pair : m_grm.get_rooms()) {
        auto& room = pair.second;
        if (room->get_number() == input || room->get_name() == input) {
            matched_rooms.push_back(room.get());
        }
    }

    // 如果没有找到匹配的房间
    if (matched_rooms.empty()) {
        std::cerr << "未找到编号或名称为 " << input << " 的客房。\n";
        std::cout << "按回车键返回...";
        std::cin.ignore(); // 忽略之前的输入
        std::cin.get();    // 等待用户按回车键
        return;
    }

    // 如果找到多个匹配的房间（名称相同）
    if (matched_rooms.size() > 1) {
        std::cout << "找到多个名称为 " << input << " 的客房，请根据编号确认：\n";
        for (const auto& room : matched_rooms) {
            std::cout << "编号: " << room->get_number() << ", 名称: " << room->get_name()
                      << ", 状态: " << room->show_state() << "\n";
        }

        std::string room_number;
        std::cout << "请输入要设置的客房编号（或输入 'back' 返回）：";
        std::cin >> room_number;

        // 检查是否输入了 'back'
        if (room_number == "back") {
            return; // 返回到上一级菜单
        }

        // 根据编号再次查找
        auto it = std::find_if(matched_rooms.begin(), matched_rooms.end(),
                               [&room_number](const GuestRoom* room) {
                                   return room->get_number() == room_number;
                               });

        if (it == matched_rooms.end()) {
            std::cerr << "未找到编号为 " << room_number << " 的客房。\n";
            std::cout << "按回车键返回...";
            std::cin.ignore(); // 忽略之前的输入
            std::cin.get();    // 等待用户按回车键
            return;
        }

        // 设置房间状态
        set_room_state(**it, false); // 调用设置状态函数，is_admin 参数为 false
    } else {
        // 如果只有一个匹配的房间，直接设置状态
        set_room_state(*matched_rooms[0], false); // 调用设置状态函数，is_admin 参数为 false
    }
}
void RoomView::frontend_menu() {
    bool in_frontend = true;
    while (in_frontend) {
        clear_screen(); // 清空屏幕
        cout << "-----------------前台专用-----------------" << endl;
        cout << "*********  1. 显示客房信息   *********" << endl;
        cout << "*********  2. 设置客房状态   *********" << endl;
        cout << "*********  3. 添加客房信息   *********" << endl;
        cout << "*********  4. 返回主菜单   *********" << endl;
        cout << "请输入您的操作：";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略无效输入
            std::cerr << "输入无效，请重新输入。\n";
            continue;
        }

        switch (choice) {
            case 1:
                show_all_rooms();
                break;
            case 2:
                set_room_state_for_frontend();
                break;
            case 3:
                add_room_for_frontend();
                break;
            case 4:
                clear_screen(); // 返回主菜单前清屏
                in_frontend = false; // 返回主菜单
                break;
            default:
                std::cerr << "无效的操作，请重新输入。\n";
                break;
        }
    }
}
void RoomView::display_rooms(bool show_prompt) {
    cout << "-----------------所有客房信息-----------------" << endl;
    cout << "编号" << setw(10) << "名称" << setw(12) << "面积" << setw(11) << "价格" << setw(15) << "床位数量" << setw(12) << "状态" << setw(20) << "登记时间" << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;

    // 使用 m_grm 获取房间信息
    const auto& rooms = m_grm.get_rooms();
    if (rooms.empty()) {
        cout << "暂无客房信息！" << endl;
    } else {
        for (const auto& room : rooms) {
            // 将数值和单位拼接成一个字符串
            string area = to_string(room.second->get_area()) + "平方";
            string price = to_string(room.second->get_price()) + "元";
            string bed_num = to_string(room.second->get_bed_number()) + "个";

            // 输出房间信息
            cout << setw(10) << left << room.second->get_number() << " "
                 << setw(10) << left << room.second->get_name() << " "
                 << setw(10) << left << area << " "
                 << setw(10) << left << price << " "
                 << setw(15) << left << bed_num << " "
                 << setw(9) << left << room.second->show_state() << " "
                 << setw(20) << left << room.second->get_registration_time() << endl;
        }
    }
    cout << "-----------------------------------------------------------------------------------------" << endl;

    // 如果需要显示提示信息
    if (show_prompt) {
        cout << "按回车键下一步...";
        cin.ignore(); // 忽略之前的输入
        cin.get();    // 等待用户按回车键
    }
}
void RoomView::show_all_rooms() {
    display_rooms(true); // 显示客房信息，并显示“按回车键返回”的提示
}
void RoomView::manage_backend() {
    bool in_backend = true;
    while (in_backend) {
        clear_screen(); // 清空屏幕
        cout << "-----------------管理后台-----------------" << endl;
        cout << "*********  1. 显示客房信息   *********" << endl;
        cout << "*********  2. 设置客房状态   *********" << endl;
        cout << "*********  3. 删除客房信息   *********" << endl;
        cout << "*********  4. 添加客房信息   *********" << endl;
        cout << "*********  5. 退出管理后台   *********" << endl;
        cout << "请输入您的操作：";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略无效输入
            std::cerr << "输入无效，请重新输入。\n";
            continue;
        }

        switch (choice) {
            case 1:
                clear_screen(); // 清空屏幕
                display_rooms(true); // 显示客房信息，并显示“按回车键返回”的提示
                break;
            case 2:
                set_room_state_for_backend();
                break;
            case 3:
                remove_data();
                break;
            case 4:
                add_room(true); // 管理员添加客房信息
                break;
            case 5:
                in_backend = false; // 退出管理后台
                std::cout << "已退出管理后台。\n";
                break;
            default:
                std::cerr << "无效的操作，请重新输入。\n";
                break;
        }
    }
}
void RoomView::show_available_rooms() {
    cout << "-----------------可用客房信息-----------------" << endl;
    cout << "编号" << setw(20) << "名称" << setw(10) << "面积" << setw(10) << "价格" << setw(15) << "床位数量" << setw(10) << "状态" << endl;
    cout << "---------------------------------------------" << endl;

    // 使用 m_grm 获取房间信息
    const auto& rooms = m_grm.get_rooms();
    if (rooms.empty()) {
        cout << "暂无可用客房信息！" << endl;
    } else {
        for (const auto& room : rooms) {
            ROOM_STATE state = room.second->get_state();
            if (state == ROOM_STATE::FREE || state == ROOM_STATE::BOOKED) {
                // 将数值和单位拼接成一个字符串
                string area = to_string(room.second->get_area()) + "平方";
                string price = to_string(room.second->get_price()) + "元";
                string bed_num = to_string(room.second->get_bed_number()) + "个";

                cout << setw(20) << left << room.second->get_number() << " "
                     << setw(10) << left << room.second->get_name() << " "
                     << setw(10) << left << area << " "
                     << setw(10) << left << price << " "
                     << setw(15) << left << bed_num << " "
                     << setw(10) << left << room.second->show_state() << endl;
            }
        }
    }
    cout << "---------------------------------------------" << endl;
}
bool RoomView::authenticate_admin() {
    std::cout << "请输入管理员密码：";
    std::string password = get_password(); // 使用 get_password 获取密码

    if (password == ADMIN_PASSWORD) {
        std::cout << "密码正确，欢迎进入管理后台！\n";
        std::cout << "3秒后自动进入后台...\n";
        std::this_thread::sleep_for(std::chrono::seconds(3)); // 暂停3秒
        return true;
    } else {
        std::cout << "密码错误，拒绝访问！\n";
        // 等待用户按回车键
        std::cout << "按回车键返回...";
        std::cin.ignore(); // 忽略之前的输入
        std::cin.get();    // 等待用户按回车键
        return false;
    }
}
void RoomView::set_room() {
    std::string input;
    std::cout << "请输入要设置的客房编号或名称（只含字母数字或其组合，或输入 'back' 返回）：";
    std::cin >> input;

    // 检查是否输入了 'back'
    if (input == "back") {
        return; // 返回到上一级菜单
    }

    // 验证输入是否只包含字母和数字
    if (!std::regex_match(input, std::regex("^[a-zA-Z0-9]+$"))) {
        std::cerr << "输入无效，请输入只含字母数字或其组合的编号或名称。\n";
        return;
    }

    // 尝试根据输入查找客房
    GuestRoom* room = m_grm.find_room_by_identifier(input);
    if (!room) {
        std::cerr << "未找到编号或名称为 " << input << " 的客房。\n";
        return;
    }

    set_room_state(*room);  // 调用 set_room_state 函数
}
void RoomView::set_room_state(GuestRoom& room, bool is_admin) {
    // 如果是前台用户，且房间状态为“维修”，则禁止修改
    if (!is_admin && room.get_state() == ROOM_STATE::MAINTENANCE) {
        std::cout << "前台用户无权限修改维修中的房间状态！\n";
        std::cout << "按回车键返回...";
        std::cin.ignore(); // 忽略之前的输入
        std::cin.get();    // 等待用户按回车键
        return;
    }

    int state;
    std::cout << "请选择新的状态：\n"
              << "1. 空闲\n"
              << "2. 入住\n"
              << "3. 已预订\n"
              << "4. 维修\n"
              << "请输入状态编号（或输入 'back' 返回）：";

    // 读取新的状态
    if (!(std::cin >> state)) {
        std::cin.clear(); // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略缓冲区中的剩余内容
        std::cerr << "输入无效，请重新选择。\n";
        return; // 返回主菜单
    }

    // 检查是否输入了 'back'
    if (state == 0) { // 假设用户输入 'back' 时，state 会被解析为 0
        return; // 返回到上一级菜单
    }

    // 验证状态编号是否有效
    if (state < 1 || state > 4) {
        std::cerr << "无效的状态编号，请重新选择。\n";
        return; // 返回主菜单
    }

    // 更新客房状态
    ROOM_STATE new_state = static_cast<ROOM_STATE>(state);
    room.set_state(new_state);

    // 保存更改
    if (m_grm.save_data()) {  // 调用 GuestRoomManager::save_data()
        std::cout << "客房状态已成功更新。\n";
    } else {
        std::cerr << "保存客房状态失败。\n";
    }

    // 等待用户按回车键
    std::cout << "按回车键返回...";
    std::cin.ignore(); // 忽略之前的输入
    std::cin.get();    // 等待用户按回车键
}
void ensureFileExists(const std::string& filename) {
    // 检查文件是否存在
    std::ifstream file_check(filename);
    if (!file_check.good()) {
        // 如果文件不存在，则创建文件
        std::ofstream file_create(filename);

        // 可选：可以在这里写入文件头或其他初始化内容
        // 例如，CSV 文件的表头
        // file_create << "编号,名称,价格,床位数量,面积,状态\n";

        // 关闭文件（自动关闭，因为 file_create 离开作用域时会调用析构函数）
        if (file_create) {
            std::cout << "文件 " << filename << " 已创建。\n";
        } else {
            std::cerr << "无法创建文件 " << filename << "。\n";
        }
    } else {
        std::cout << "文件 " << filename << " 已存在。\n";
    }
}
void RoomView::show_menu()
{
    cout << "-----------------酒店管理系统-----------------" << endl;
    cout << "*********  1. 显示客房信息   *********" << endl;
    cout << "*********  2. 设置客房状态   *********" << endl;
    cout << "*********  3. 删除客房信息   *********" << endl;
    cout << "*********  4. 添加客房信息   *********" << endl;
    cout << "*********  5. 退出管理系统   *********" << endl;
}
void RoomView::add_room(bool is_admin) {
    clear_screen(); // 清空屏幕
    show_all_rooms(); // 显示所有客房信息

    std::string name;
    int price;
    int bed_num;
    int area;
    int state;
    int floor;

    std::cout << "请输入客房名称（或输入 'back' 返回）：";
    std::cin >> name;

    // 检查是否输入了 'back'
    if (name == "back") {
        return; // 返回到上一级菜单
    }

    // 验证价格输入
    std::cout << "请输入客房价格（或输入 'back' 返回）：";
    while (!(std::cin >> price) || price <= 0) {
        std::cin.clear(); // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略无效输入
        std::cout << "输入无效，请输入正整数（或输入 'back' 返回）：";
        std::string input;
        std::cin >> input;
        if (input == "back") {
            return; // 返回到上一级菜单
        }
    }

    // 验证床数输入
    std::cout << "请输入客房床数（或输入 'back' 返回）：";
    while (!(std::cin >> bed_num) || bed_num <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效，请输入正整数（或输入 'back' 返回）：";
        std::string input;
        std::cin >> input;
        if (input == "back") {
            return; // 返回到上一级菜单
        }
    }

    // 验证面积输入
    std::cout << "请输入客房面积（或输入 'back' 返回）：";
    while (!(std::cin >> area) || area <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效，请输入正整数（或输入 'back' 返回）：";
        std::string input;
        std::cin >> input;
        if (input == "back") {
            return; // 返回到上一级菜单
        }
    }

    // 输入楼层
    std::cout << "请输入客房所在楼层（或输入 'back' 返回）：";
    while (!(std::cin >> floor) || floor <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效，请输入正整数（或输入 'back' 返回）：";
        std::string input;
        std::cin >> input;
        if (input == "back") {
            return; // 返回到上一级菜单
        }
    }

    // 如果是管理员，允许设置状态
    if (is_admin) {
        std::cout << "请选择客房状态：\n"
                  << "1. 空闲\n"
                  << "2. 已预订\n"
                  << "3. 入住\n"
                  << "4. 维修\n"
                  << "请输入状态编号（或输入 'back' 返回）：";
        while (!(std::cin >> state) || state < 1 || state > 4) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入 1 到 4 之间的数字（或输入 'back' 返回）：";
            std::string input;
            std::cin >> input;
            if (input == "back") {
                return; // 返回到上一级菜单
            }
        }
    } else {
        // 前台用户，默认状态为“空闲”
        state = 1;
    }

    // 创建客房对象并使用智能指针管理
    ROOM_STATE room_state = static_cast<ROOM_STATE>(state);
    auto room = std::make_shared<GuestRoom>(name, price, bed_num, area, room_state);
    room->set_number(room->generate_number(floor)); // 生成新的编号

    // 添加客房
    m_grm.add_room(room); // 通过 m_grm 添加客房
    std::cout << "客房已成功添加。\n";

    cout << "按回车键返回菜单...";
    cin.ignore(); // 忽略之前的输入
    cin.get();    // 等待用户按回车键
}
void RoomView::remove_data() {
    clear_screen(); // 清空屏幕
    show_all_rooms(); // 显示所有客房信息

    std::string identifier;
    std::cout << "请输入要删除的客房编号或名称（或输入 'back' 返回）：";
    std::cin >> identifier;

    // 检查是否输入了 'back'
    if (identifier == "back") {
        return; // 返回到上一级菜单
    }

    if (m_grm.remove_data(identifier)) {
        std::cout << "客房已成功删除。\n";
    } else {
        std::cout << "删除失败，请检查输入的编号或名称。\n";
    }

    cout << "按回车键返回菜单...";
    cin.ignore(); // 忽略之前的输入
    cin.get();    // 等待用户按回车键
}
void RoomView::show_all() {
    cout << "-----------------所有客房信息-----------------" << endl;
    cout << "编号" << setw(15) << "名称" << setw(10) << "面积" << setw(10) << "价格" << setw(15) << "床位数量" << setw(10) << "状态" << setw(20) << "登记时间" << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;

    // 使用 m_grm 获取房间信息
    const auto& rooms = m_grm.get_rooms();
    if (rooms.empty()) {
        cout << "请添加客房信息后再进行操作!" << endl;
    } else {
        for (const auto& room : rooms) {
            // 将数值和单位拼接成一个字符串
            string area = to_string(room.second->get_area()) + "平方";
            string price = to_string(room.second->get_price()) + "元";
            string bed_num = to_string(room.second->get_bed_number()) + "个";

            // 输出房间信息
            cout << setw(15) << left << room.second->get_number() << " "
                 << setw(7) << left << room.second->get_name() << " "
                 << setw(9) << left << area << " "
                 << setw(13) << left << price << " "
                 << setw(11) << left << bed_num << " "
                 << setw(10) << left << room.second->show_state() << " "
                 << setw(20) << left << room.second->get_registration_time() << endl;
        }
    }
    cout << "-----------------------------------------------------------------------------------------" << endl;
}
//启动酒店管理系统
void RoomView::run() {

    bool state = true;
    while (state) {
        clear_screen(); // 清空屏幕
        cout << "-----------------酒店管理系统-----------------" << endl;
        cout << "*********  1. 前台专用   *********" << endl;
        cout << "*********  2. 管理后台   *********" << endl;
        cout << "*********  3. 退出系统   *********" << endl;
        cout << "请输入您的操作：";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略无效输入
            std::cerr << "输入无效，请重新输入。\n";
            continue;
        }

        switch (choice) {
            case 1: // 前台专用
                clear_screen(); // 进入前台菜单前清屏
                frontend_menu();
                break;
            case 2: // 管理后台
                if (authenticate_admin()) {
                    clear_screen(); // 进入管理后台前清屏
                    manage_backend(); // 进入管理后台
                }
                break;
            case 3: // 退出系统
                state = false;
                std::cout << "退出系统。\n";
                break;
            default:
                std::cerr << "无效的操作，请重新输入。\n";
                break;
        }
    }
}