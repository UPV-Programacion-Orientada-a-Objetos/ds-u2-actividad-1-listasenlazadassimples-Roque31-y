// Example: how to create sensors and register readings (snippet for Doxygen @example)
#include "SensorSystem.h"

int main_example() {
    SistemaGestion sistema;
    sistema.agregarSensor(new SensorTemperatura("T-EX-01"));
    sistema.agregarSensor(new SensorPresion("P-EX-01"));
    sistema.agregarSensor(new SensorVibracion("V-EX-01"));

    // Registrar lecturas de ejemplo
    sistema.buscarSensor("T-EX-01")->registrarLectura(23.4f);
    sistema.buscarSensor("P-EX-01")->registrarLectura(101.0f);
    sistema.buscarSensor("V-EX-01")->registrarLectura(12.0f);

    // Ejecutar procesamiento polimórfico
    sistema.ejecutarProcesamiento();

    return 0; // no se usa en la app principal; es solo un snippet para la docs
}
