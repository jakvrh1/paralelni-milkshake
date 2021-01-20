# Paralelni milkshake

Project for University class "Porazdeljeni sistemi" (eng. Multithreading).

## Build

Requires POSIX system for pthreads and CMake.

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
make seq # executes program sequential via run.sh file in same directory
make pip # executes program pipeline via run.sh file in same directory
make sqr # executes program pipesquare via run.sh file in same directory
```

## License
[MIT](https://choosealicense.com/licenses/mit/)
