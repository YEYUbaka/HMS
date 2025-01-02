#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;
//定义enum ROOM_STATE枚举类型，表示客房状态：空闲、入住
enum class ROOM_STATE
{
    FREE = 0,
    CHECK_IN =1,
};
//客房类
class GuestRoom
{
public:
    GuestRoom()=default;
    GuestRoom(string,int,int,int,ROOM_STATE);
    string generate_number();
    string show_state();
    static bool save_data(const map<string, GuestRoom>& room_list);
    map<string,GuestRoom> read_data();
public:
    const std::string& get_number() const;
    const std::string& get_name() const;
    int get_price() const;
    int get_area() const;
    int get_bed_number() const;
    void set_state();
    void set_number(const std::string& number);
    ROOM_STATE get_state() const;
private:
    string m_number;
    string m_name;
    int m_price;
    int m_bed_number;
	int m_area;
    enum ROOM_STATE m_state;
};
