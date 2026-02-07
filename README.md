# C# To C++ Compiler
**C# To C++ Compiler es un compilador programado en C++ para compilar código C# a C++**, el objetivo de este compilador es poder compilar código C# a C++ para luego ser compilado a código de máquina sin depender del compilador de dotnet (.NET), el compilador coge tu código C# y reemplaza toda la librería que está siendo usada en using por
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

# Uso
Usa este comando en el símbolo del sistema para compilar tu código:
```cmd
.\cstocpp codigoc#.cs nombredelarchivocpp.cpp
```
para que sea más cómodo, recuerda usar el PATH de Windows para ejecutar el ejecutable desde cualquier carpeta.

# Licencia
El Compilador está **bajo MIT, pero esto no se aplica a tu ejecutable ni al código generado, el código es 100% tuyo.**
