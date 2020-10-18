#include <cstring>
#include "myString.h"

using namespace std;

// 普通构造函数
myString::myString(const char* str) {
    if (str != nullptr) {
        int length = strlen(str);
        m_data = new char[length + 1];
        strcpy(m_data, str);
    } else {
        m_data = new char[1];
        *m_data = '\0';
    }
}

// 拷贝构造函数
myString::myString(const myString& other) {
    if (other.m_data != nullptr) {
        m_data = new char[strlen(other.m_data) + 1];
        strcpy(m_data, other.m_data);
    } else {
        m_data = nullptr;
    }
}

// 析构函数
myString::~myString() {
    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }
}

// 赋值函数
myString& myString::operator = (const myString& other) {
    if (this != &other) {
        delete[] m_data;
        if (other.m_data != nullptr) {
            m_data = new char[strlen(other.m_data) + 1];
            strcpy(m_data, other.m_data);
        } else {
            m_data = nullptr;
        }
    }
    return *this;
}

// 字符串连接
myString& myString::operator + (const myString& other) {
    myString newStr;
    if (other.m_data == nullptr) {
        newStr = *this;
    } else if (m_data == nullptr){
        newStr = other;
    }
    else {
        newStr.m_data = new char[strlen(m_data) + strlen(other.m_data) + 1];
        strcpy(newStr.m_data, other.m_data);
        strcat(newStr.m_data, m_data);
    }
    return newStr;
}

// 判断相等
bool myString::operator == (const myString& other) {
    if (strlen(m_data) != strlen(other.m_data)) {
        return false;
    } else {
        return strcmp(m_data, other.m_data) == 0 ? true : false;
    }
}

// 返回长度
int myString::getLength() {
    return strlen(m_data);
}

string myString::getString() {
    return string(m_data);
}