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

## Known Limitations

- Input and output paths must be different. Using the same path can truncate
   and destroy the input file.
- When multiple symbols have the same frequency, this implementation uses a
   deterministic tie-breaking order. The file format does not store the Huffman
   tree explicitly, so compressed files should be decompressed with this
   implementation or another implementation using the same tree-building rules.
- Huffman codes are stored in 32 bits. Highly unbalanced frequency
   distributions that require codes longer than 32 bits are not supported.
- Corrupted frequency tables are not fully validated. Invalid headers may be
   interpreted as empty files or may request excessively large output.
- Single-symbol compressed files do not validate the encoded bit stream during
   decompression.
- The compressed format stores integers using the host system's native byte
   order, so files may not be portable between systems with different
   architectures.
- Final file-close errors are not currently reported, so some disk-write
   failures may be reported as successful operations.

## Project Files

- `main.c` - Command-line argument handling.
- `huffman.c` and `huffman.h` - Compression, decompression, and file format.
- `huffman_tree.c` and `huffman_tree.h` - Huffman tree construction and codes.
- `priority_queue.c` and `priority_queue.h` - Min-heap used to build the tree.
- `bitio.c` and `bitio.h` - Bit-level input and output.
