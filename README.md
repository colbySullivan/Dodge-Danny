# SFML-Template
Commands to build statically and dynamically are provided.

# Running

## Makefile run
To compile
```
g++ -c main.cpp -I"SFML-2.6.0\include"
```

Create runnable exe
```
g++ main.o -o main -L"SFML-2.6.0\lib" -lsfml-graphics -lsfml-window -lsfml-system
```

If make is installed on your device then run the following command

```
make
```
To get rid of the main.o file run
```
make clean
```

# Scripts
### Static build
```
.\static.bat
```
### Dynamic build
```
.\dynamic.bat
```
### Remove main.o
```
.\clean.bat
```

## Demo
![dennisDemo](https://github.com/colbySullivan/Dodge-Danny/assets/88957296/e8ec6d19-4ce8-4489-935e-8855c93de4ea)

### Plans
There are currently no plans to add to this as it was a learning project and I quite like the fever dream aesthetic.

# Dependencies
Install of SMFL is required to run the program.

https://www.sfml-dev.org/download.php

Make sure to place the folder into the C drive or replace the location in the run/compile commands.
