#include "serial_linux.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>
#include <string>

static speed_t to_speed(int baud){
  switch(baud){
    case 9600: return B9600;
    case 19200: return B19200;
    case 38400: return B38400;
    case 57600: return B57600;
    case 115200: return B115200;
    default: return B115200;
  }
}

std::string readLineFromSerial(const char* device, int baud){
  int fd = open(device, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  if(fd < 0) return {};

  termios tio{}; tcgetattr(fd, &tio);
  cfmakeraw(&tio);
  cfsetispeed(&tio, to_speed(baud));
  cfsetospeed(&tio, to_speed(baud));
  tio.c_cflag |= (CLOCAL | CREAD);
  tcsetattr(fd, TCSANOW, &tio);

  std::string line;
  char ch;
  // Esperar hasta 1.5 s por datos
  for(;;){
    fd_set set; FD_ZERO(&set); FD_SET(fd, &set);
    timeval tv{1, 500000}; // 1.5 s
    int r = select(fd+1, &set, nullptr, nullptr, &tv);
    if(r <= 0) break; // timeout o error
    if(read(fd, &ch, 1) == 1){
      if(ch == '\n' || ch == '\r'){
        if(!line.empty()) break;
        else continue;
      }
      line.push_back(ch);
      if(line.size() > 2048) break; // límite de seguridad
    }
  }
  close(fd);
  return line;
}
