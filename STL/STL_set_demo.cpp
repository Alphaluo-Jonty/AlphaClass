#include <iostream>
#include <set>
#include <cstring>
#include <vector>

using namespace std;

struct strLess {
    bool operator() (const char *s1, const char *s2) const {
        return strcmp(s1, s2) < 0;
    }
};

int main() {
    // 创建空的set对象
    set<int> s1;
    // 创建空的set对象，以及比较函数对象为自定义的strLess
    set<const char*, strLess> s2(strLess);
    // 利用set对象s1，拷贝生成set对象s3
    set<int> s3(s1);
    // 利用迭代区间所指的元素，创建一个set对象
    int iArray[] = {12, 32, 56};
    set<int> s4(iArray, iArray+3);
    const char* szArray[] = {"hello", "dog", "bird"};
    set<const char*, strLess> s5(szArray, szArray+3, strLess());
    //  用迭代器，指定区间
    vector<int> vec;
    for (int i = 0; i < 6; ++i) {
        vec.push_back(i);
    }
    set<int> s6(vec.begin(), vec.end());
    
    // 元素插入,只能用insert
    s6.insert(6);
    // 查找和读取元素 -- find
    set<int>::iterator it;
    it = s6.find(5);
    if (it != s6.end()) {  // 判断条件也是看是否为end()
        cout << "Found" << endl;
    } else {
        cout << "Do not found" << endl;
    }
    // 不能插入相同元素
    pair<set<int>::iterator, bool> p = s6.insert(6);
    if (p.second) {
        cout << "Insert OK" << endl;
    } else {
        cout << "Insert Failed" << endl;
    }
    // 遍历,同样使用迭代器
    for (it = s6.begin(); it != s6.end(); ++it) {
        cout << *it << endl;
    }

    // 删除
    s6.erase(6); // 直接删除key(也是value)为6的元素
    s6.erase(s6.begin()); // 删除迭代器指向元素
    it = s6.begin()++;
    s6.erase(it, ++it); // 删除迭代器区间的元素
    s6.clear();  // 删除全部元素

    return 0;
}