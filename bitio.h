#ifndef BITIO_H
#define BITIO_H

#include <stdint.h>
#include <stdio.h>

typedef struct BitWriter{
    FILE *file;
    unsigned char buffer; // can hold 8 bits 0-255
    int bit_count; // # of useful bits held
} BitWriter;

typedef struct BitReader{
    FILE *file;
    unsigned char buffer;
    int bit_count;
} BitReader;

void bit_writer_init(BitWriter *writer, FILE *file);
int bit_writer_write_bit(BitWriter *writer, int bit);
int bit_writer_write_code(BitWriter *writer, uint32_t code, uint8_t length);
int bit_writer_flush(BitWriter *writer);

void bit_reader_init(BitReader *reader, FILE *file);
int bit_reader_read_bit(BitReader *reader);

#endif