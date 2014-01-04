#ifndef TRANSPOSITION_H__
#define TRANSPOSITION_H__

#include <memory>
#include <inttypes.h>
#include "zobrist.h"

struct TTNode
{
  public:
    TTNode()
      : _zobrist(), _depth(0), _score(0) {}

    TTNode(ZobristHash z, uint32_t d, float s)
      : _zobrist(z), _depth(d), _score(s) {}

    ZobristHash _zobrist;
    uint32_t _depth;
    float _score;
    // SType type;
    // Step beststep;
    // bool aged;
};

class TranspositionTable
{
  public:

    TranspositionTable(uint64_t size_MB) 
      : _store(new std::vector<TTNode>), _nelt(0)
        // _size_MB(size_MB), _hits(0), _miss(0), _collisions(0)
    {
      _nelt = 2;
      for( _nelt=2; (_nelt << 1) * sizeof(TTNode) <= (1024 * 1024 * size_MB); _nelt <<=1) ;
      assert(_nelt >= 2);
      _store->resize(_nelt);
    }

    // const TTNode* get(const ZobristHash &zhash) 
    // {
    // // The mask value is limited to half of the nelts in the _store, so
    // // (_nelt >> 1) - 1.  The '-1' is to set all ones.  If _nelt == 32
    // // (0b00010000), then mask == 15 (0b00000111).  
    // // The key is lshifted by 1, multiplying by 2.  So the key is
    // // always even, leaving room for the backup entry in _store at (key
    // // + 1).
    // const uint64_t key = (zhash.get_hash() & ((_nelt >> 1) - 1) << 1);
    // assert(key < _nelt);
    // TTNode *node = &_store[key];
    // if (node->_zobrist == zhash) {
    // return node;
    // }
    // else {
    // node = &_store[key+1];
    // }
    // return NULL;
    // }

  private:
    const std::unique_ptr<std::vector<TTNode> > _store;
    uint64_t _nelt;
    // uint64_t _size_MB;
    // uint64_t _hits, _miss, _collisions, _nelt;

};

#endif
