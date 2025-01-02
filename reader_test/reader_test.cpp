#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

using namespace std;

int main() {
    const char* const room_data_file = "rooms.txt";
    map<string, string> test_map;
    test_map["101"] = "Test Room";

    // Save data
    ofstream ofs(room_data_file);
    for (const auto& pair : test_map) {
        ofs << pair.first << "," << pair.second << "\n";
    }
    ofs.close();

    // Read data
    ifstream ifs(room_data_file);
    map<string, string> read_map;
    string line, key, value;
    while (getline(ifs, line)) {
        stringstream ss(line);
        if (getline(ss, key, ',') && getline(ss, value)) {
            read_map[key] = value;
            cout << "Read: " << key << " -> " << value << endl;
        }
    }
    ifs.close();

    return 0;
}