#pragma once
#include "room_view.h"
#include "room_manager.h"
class RoomView
{
public:
    void show_menu();
    void add_room();
    void remove_data();
    void set_room();
    void show_all();
    void run();
private:
    GuestRoomManager m_grm;
};
