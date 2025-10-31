#include "SensorSystem.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <limits>
#include <thread>
#include <chrono>

/**
 * @brief Simula una lectura desde el puerto serial para un tipo específico de sensor
 * @param tipoSensor Tipo de sensor (1:Temperatura, 2:Presión, 3:Vibración)
 * @return float Valor simulado de la lectura
 */
float simularLecturaSerial(int tipoSensor) {
    // Simular delay de comunicación serial
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    switch (tipoSensor) {
        case 1: // Temperatura (float)
            return 30.0f + static_cast<float>(rand() % 200) / 10.0f;
        case 2: // Presión (int)
            return 70.0f + static_cast<float>(rand() % 20);
        case 3: // Vibración (int)
            return static_cast<float>(rand() % 50); // 0-49 vibraciones
        default:
            return 0.0f;
    }
}

/**
 * @brief Muestra el menú principal y maneja la interacción del usuario
 * @param sistema Referencia al sistema de gestión de sensores
 */
void menu(SistemaGestion& sistema) {
    int opcion;
    std::cout << "\n--- Sistema IoT de Monitoreo Polimórfico ---\n";
    do {
        std::cout << "\nSeleccione una opcion:\n";
        std::cout << "1. Crear Sensor de Temperatura (FLOAT)\n";
        std::cout << "2. Crear Sensor de Presion (INT)\n";
        std::cout << "3. Crear Sensor de Vibracion (INT)\n";
        std::cout << "4. Simular Lectura Serial y Registrar\n";
        std::cout << "5. Ejecutar Procesamiento Polimórfico\n";
        std::cout << "6. Imprimir Info de Sensores\n";
        std::cout << "7. Cerrar Sistema (Liberar Memoria)\n";
        std::cout << "Opcion: ";
        
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            opcion = 0;
            std::cout << "Entrada invalida. Intente de nuevo.\n";
            continue;
        }

        switch (opcion) {
            case 1: {
                char id[50];
                std::cout << "Ingrese ID del sensor de Temperatura (ej: T-001): ";
                std::cin >> id;
                SensorBase* nuevo = new SensorTemperatura(id);
                sistema.agregarSensor(nuevo);
                std::cout << "Sensor '" << id << "' creado e insertado en la lista de gestion.\n";
                break;
            }
            case 2: {
                char id[50];
                std::cout << "Ingrese ID del sensor de Presion (ej: P-105): ";
                std::cin >> id;
                SensorBase* nuevo = new SensorPresion(id);
                sistema.agregarSensor(nuevo);
                std::cout << "Sensor '" << id << "' creado e insertado en la lista de gestion.\n";
                break;
            }
            case 3: {
                char id[50];
                std::cout << "Ingrese ID del sensor de Vibracion (ej: V-001): ";
                std::cin >> id;
                SensorBase* nuevo = new SensorVibracion(id);
                sistema.agregarSensor(nuevo);
                std::cout << "Sensor '" << id << "' creado e insertado en la lista de gestion.\n";
                break;
            }
            case 4: {
                char id[50];
                std::cout << "Ingrese ID del sensor para registrar lectura: ";
                std::cin >> id;
                SensorBase* sensor = sistema.buscarSensor(id);
                
                if (sensor) {
                    float lectura = 0.0f;
                    char tipo = id[0];
                    
                    if (tipo == 'T' || tipo == 't') {
                        lectura = simularLecturaSerial(1);
                        std::cout << "Simulando lectura serial FLOAT para " << id << ": " << lectura << "\n";
                    } else if (tipo == 'P' || tipo == 'p') {
                        lectura = simularLecturaSerial(2);
                        std::cout << "Simulando lectura serial INT para " << id << ": " << static_cast<int>(lectura) << "\n";
                    } else if (tipo == 'V' || tipo == 'v') {
                        lectura = simularLecturaSerial(3);
                        std::cout << "Simulando lectura serial INT (Vibraciones) para " << id << ": " << static_cast<int>(lectura) << "\n";
                    } else {
                        lectura = simularLecturaSerial(1);
                        std::cout << "Simulando lectura serial (Default) para " << id << ": " << lectura << "\n";
                    }
                    
                    sensor->registrarLectura(lectura);
                } else {
                    std::cout << "Error: Sensor con ID '" << id << "' no encontrado.\n";
                }
                break;
            }
            case 5: {
                sistema.ejecutarProcesamiento();
                break;
            }
            case 6: {
                std::cout << "\n--- Estado Actual de los Sensores ---\n";
                sistema.imprimirTodos();
                break;
            }
            case 7: {
                std::cout << "Iniciando cierre del sistema...\n";
                break;
            }
            default:
                std::cout << "Opcion no valida.\n";
        }
    } while (opcion != 7);
}

int main() {
    // Inicializar generador de números aleatorios
    srand(static_cast<unsigned int>(time(0)));
    
    // Crear sistema de gestión
    SistemaGestion sistema;
    
    std::cout << "\n--- Creando Sensores de Ejemplo ---\n";
    sistema.agregarSensor(new SensorTemperatura("T-001"));
    sistema.agregarSensor(new SensorPresion("P-105"));
    sistema.agregarSensor(new SensorVibracion("V-001"));
    
    std::cout << "\n--- Registro de Lecturas de Ejemplo ---\n";
    sistema.buscarSensor("T-001")->registrarLectura(45.3f);
    sistema.buscarSensor("T-001")->registrarLectura(42.1f);
    sistema.buscarSensor("P-105")->registrarLectura(80.0f);
    sistema.buscarSensor("P-105")->registrarLectura(85.0f);
    sistema.buscarSensor("V-001")->registrarLectura(25.0f);
    sistema.buscarSensor("V-001")->registrarLectura(30.0f);
    
    menu(sistema);

    return 0;
}