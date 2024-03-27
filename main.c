#include "common.h"
#include "chunk.h"
#include "debug.h"
#include <stdio.h>

int main(int arc, const char* argv[]){
    Chunk chunk;
    initChunk(&chunk);
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 2);
    writeChunk(&chunk, constant, 2);
    writeChunk(&chunk, constant, 2);
    writeChunk(&chunk, constant, 2);
    writeChunk(&chunk, constant, 2);
    writeChunk(&chunk, OP_RETURN, 3);
    for(int i = 0; i < chunk.count; i++){
        printf("%d ", chunk.lines[i]);
    }
    //disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
}