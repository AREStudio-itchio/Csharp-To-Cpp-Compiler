# C# To C++ Compiler
**C# To C++ Compiler es un compilador programado en C++ para compilar código C# a C++**, el compilador coge tu código C# y reemplaza toda la librería que está siendo usada en using por
```c++
#include <LibreriaUsada.h>
```
y todo el código como arrays, diccionarios, variables, serán convertidas en su equivalente a C++, el main, también, y los usos de librerías, como por ejemplo:
```c#
Console.WriteLine("Hello, world!");
```
será convertido en su equivalente a C++:
```c++
Console::WriteLine("Hello, world!")
```
recuerda usar using namespace libreria; para poder quitar el namespace de la librería.

# Licencia
El Compilador está **bajo MIT, pero esto no se aplica a tu ejecutable ni al código generado, el código es 100% tuyo.**
