# Paralelni milkshake

Project for University class "Porazdeljeni sistemi" (eng. Distributed systems). The goal of the project is to construct a pipeline parallel execution model for an encoding problem: given some images, perform run-length and Huffman encoding (with optimal code for each image) and save the encoded result.

For a single image, four steps are performed:
1. Read image
2. Run-length encoding
3. Huffman encoding
4. Write encoded data

The project consists of three programs written in `C++`:
- `sequential` uses a loop that performs the four steps outlined above
- `pipeline` uses four threads for the four stages; producer-consumer approach with a buffer
- `pipesquare` is an upgraded version `pipeline` that supports a custom number of threads for each stage

## Build

Requires a POSIX system for `pthread.h` support and `CMake` for building. To build the project, execute the following commands from the project root directory:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Different programs can be executed with `make`:

```bash
make seq # executes program sequential via run.sh file in same directory
make pip # executes program pipeline via run.sh file in same directory
make sqr # executes program pipesquare via run.sh file in same directory
```

## License

This project uses [MIT](https://choosealicense.com/licenses/mit/) license; see `LICENSE` file for more details.
