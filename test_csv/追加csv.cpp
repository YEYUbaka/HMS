#include <iostream>
#include <fstream>
#include <string>
using namespace std;
 
 
int main()
{
	string fname = "test.csv";
 
	//先判断文件是否存在
	ifstream file(fname);
	if (!file.is_open()) {
		cout << "File does not exist!" << endl;
		return 1;
	}
	else {
		cout << "File exists!" << endl;
		file.close();//必须先关闭文件后才可写入
 
		ofstream outFile(fname, ios::app);
 
		// ********写入两行数据*********
		outFile << "wangwu" << ','
			<< "1234" << ','
			<< to_string(12.32) << ','
			<< "河南省" << endl;
 
		outFile << "lisi" << ','
			<< to_string(2032.1) << ','
			<< to_string(789.2) << ','
			<< "北京市" << endl;
 
		//数字需转为字符串进行写入,csv文件结束一行写入需要"\n"或者endl进行换行
 
		outFile.close();
	}
 
	return 0;
}
 
 