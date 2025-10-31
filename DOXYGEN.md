Generar documentación con Doxygen (opcional, recomendado)

Este repositorio incluye una plantilla `Doxyfile.in` y un objetivo CMake `doc` que ejecuta Doxygen desde el directorio de `build`.

Si usas CMake y tienes Doxygen instalado, desde la carpeta `build` ejecuta:

```powershell
cmake --build . --target doc
```

Si prefieres ejecutar Doxygen directamente sin CMake:

```powershell
# Genera un Doxyfile por defecto
doxygen -g Doxyfile
# Edita INPUT y OUTPUT_DIRECTORY según necesites
doxygen Doxyfile
```

Instala Doxygen (https://www.doxygen.nl/) y Graphviz (opcional) para generar diagramas.

La configuración por defecto de `Doxyfile.in` genera HTML en la carpeta `doxygen_docs/html` en la raíz del repositorio.
