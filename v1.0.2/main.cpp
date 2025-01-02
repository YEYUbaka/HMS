#include "guest_room.h"
#include <iostream>
#include <time.h>
#include <string>
#include "room_manager.h"
#include "room_view.h"
#include "guest_room.h"
#include <filesystem>
using namespace std;
int main()
{
    RoomView grv;
    grv.run();
//    std::cout << "Current path is: " << std::filesystem::current_path() << std::endl;
    return 0;
}


