/*
*   Used to read a line from the serial port ending with '\n'.
*   Daniel Robinson 2022
*/
#include <list>

//for frees in the destructor
#define STORE_STRINGS

class SerialPort{
public:
    //open/close and configure
    SerialPort(const char *deviceName);
    ~SerialPort();

    //return a string of characters
    //if STORE_STRINGS is defined then returned strings are freed at the destructor
    const char *readString() const;
    //returns an int
    int readInt() const;

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