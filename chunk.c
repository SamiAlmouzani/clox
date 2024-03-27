#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk* chunk){
  chunk->capacity = 0;
  chunk->count = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants); 
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);

  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

static int getLine(Chunk* chunk, int index){
  if(chunk->lines[index] > 128){
    return chunk->lines[index]; 
  }
  else {
    return chunk->lines[index - 1];
  }
}

void writeChunk(Chunk* chunk, uint8_t byte, int line){
  if(chunk->capacity < chunk->count + 1){
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code,
        oldCapacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines,
    oldCapacity, chunk->capacity);
  }
  chunk->code[chunk->count] = byte;

  /** Encoding Starts Here*/

  unsigned int prev = chunk->lines[chunk->count - 1];
  // checks if prev number was encoded
  if(chunk->count > 0 && prev > 128){
    // then checks to see if the number before the encoded one is equal to line #
    if(chunk->lines[chunk->count - 2] == line){
      chunk->lines[chunk->count - 1] = prev + 1;
    }
    // places the line value if it is not equal to the tuple of ints
    else{
      chunk->lines[chunk->count] = line;
      chunk->count++;
    }
  } 
  // checks if count is 1 or greater AND if the prev == line 
  else if(chunk->count > 0 && prev == line){
      // this will be decoded with (value - 128) to get run_length in getLine()
      chunk->lines[chunk->count] = 129;
      chunk->count++;
  }
  // evaluates if beginning of a chunk or if the prev doesn't equal to the line # 
  else {
    chunk->lines[chunk->count] = line;
    chunk->count++;
  }

   /** End */
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}