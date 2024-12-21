#include "room_manager.h"
#include "guest_room.h"
#include <iostream>
using namespace std;
//构造函数实现
GuestRoomManager::GuestRoomManager()
{
    GuestRoom().read_data();
}
//查找客房
bool GuestRoomManager::check_in(const string number)
{
    //读取文件中数据到容器
    GuestRoom grm;
    m_room_list = grm.read_data();
    //在容器中查找客房
    if (m_room_list.find(number) != m_room_list.end())
    {
        return false;
    }
    return true;
}
//添加客房
void GuestRoomManager::add_room( GuestRoom room)
{
    //判断客房编号是否存在
    if (check_in(room.get_num()))
    {
        cout<<"房间编号已存在"<<endl;
        return;
    }
    //将客房添加到容器中
    m_room_list.insert(make_pair(room.get_num(),room));
    //将容器中的数据保存到文件中
    GuestRoom().save_data(m_room_list);
}

//删除客房数据
bool GuestRoomManager::remove_data(const string number)
{
    //判断客房编号是否存在
    if (!check_in(number))
    {
        cout<<"房间编号不存在"<<endl;
        return false;
    }
    //删除
    m_room_list = GuestRoom().read_data();
    m_room_list.erase(number);
    //更新文件
    GuestRoom().save_data(m_room_list);
    return true;
}

//设置客房状态
void GuestRoomManager::set_room_state(string number)
{
    //判断客房编号是否存在
    if (!check_in(number))
    {
        cout<<"房间编号不存在"<<endl;
        return;
    }
    else
    {
        //将文件中客房数据读取到文件中
        m_room_list = GuestRoom().read_data();
        //设置为入住状态
        m_room_list[number].set_state();
        //将数据保存到文件中
        GuestRoom().save_data(m_room_list);
    }
}