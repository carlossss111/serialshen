/*
* Reads a line ending with '\n' from a serial port.
* Daniel Robinson 2022
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#define SERIAL_DEVICE "/dev/ttyACM0"

int main(int argc, char** argv){
	//open the serial port
	int serialPort = open(SERIAL_DEVICE, O_RDWR);
	if(serialPort < 0){
		fprintf(stderr,"Error %i from open(): %s\n", errno, strerror(errno));
		exit(1);
	}

	//read in i/o flags (see "termios" struct)
	struct termios tty;
	if(tcgetattr(serialPort, &tty)) {
		fprintf(stderr,"Error %i from tcgetattr(): %s\n", errno, strerror(errno));
		exit(1);
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
	if (tcsetattr(serialPort, TCSANOW, &tty)) {
		fprintf(stderr,"Error %i from tcsetattr(): %s\n", errno, strerror(errno));
		exit(1);
	}	

	//read from the port
	for(;;){
		char buffer[128];
		int numOfBytes = read(serialPort, &buffer, sizeof(buffer));
		if(!numOfBytes){
			fprintf(stderr,"Error from read(): Timed out.\n");
			exit(1);
		}
		
		//output result as an ASCII string
		printf("Number of bytes = %d\nString = %s", numOfBytes, buffer);
	}
	return 0;
}

//useful advice from https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
