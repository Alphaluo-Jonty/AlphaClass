class AString {
public:
    // default constructure function
    AString(const char *str = nullptr);
    // copy constructure function
    AString(const AString& str);
    // deconstruct function
    ~AString();
    // override operator +
    AString& operator + (const AString& other);
    // override operator =
    AString& operator = (const AString& other);
    // override operator ==
    bool operator == (const AString& other);
    // get length of AString
    int getLength();
    // get char
    char* getChar();

private:
    char* m_data;
};