#ifndef SENSOR_SYSTEM_H
#define SENSOR_SYSTEM_H

/**
 * @file SensorSystem.h
 * @brief Sistema de gestión de sensores IoT usando polimorfismo y templates
 * @author KirbyStone69
 * @date 2025-10-30
 * 
 * Sistema polimórfico de gestión de sensores que implementa:
 * - Jerarquía de clases con clase base abstracta
 * - Templates para manejo genérico de datos
 * - Listas enlazadas implementadas manualmente
 * - Gestión de memoria con la regla de los tres
 * - Simulación de lecturas seriales
 */

#include <iostream>
#include <cstring>

// Forward declarations
template <typename T> struct Nodo;
template <typename T> class ListaSensor;

/**
 * @brief Clase base abstracta que define la interfaz común para todos los sensores
 * 
 * Esta clase implementa el patrón de diseño Template Method a través de métodos
 * virtuales puros que deben ser implementados por las clases derivadas.
 */
class SensorBase {
protected:
    char id[50]; ///< Identificador único del sensor
public:
    /**
     * @brief Constructor de la clase base
     * @param sensorId Identificador único del sensor
     */
    SensorBase(const char* sensorId);
    
    /**
     * @brief Destructor virtual puro
     */
    virtual ~SensorBase() = 0;
    
    /**
     * @brief Registra una nueva lectura del sensor
     * @param lectura Valor de la lectura a registrar
     */
    virtual void registrarLectura(float lectura) = 0;
    
    /**
     * @brief Procesa las lecturas almacenadas según la lógica específica de cada sensor
     */
    virtual void procesarLectura() = 0;
    
    /**
     * @brief Imprime información del sensor y sus lecturas
     */
    virtual void imprimirInfo() const = 0;
    
    /**
     * @brief Obtiene el identificador del sensor
     * @return const char* Identificador del sensor
     */
    virtual const char* getId() const;
};

/**
 * @brief Nodo genérico para la lista enlazada
 * @tparam T Tipo de dato a almacenar
 */
template <typename T>
struct Nodo {
    T dato;                ///< Dato almacenado en el nodo
    Nodo<T>* siguiente;    ///< Puntero al siguiente nodo
    
    /**
     * @brief Constructor del nodo
     * @param valor Dato a almacenar
     */
    Nodo(T valor) : dato(valor), siguiente(nullptr) {
        std::cout << "[Log] Nodo<" << typeid(T).name() << "> " << dato << " creado.\n";
    }
    
    /**
     * @brief Destructor del nodo
     */
    ~Nodo() {
        std::cout << "[Log] Nodo<" << typeid(T).name() << "> " << dato << " liberado.\n";
    }
};

/**
 * @brief Lista enlazada genérica para almacenar lecturas de sensores
 * @tparam T Tipo de dato de las lecturas
 */
template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;    ///< Puntero al primer nodo
    int cantidad;       ///< Cantidad de nodos en la lista

public:
    /**
     * @brief Constructor por defecto
     */
    ListaSensor() : cabeza(nullptr), cantidad(0) {
        std::cout << "[Log] ListaSensor<" << typeid(T).name() << "> creada.\n";
    }
    
    /**
     * @brief Destructor
     */
    ~ListaSensor() {
        std::cout << "[Destructor ListaSensor] Liberando lista interna...\n";
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        cabeza = nullptr;
    }
    
    /**
     * @brief Constructor de copia
     */
    ListaSensor(const ListaSensor& other) : cabeza(nullptr), cantidad(0) {
        Nodo<T>* actual = other.cabeza;
        while (actual != nullptr) {
            insertar(actual->dato);
            actual = actual->siguiente;
        }
    }
    
    /**
     * @brief Operador de asignación
     */
    ListaSensor& operator=(const ListaSensor& other) {
        if (this != &other) {
            // Liberar memoria actual
            while (cabeza != nullptr) {
                Nodo<T>* temp = cabeza;
                cabeza = cabeza->siguiente;
                delete temp;
            }
            cantidad = 0;
            
            // Copiar datos
            Nodo<T>* actual = other.cabeza;
            while (actual != nullptr) {
                insertar(actual->dato);
                actual = actual->siguiente;
            }
        }
        return *this;
    }
    
    /**
     * @brief Inserta un nuevo valor al final de la lista
     * @param valor Valor a insertar
     */
    void insertar(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            Nodo<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
        cantidad++;
        std::cout << "[Log] Insertando Nodo<" << typeid(T).name() << "> valor: " << valor << "\n";
    }
    
    /**
     * @brief Elimina el nodo con el valor más bajo
     */
    void eliminarMenor() {
        if (cabeza == nullptr) return;
        
        Nodo<T>* anterior = nullptr;
        Nodo<T>* actual = cabeza;
        Nodo<T>* menorAnterior = nullptr;
        Nodo<T>* menor = cabeza;
        
        while (actual != nullptr) {
            if (actual->dato < menor->dato) {
                menor = actual;
                menorAnterior = anterior;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        
        if (menorAnterior == nullptr) {
            cabeza = menor->siguiente;
        } else {
            menorAnterior->siguiente = menor->siguiente;
        }
        
        std::cout << "[Log] Eliminando valor menor: " << menor->dato << "\n";
        delete menor;
        cantidad--;
    }
    
    /**
     * @brief Calcula el promedio de todos los valores en la lista
     * @return float Promedio calculado
     */
    float calcularPromedio() const {
        if (cabeza == nullptr) return 0.0f;
        
        T suma = 0;
        Nodo<T>* actual = cabeza;
        int count = 0;
        
        while (actual != nullptr) {
            suma += actual->dato;
            actual = actual->siguiente;
            count++;
        }
        
        return static_cast<float>(suma) / count;
    }
    
    int getCantidad() const { return cantidad; }
    
    T getPrimero() const {
        if (cabeza != nullptr) {
            return cabeza->dato;
        }
        return T();
    }
};

/**
 * @brief Clase concreta para sensores de temperatura
 */
class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float> historial; ///< Historial de lecturas de temperatura

public:
    SensorTemperatura(const char* sensorId) : SensorBase(sensorId) {
        std::cout << "[SensorTemperatura] Creado sensor: " << sensorId << "\n";
    }
    
    ~SensorTemperatura() override {
        std::cout << "[Destructor SensorTemperatura] Liberando sensor: " << id << "\n";
    }
    
    void registrarLectura(float lectura) override {
        historial.insertar(lectura);
        std::cout << "[Temperatura] Registrada lectura: " << lectura << " en " << id << "\n";
    }
    
    void procesarLectura() override {
        std::cout << "[Procesando Temperatura " << id << "] ";
        int lecturasAntes = historial.getCantidad();
        
        if (lecturasAntes > 1) {
            historial.eliminarMenor();
            float promedio = historial.calcularPromedio();
            std::cout << "Lectura mas baja eliminada. Promedio restante: " << promedio 
                      << " sobre " << historial.getCantidad() << " lecturas.\n";
        } else if (lecturasAntes == 1) {
            float unicaLectura = historial.getPrimero();
            std::cout << "Una unica lectura: " << unicaLectura << ". No se elimina nada.\n";
        } else {
            std::cout << "No hay lecturas para procesar.\n";
        }
    }
    
    void imprimirInfo() const override {
        std::cout << "[T-INFO " << id << "] Tipo: Temperatura, Lecturas: " 
                  << historial.getCantidad() << ", Promedio: " 
                  << historial.calcularPromedio() << "\n";
    }
};

/**
 * @brief Clase concreta para sensores de vibración
 */
class SensorVibracion : public SensorBase {
private:
    ListaSensor<int> historial; ///< Historial de conteos de vibración

public:
    SensorVibracion(const char* sensorId) : SensorBase(sensorId) {
        std::cout << "[SensorVibracion] Creado sensor: " << sensorId << "\n";
    }
    
    ~SensorVibracion() override {
        std::cout << "[Destructor SensorVibracion] Liberando sensor: " << id << "\n";
    }
    
    void registrarLectura(float lectura) override {
        int conteoVibraciones = static_cast<int>(lectura);
        historial.insertar(conteoVibraciones);
        std::cout << "[Vibracion] Registrada lectura: " << conteoVibraciones << " en " << id << "\n";
    }
    
    void procesarLectura() override {
        std::cout << "[Procesando Vibracion " << id << "] ";
        if (historial.getCantidad() > 0) {
            float promedio = historial.calcularPromedio();
            std::cout << "Conteo total de vibraciones: " << historial.getCantidad() 
                      << ", Promedio por lectura: " << promedio << "\n";
        } else {
            std::cout << "No hay lecturas para procesar.\n";
        }
    }
    
    void imprimirInfo() const override {
        std::cout << "[V-INFO " << id << "] Tipo: Vibracion, Lecturas: " 
                  << historial.getCantidad() << ", Promedio: " 
                  << historial.calcularPromedio() << "\n";
    }
};

/**
 * @brief Clase concreta para sensores de presión
 */
class SensorPresion : public SensorBase {
private:
    ListaSensor<int> historial; ///< Historial de lecturas de presión

public:
    SensorPresion(const char* sensorId) : SensorBase(sensorId) {
        std::cout << "[SensorPresion] Creado sensor: " << sensorId << "\n";
    }
    
    ~SensorPresion() override {
        std::cout << "[Destructor SensorPresion] Liberando sensor: " << id << "\n";
    }
    
    void registrarLectura(float lectura) override {
        int lecturaInt = static_cast<int>(lectura);
        historial.insertar(lecturaInt);
        std::cout << "[Presion] Registrada lectura: " << lecturaInt << " en " << id << "\n";
    }
    
    void procesarLectura() override {
        std::cout << "[Procesando Presion " << id << "] ";
        float promedio = historial.calcularPromedio();
        std::cout << "Promedio de lecturas: " << promedio 
                  << " sobre " << historial.getCantidad() << " lecturas.\n";
    }
    
    void imprimirInfo() const override {
        std::cout << "[P-INFO " << id << "] Tipo: Presion, Lecturas: " 
                  << historial.getCantidad() << ", Promedio: " 
                  << historial.calcularPromedio() << "\n";
    }
};

/**
 * @brief Nodo para la lista de gestión del sistema
 */
struct NodoGestion {
    SensorBase* sensor;      ///< Puntero al sensor (polimórfico)
    NodoGestion* siguiente;  ///< Puntero al siguiente nodo
    
    NodoGestion(SensorBase* s) : sensor(s), siguiente(nullptr) {}
    
    ~NodoGestion() {
        std::cout << "[Destructor General] Liberando Nodo: " << sensor->getId() << "\n";
        delete sensor;
    }
};

/**
 * @brief Sistema principal de gestión de sensores
 */
class SistemaGestion {
private:
    NodoGestion* cabeza;     ///< Puntero al primer nodo de la lista

public:
    SistemaGestion() : cabeza(nullptr) {
        std::cout << "[SistemaGestion] Sistema creado.\n";
    }
    
    ~SistemaGestion() {
        std::cout << "\n--- Liberación de Memoria en Cascada ---\n";
        while (cabeza != nullptr) {
            NodoGestion* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
        std::cout << "Sistema cerrado. Memoria limpia.\n";
    }
    
    void agregarSensor(SensorBase* sensor) {
        NodoGestion* nuevoNodo = new NodoGestion(sensor);
        
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            NodoGestion* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
        std::cout << "[Sistema] Sensor '" << sensor->getId() << "' agregado al sistema.\n";
    }
    
    SensorBase* buscarSensor(const char* id) {
        NodoGestion* actual = cabeza;
        while (actual != nullptr) {
            if (strcmp(actual->sensor->getId(), id) == 0) {
                return actual->sensor;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    void ejecutarProcesamiento() {
        std::cout << "\n--- Ejecutando Polimorfismo ---\n";
        NodoGestion* actual = cabeza;
        while (actual != nullptr) {
            std::cout << "-> Procesando Sensor " << actual->sensor->getId() << "...\n";
            actual->sensor->procesarLectura();
            actual = actual->siguiente;
        }
    }
    
    void imprimirTodos() {
        NodoGestion* actual = cabeza;
        while (actual != nullptr) {
            actual->sensor->imprimirInfo();
            actual = actual->siguiente;
        }
    }
};

#endif // SENSOR_SYSTEM_H