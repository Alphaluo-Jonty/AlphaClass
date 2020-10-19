#include <algorithm>
#include <iostream>

template<typename T>
class myVector 
{
private:
    /* myVector each time increase space length */
    #define WALK_LENGTH 64;

public:
    // default constructor
    myVector(): array(nullptr), theSize(0), theCapacity(0) {}
    myVector(const T& t, unsigned int n): array(nullptr), theSize(0), theCapacity(0) {
        while (n--) {
            push_back(t);
        }
    }

    // copy constructor
    myVector(const myVector<T>& other): array(nullptr), theSize(0), theCapacity(0) {
        *this = other;
    }

    // destructor
    ~myVector() {
        clear();
    }

    // operator =
    myVector<T>& operator = (myVector<T>& other) {
        if (*this == other) {
            return *this;
        }
        clear();
        theSize = other.size();
        theCapacity = other.capacity();
        array = new T[theCapacity];
        for (unsigned int i = 0; i < theSize; ++i) {
            array[i] = other[i];
        }
        return *this;
    }

    // operator []
    T& operator[] (unsigned int pos) {
        assert(pos < theSize);
        return array[pos];
    }

    // element theSize
    unsigned int size() {
        return theSize;
    }

    // element theCapacity
    unsigned int capacity() {
        return theCapacity;
    }

    // is empty
    bool empty() {
        return theSize == 0;
    }

    // clear myVector
    void clear() {
        deallocator(array);
        theSize = 0;
        theCapacity = 0;
        array = nullptr;
    }

    // erase an element in the pos
    void erase(unsigned int pos){
        if (pos < theSize) {
            --theSize;
            for (unsigned int i = pos; i < theSize; ++i) {
                array[i] = array[i + 1];
            }
        }
    }

    T& pop_back() {
        return array[--theSize];
    }

    void push_back(T& t) {
        if (theCapacity == theSize) {
            T* oldArray = array;
            theCapacity += WALK_LENGTH;
            array = allocator(theCapacity);
            for (unsigned int i = 0; i < theSize; ++i) {
                array[i] = oldArray[i];
            }
            deallocator(oldArray);
        }
        array[theSize++] = t;
    }

private:
    // deallocator
    void deallocator(T* t) {
        if (t) {
            delete[] t;
        }
    }

    // allocator
    T* allocator(unsigned int n) {
        return new T[n];
    }
private:
    T* array;
    unsigned int theSize;
    unsigned int theCapacity;
};