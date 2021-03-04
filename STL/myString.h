class myString {
public:
    // default constructure function
    myString(const char *str = nullptr);
    // copy constructure function
    myString(const myString& str);
    // deconstruct function
    ~myString();
    // override operator +
    myString& operator + (const myString& other);
    // override operator =
    myString& operator = (const myString& other);
    // override operator ==
    bool operator == (const myString& other);
    // get length of myString
    int getLength();
    // get char
    char* getChar();

private:
    char *m_data;
};