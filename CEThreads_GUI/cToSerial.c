#include "cToSerial.h"
#include <stdio.h>
#include <unistd.h>    // For close() and write()
#include <fcntl.h>     // For file control options like open()
#include <string.h>    // For memset()

// Initialize serial communication
int serial_init(const char *portname, int baudrate) {
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd == -1) {
        perror("Error opening serial port");
        return -1;
    }

    // Configure port settings
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd, &tty) != 0) {
        perror("Error getting attributes");
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, baudrate);
    cfsetispeed(&tty, baudrate);

    // 8 data bits, no parity, 1 stop bit
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;  // No signaling chars, no echo, no canonical processing
    tty.c_oflag = 0;  // No remapping, no delays

    // Set input and output attributes
    tty.c_cc[VMIN]  = 1; // Read at least 1 character
    tty.c_cc[VTIME] = 5; // Wait for 0.5 seconds

    tty.c_cflag |= (CLOCAL | CREAD);    // Enable the receiver and set local mode
    tty.c_cflag &= ~(PARENB | PARODD);  // Disable parity
    tty.c_cflag &= ~CSTOPB;             // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;            // Disable hardware flow control

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error setting attributes");
        close(fd);
        return -1;
    }

    return fd;
}

// Send data to Arduino
int serial_send(int fd, const char *data) {
    int len = strlen(data);
    int bytes_written = write(fd, data, len);

    if (bytes_written == -1) {
        perror("Error writing to serial port");
        return -1;
    }

    return bytes_written;
}

// Close serial communication
void serial_close(int fd) {
    close(fd);
}

int serial_read(int fd, char *buffer, size_t size)
{
    int bytes_read = read(fd, buffer, size);
    if (bytes_read == -1) {
        perror("Error reading from serial port");
        return -1;
    }

    buffer[bytes_read] = '\0';  // Null-terminate the string
    return bytes_read;
}
/*
 *         int bytes_read = serial_read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            qDebug() << "Received from Arduino: %s\n" << buffer;
        }*/
