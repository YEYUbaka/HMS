#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    // 打开文件进行写入
    ofstream file("rooms.txt");
    if (!file.is_open()) {
        cerr << "无法打开文件 rooms.txt" << endl;
        return 1;
    }

    // 写入测试数据
    file << "101,标准单人间,20,300,1,0\n";
    file << "102,豪华双人间,30,500,2,1\n";
    file << "103,商务套房,40,800,2,1\n";
    file << "104,家庭套房,50,1000,3,0\n";
    file << "105,总统套房,60,2000,2,1\n";
    file << "106,标准双人间,25,400,2,0\n";
    file << "107,豪华单人间,28,450,1,1\n";
    file << "108,行政套房,35,700,2,0\n";
    file << "109,海景房,32,600,2,1\n";
    file << "110,花园房,27,550,2,0\n";

    // 关闭文件
    file.close();

    cout << "rooms.txt 文件已成功生成！" << endl;

    return 0;
}