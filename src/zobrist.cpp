#include "zobrist.h"

// defines `const static uint64_t zobrist_array_[]`.
#include "zobrist_array.include"

const uint64_t *ZobristHash::get_zobrist_array()
{
  return zobrist_array_;
}
