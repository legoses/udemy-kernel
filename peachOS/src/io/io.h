#ifndef IO_H
#define IO_H

unsigned char insb(unsigned short port); // reas sungle byte from  port
unsigned short insw(unsigned short port); // read two bytes from port

void outb(unsigned short port, unsigned char val); // out put one byte to port
void outw(unsigned short port, unsigned short val); // output two bytes to port

#endif
