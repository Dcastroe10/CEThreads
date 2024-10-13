#ifndef CTOSERIAL_H
#define CTOSERIAL_H

#include <cstddef>
#ifdef __cplusplus
extern "C" {
#endif





#include <termios.h>

// Function to initialize serial communication
int serial_init(const char *portname, int baudrate);

// Function to send data to Arduino
int serial_send(int fd, const char *data);


//
int serial_read(int fd, char *buffer, size_t size);

// Function to close serial communication
void serial_close(int fd);






#ifdef __cplusplus
}
#endif

#endif // CTOSERIAL_H
