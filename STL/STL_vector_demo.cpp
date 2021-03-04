#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void print(int n) {
    cout << n << endl;
}

// 结构体外定义比较函数
struct Rect{
    int id;
    int length;
    int width;
};

int cmpRect(Rect a, Rect b) {
    if (a.id != b.id) {
        return a.id < b.id;
    } else {
        if (a.length != b.length) {
            return a.length < b.length;
        } else {
            return a.width < b.width;
        }
    }
}

// 结构体内定义比较函数——即重载 < 操作符
struct Result{
    int id;
    int length;
    int width;
    bool operator < (const Result &other) const {
        if (id != other.id) {
            return id < other.id;
        } else {
            if (length != other.length) {
                return length < other.length;
            } else {
                return width < other.width;
            }
        }
    }
};

int main() {
    // 初始化方法
    vector<int> v1;     // 空的vector
    vector<int> v2(10, 0);  // 容器包含10个0
    vector<int> v3(v2);  // 拷贝方式初始化
    vector<int> v4(5);   // 5个值初始化的对象，默认为5个0
    vector<int> v5{1, 2, 3, 4, 5};  // 注意是 {}
    vector<int> v6 = {1, 2, 3, 4, 5}; // 等价于上一方法
    int a[7] = {1, 2, 3, 4, 5, 6, 7};
    vector<int> v7(a, a+7); // 与set的初始化方式之一相同

    // for_each 遍历，注意包含头文件<algorithm>
    for_each(v7.begin(), v7.end(), print);

    // 排序，对自定义的类型添加比较函数
    // 使用sort+自定义比较函数，对vector进行排序
    vector<Rect> vec;
    Rect rect;
    rect.id = 2, rect.length = 3, rect.width = 4;
    vec.push_back(rect);
    rect.id = 1, rect.length = 2, rect.width = 3;
    vec.push_back(rect);
    vector<Rect>::iterator it = vec.begin();
    // 未排序前
    cout << (*it).id << " " << (*it).length << " " << (*it).width << endl;
    // 自定义的类型，如果没有重载 < 运算符，sort时需要指定自定义比较函数
    sort(vec.begin(), vec.end(), cmpRect);
    // 排序后
    cout << (*it).id << " " << (*it).length << " " << (*it).width << endl;

    // 使用sort，对自定义类型的容器进行排序
    vector<Result> vecRes;
    Result res;
    res.id = 2, res.length = 3, res.width = 4;
    vecRes.push_back(res);
    res.id = 1, res.length = 2, res.width = 3;
    vecRes.push_back(res);
    vector<Result>::iterator itor = vecRes.begin();
    // 未排序前
    cout << (*itor).id << " " << (*itor).length << " " << (*itor).width << endl;
    // 使用sort，直接对vector排序(因为已经重载了 < 操作符)
    sort(vecRes.begin(), vecRes.end());
    cout << (*itor).id << " " << (*itor).length << " " << (*itor).width << endl;
    
    // 查找，使用<algorithm>中的find函数，返回迭代器
    if ( find(v6.begin(), v6.end(), 3) != v6.end()) {
        cout << "Found" << endl;
    } else {
        cout << "Do not found" << endl;
    }

    // 删除
    // 以正确删除v8中的3为例
    vector<int> v8 = {1, 2, 3, 3, 4, 6, 6, 8};
    vector<int>::iterator iteor = v8.begin();
    for (; iteor != v8.end();) {
        if (*iteor == 3) {
            iteor = v8.erase(iteor);
        } else {
            ++iteor;
        }
    }

    // 插入
    v8.insert(v8.end(), 7);  // 在最后插入7
    v8.insert(v8.begin(), 0); // 在最前插入0
    v8.insert(v8.begin()+1, 10); // 在第2个元素前插入10
    v8.insert(v8.begin()+2, 3, 9); // 在第3个元素前插入3个9
    return 0;
}