#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "SerialPort.h"

SerialPort::SerialPort(const char *deviceName){
    //call helper functions
    if(open(deviceName) != 0)
        exit(1);
    if(configure() != 0)
        exit(2);
}

SerialPort::~SerialPort(){
    //close the file descriptor
    close();

    //clean up the list of strings read
    #ifdef STORE_STRINGS
    for(char * elem : m_strList)
        free((void *) elem);
    #endif
}

const char *SerialPort::readString() const{
    char buffer[1024];
    char *out;

    //read into buffer
    int numOfBytes = read(m_serial, &buffer, sizeof(buffer));
    if(!numOfBytes){
        fprintf(stderr,"Error from read(): Timed out.\n");
        return "";
    }

    //copy into smaller specifically sized buff
    if(!(out = (char *) malloc(numOfBytes))){
        fprintf(stderr,"Error from malloc()\n");
        exit(1);
    }
    memcpy(out, buffer, numOfBytes);
    out[numOfBytes - 1] = '\0'; //replace \n with \0

    //store strings for destructor garbage collection
    #ifdef STORE_STRINGS
    m_strList.push_back(out);
    #endif

    return out;
}

int SerialPort::readInt() const{
    //read as a string
    char buffer[sizeof(int) + 1];
    int numOfBytes = read(m_serial, &buffer, sizeof(buffer));
    if(!numOfBytes){
        fprintf(stderr,"Error from read(): Timed out.\n");
        return -1;
    }

    //convert into an int
    return atoi(buffer);
}

int SerialPort::open(const char *deviceName){
    //open the serial port using a file descriptor
    m_serial = ::open(deviceName, O_RDWR);
	if(m_serial < 0){
		fprintf(stderr,"Error %i from open(): %s\n", errno, strerror(errno));
		return m_serial;
	}
    return 0;
}

int SerialPort::configure(){
    //read in i/o flags (see "termios" struct)
	struct termios tty;
	if(tcgetattr(m_serial, &tty)) {
		fprintf(stderr,"Error %i from tcgetattr(): %s\n", errno, strerror(errno));
		return 1;
	}	

	//communication configurations
	tty.c_cflag &= ~PARENB; //disable parity bit
	tty.c_cflag &= ~CSTOPB; //use only one stop bit
	tty.c_cflag &= ~CSIZE; //clear size bits (for statement below)
	tty.c_cflag |= CS8; //8 bits per byte
	tty.c_cflag &= ~CRTSCTS; //disable control flow
	tty.c_cflag |= CREAD | CLOCAL; //enable reading and disable control lines

	//local configurations
	tty.c_lflag |= ICANON; //enable canonical mode (input processed when receiving \n)
	tty.c_lflag &= ~ECHO; //disable echo back to serial port
	tty.c_lflag &= ~ECHOE; //disable erasure
	tty.c_lflag &= ~ECHONL; //disable new-line echo
	tty.c_lflag &= ~ISIG; //disable reading of INTR, QUIT, SUSP characters

	//input configurations
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); //disable software flow control
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP); //disable special handling

	//output configurations
	tty.c_oflag &= ~OPOST; //disable special interpretation of \n
	tty.c_oflag &= ~ONLCR; //disable conversion of \n to \r	

	//timing
	tty.c_cc[VTIME] = 50; //wait for up to n deciseconds
	tty.c_cc[VMIN] = 0;

	//set baud rate
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	
	//save all of the configs stored in the struct to the operating system
	if (tcsetattr(m_serial, TCSANOW, &tty)) {
		fprintf(stderr,"Error %i from tcsetattr(): %s\n", errno, strerror(errno));
		return 1;
	}	
    return 0;
}

int SerialPort::close(){
    //close the file descriptor
    if(::close(m_serial)){
        fprintf(stderr, "Error from close()\n");
        return 1;
    }
    return 0;
}