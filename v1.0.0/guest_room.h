#pragma once
#include <string>
#include <map>
using namespace std;
//定义enum ROOM_STATE枚举类型，表示客房状态：空闲、入住
enum ROOM_STATE
{
    FREE = 0,
    CHECK_IN
};
//客房类
class GuestRoom
{
public:
    GuestRoom()=default;
    GuestRoom(string,int,int,int,ROOM_STATE);
    string generate_number();
    string show_state();
    bool save_data(map<string,GuestRoom>&);
    map<string,GuestRoom> read_data();
public:
    string get_num();
    string get_name();
    int get_price();
    int get_area();
    int get_bed_num();
    void set_state();
private:
    string m_number;
    string m_name;
    int m_price;
    int m_area;
    int m_bed_number;
    enum ROOM_STATE m_state;
};
