#pragma once
#include "guest_room.h"
#include "room_manager.h"
#include <map>
#include <string>
class GuestRoomManager
{
public:
    GuestRoomManager();
    bool check_in(const string);
    void add_room(const GuestRoom room);
    bool remove_data(const string);
    void set_room_state(string number);
private:
    map<string, GuestRoom> m_room_list;
};
