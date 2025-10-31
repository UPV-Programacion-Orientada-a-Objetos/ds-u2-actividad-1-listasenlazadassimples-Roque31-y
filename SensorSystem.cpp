#include "SensorSystem.h"

// Implementación de SensorBase
SensorBase::SensorBase(const char* sensorId) {
    strncpy(id, sensorId, sizeof(id) - 1);
    id[sizeof(id) - 1] = '\0';
}

SensorBase::~SensorBase() {
    // Destructor virtual puro - implementación requerida
}

const char* SensorBase::getId() const {
    return id;
}