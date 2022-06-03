/*
*   Used to read a line from the serial port ending with '\n'.
*   Daniel Robinson 2022
*/

class SerialPort{
public:
    //open/close and configure
    SerialPort(const char *deviceName);
    ~SerialPort();

    //return a string of characters ended by a newline
    const char *readString();
    int readInt();

private:
    //called by constructors and destructors
    int open(const char *deviceName);
    int configure();
    int close();

    //file descriptor
    int m_serial;
};