#include <iostream>
#include <fstream>
#include <string>
using namespace std;
 
 
int main()
{
	string fname = "test.csv";
 
	ofstream outFile(fname, ios::out);
 
	if (outFile.is_open())  // 检查文件是否打开成功
	{
		// 写入标题行
		outFile << "name" << ','
			<< "income" << ','
			<< "expenditure" << ','
			<< "addr" << endl;
 
		// ********写入两行数据*********
 
		outFile << "zhangsan" << ','
			<< "3000" << ','
			<< "1200" << ','
			<< "陕西省" << endl;
 
		outFile << "lisi" << ','
			<< to_string(2032.1) << ','
			<< to_string(789.2) << ','
			<< "北京市" << endl;
 
		//数字需转为字符串进行写入,csv文件结束一行写入需要"\n"或者endl进行换行
 
		outFile.close();
	}
 
	else
	{
		cout << "文件无法打开！" << endl;
	}
}
 