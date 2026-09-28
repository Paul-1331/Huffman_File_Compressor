# Huffman File Compressor

This project is a command-line file compressor written in C. It uses a Huffman
tree to encode input bytes and stores the byte-frequency table in the output so
that the tree can be reconstructed during decompression.

## Requirements

- A C11-compatible compiler, such as GCC or Clang
- GNU Make or another Make-compatible build tool

## Build

Build the `huffman` executable from the project directory:

```sh
make
```

The Makefile also provides these targets:

```sh
make          # Build the huffman executable
make clean    # Remove the executable and object files
```

To rebuild from scratch, run `make clean && make`.

## Usage

Compress a file with:

```sh
./huffman compress <input_file> <output_file>
```

Decompress a file with:

```sh
./huffman decompress <input_file> <output_file>
```

For example:

```sh
./huffman compress input.txt input.huf
./huffman decompress input.huf restored.txt
cmp input.txt restored.txt
```

The compressor handles empty files and files containing a single distinct byte.
Existing output files are overwritten. The input and output paths should be
different files.

## File Format

Each compressed file contains:

1. The four-byte magic value `HUF1`.
2. A frequency table of 256 unsigned 64-bit values, one for each possible byte.
3. The Huffman-encoded bit stream, padded with zero bits at the end of the last
   byte when necessary.

Decompression validates the magic value and uses the frequency table to know
how many bytes to reconstruct, so padding bits are ignored.

## Project Files

- `main.c` - Command-line argument handling.
- `huffman.c` and `huffman.h` - Compression, decompression, and file format.
- `huffman_tree.c` and `huffman_tree.h` - Huffman tree construction and codes.
- `priority_queue.c` and `priority_queue.h` - Min-heap used to build the tree.
- `bitio.c` and `bitio.h` - Bit-level input and output.
