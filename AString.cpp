#include <cstring>
#include "AString.h"

using namespace std;

AString::AString(const char* str) {
    if (str != nullptr) {
        int length = strlen(str);
        m_data = new char[length + 1];
        strcpy(m_data, str);
    } else {
        m_data = new char[1];
        *m_data = '\0';
    }
}

AString::AString(const AString& other) {
    if (other.m_data != nullptr) {
        m_data = new char[strlen(other.m_data) + 1];
        strcpy(m_data, other.m_data);
    } else {
        m_data = nullptr;
    }
}

AString::~AString() {
    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }
}

AString& AString::operator = (const AString& other) {
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

AString& AString::operator + (const AString& other) {
    AString newStr;
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

bool AString::operator == (const AString& other) {
    if (strlen(m_data) != strlen(other.m_data)) {
        return false;
    } else {
        return strcmp(m_data, other.m_data) == 0 ? true : false;
    }
}

int AString::getLength() {
    return strlen(m_data);
}

char* AString::getChar() {
    return m_data;
}