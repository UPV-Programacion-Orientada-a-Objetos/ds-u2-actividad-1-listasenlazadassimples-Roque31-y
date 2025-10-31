#pragma once
#include <string>

/**
 * Lee una línea del dispositivo serial (bloqueante con timeout ~1.5s).
 * Devuelve cadena vacía en timeout o error.
 * Ejemplo de uso:
 *   std::string s = readLineFromSerial("/dev/ttyUSB0", 115200);
 */
std::string readLineFromSerial(const char* device = "/dev/ttyUSB0", int baud = 115200);
