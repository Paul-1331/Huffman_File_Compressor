#include "bitio.h"

void bit_writer_init(BitWriter*writer,FILE*file){
    writer->file = file;
    writer->bit_count = 0;
    writer->buffer = 0;
}

int bit_writer_write_bit(BitWriter*writer,int bit){
    writer->buffer = (unsigned char)((writer->buffer<<1)|(bit&1));
    writer->bit_count++;

    if(writer->bit_count==8){
        if(fputc(writer->buffer,writer->file)==EOF){
            return 0;
        }
        writer->bit_count = 0;
        writer->buffer = 0;
    }
    return 1;
}

int bit_writer_write_code(BitWriter*writer,uint32_t code, uint8_t len){
    for(int i = len-1;i>=0;i--){
        if(!bit_writer_write_bit(writer,(code>>i)&1)){
            return 0;
        }
    }
    return 1;
}

int bit_writer_flush(BitWriter*writer){
    if(writer->bit_count==0){
        return 1;
    }

    writer->buffer <<= (8-writer->bit_count);

    if(fputc(writer->buffer,writer->file)==EOF){
        return 0;
    }
    writer->buffer = 0;
    writer->bit_count = 0;
    return 1;
}

void bit_reader_init(BitReader *reader, FILE *file)
{
    reader->file = file;
    reader->buffer = 0;
    reader->bit_count = 0;
}

int bit_reader_read_bit(BitReader*reader){
    if(reader->bit_count==0){
        int c = fgetc(reader->file); // fgetc returns an int 0 - 255 and EOF for error or end of file
        if(c==EOF){
            return -1;
        }
        reader->buffer = (unsigned char)c;
        reader->bit_count = 8;
    }

    int bit  = (reader->buffer>>7)&1;
    reader->buffer <<= 1;
    reader->bit_count --;
    return bit;
}