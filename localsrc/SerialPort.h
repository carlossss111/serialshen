#include <list>

#define STORE_STRINGS

class SerialPort{
public:
    //open/close and configure
    SerialPort(const char *deviceName);
    ~SerialPort();

    //return a string of characters
    //if STORE_STRINGS is defined then returned strings are freed at the destructor
    const char *readString() const;
    //returns a 32 bit int
    uint32_t readInt32() const;

private:
    //called by constructors and destructors
    int open(const char *deviceName);
    int configure();
    int close();

    //file descriptor
    int m_serial;

    //string list
    mutable std::list<char *> m_strList;
};

struct TenBitfield{
    //split a 32bit integer into 10 bits each, 0b00[a1*ten][a2*ten][a3*ten]
    TenBitfield operator= (const uint32_t bitsIn){
        a1 = bitsIn >> 20; //LEFTMOST - first ten bits after "0b00"
        a2 = bitsIn >> 10; //MIDDLE - next ten bits
        a3 = bitsIn; //RIGHT - last ten bits
        return *this;
    }   

    unsigned int a1 : 10; //0-1023
    unsigned int a2 : 10; //0-1023
    unsigned int a3 : 10; //0-1023
};