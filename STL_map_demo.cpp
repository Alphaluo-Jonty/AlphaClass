#include <iostream>
#include <cstring>
#include <map>
#include <iterator>
using namespace std;

struct CmpByKeyLength {
    bool operator() (const string& k1, const string& k2) {
        return k1.length() < k2.length();
    }
};

int main() {
    // 创建空的map对象
    map<int, string> mapStu;
    //////// 插入数据
    // 1.用insert插入pair数据
    mapStu.insert(pair<int, string>(1, "Jack"));
    mapStu.insert(pair<int, string>(2, "Bob"));
    mapStu.insert(pair<int, string>(3, "Jim"));
    // 2. 用insert插入vaule_type数据
    mapStu.insert(map<int, string>::value_type (4, "Kimmy"));
    // 3. 用数组的方式插入数据
    mapStu[3] = "Lucia";
    mapStu[5] = "Luna";

    //////// 遍历map的方式
    // 1. 迭代器遍历
    map<int, string>::iterator iter;
    for (iter = mapStu.begin(); iter != mapStu.end(); iter++) {
        // 迭代器遍历
        cout << iter->first << " " << iter->second << endl;
    }
    // 2. 数组方式遍历
    for (int i = 1; i <= mapStu.size(); ++i) {
        cout << i << " " << mapStu[i] << endl;
    }

    //////// 查找
    // 1. count用来计数，但无法定位
    // 2. find用来寻找，根据键，返回迭代器。
    iter = mapStu.find(3);
    if (iter != mapStu.end()) {
        cout << "Found, the value is " << iter->second << endl;
    } else {
        cout << "Do not found" << endl;
    }

    //////// 删除
    mapStu.erase(2);    // 删除map中key为2的键值对
    mapStu.erase(iter); // 删除迭代器itor所指向的元素,itor必须非end
    // 正确的遍历删除方式
    iter = mapStu.begin();
    for (; iter != mapStu.end();) {
        if ((*iter).second == "Luna") {
            mapStu.erase(iter++);
        } else {
            ++iter;
        }
    }

    //////// 排序
    // 1. 使用默认的less，或者greater
    map<string, int, greater<string>> mapScore;
    mapScore.insert(pair<string, int>("Lucia", 98));
    mapScore.insert(pair<string, int>("Historial", 100));
    mapScore["Lee"] = 90;
    // 2. 使用自定义的比较函数，例如根据字符串长度
    map<string, int, CmpByKeyLength> mapScore2;
    mapScore2.insert(pair<string, int>("Lucia", 98));
    mapScore2.insert(pair<string, int>("Historial", 100));
    mapScore2["Lee"] = 90;
    return 0;
};
