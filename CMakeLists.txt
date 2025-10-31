cmake_minimum_required(VERSION 3.10)
project(SensorIoTSystem CXX)

# Habilitar C++11 o superior
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Definir los archivos fuente
set(SOURCE_FILES
    main.cpp
    serial_linux.cpp
    SensorSystem.cpp
)

# Definir los archivos de cabecera
set(HEADER_FILES
    SensorSystem.h
)

# Definir el nombre del ejecutable y los archivos fuente
add_executable(sensor_manager ${SOURCE_FILES})
## Documentación con Doxygen (más robusta)
# Esta sección genera un Doxyfile desde la plantilla Doxyfile.in y añade
# un target CMake 'doc' que ejecuta Doxygen si está disponible en el sistema.
set(PROJECT_DESCRIPTION "Sistema de gestión de sensores usando polimorfismo y templates")

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Rellena la plantilla Doxyfile.in con variables de CMake
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found. Documentation target 'doc' will not be available.")
endif()