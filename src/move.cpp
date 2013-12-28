#include <inttypes.h>
#include <limits.h>
#include <sparsehash/dense_hash_set>
#include <sparsehash/dense_hash_map>

#include "move.h"

typedef google::dense_hash_set<uint64_t, GameStateHash> hash_set;

typedef google::dense_hash_map<uint64_t, uint8_t, GameStateHash> hash_map;

static void move_counter_recur_(const GameState& gs, hash_map *seen, uint64_t *nmoves)
{
  uint64_t hash;

  std::vector<Delta> deltas;

  if (!gs.get_stepsleft())
    return;

  if (gs.get_stepsleft() >= 2) {
    generate_pushes(gs, &deltas);
    generate_pulls(gs, &deltas);
  }
  generate_steps(gs, &deltas);

  for(delta_it it=deltas.begin(); it != deltas.end(); ++it) {
    GameState new_gs = gs;
    apply_delta_and_capture(*it, &new_gs);
    hash = new_gs.get_hash();
    // check for duplicates.
    if (seen->count(hash)) {
      if ((*seen)[hash] >= new_gs.get_stepsleft())
        continue;
    } else {
      (*nmoves)++;
    }
    (*seen)[hash] = new_gs.get_stepsleft();
    move_counter_recur_(new_gs, seen, nmoves);
  }
}

uint64_t move_counter(const GameState& gs)
{
  hash_map seen(20000);
  seen.set_empty_key(ULLONG_MAX);
  uint64_t nmoves = 0;
  move_counter_recur_(gs, &seen, &nmoves);
  return nmoves;
}

void generate_unique_moves(const GameState& gs, std::vector<Move> *moves)
{
  uint64_t hash;
  std::vector<Delta> deltas;
  hash_set seen(20000);
  seen.set_empty_key(ULLONG_MAX);
  moves->reserve(1000);
  size_t idx = moves->size();

  moves->push_back(Move(gs));
  seen.insert(gs.get_hash());

  while (idx < moves->size()) {
    Move this_move = (*moves)[idx];
    const GameState& gs = this_move.get_gamestate();
    const unsigned int steps_left = this_move.get_stepsleft();
    assert(steps_left == gs.get_stepsleft());
    deltas.clear();
    if (steps_left) {
      generate_steps(gs, &deltas);
    } 
    if (steps_left >= 2) {
      generate_pushes(gs, &deltas);
      generate_pulls(gs, &deltas);
    }
    for(delta_it it=deltas.begin(); it != deltas.end(); ++it) {
      Move mv = Move(this_move);
      mv.add_delta(*it);
      hash = mv.get_zobrist_hash();
      if (!seen.count(hash)) {
        seen.insert(hash);
        moves->push_back(mv);
      }
    }
    ++idx;
  }
}

void generate_moves(const GameState& gs, std::vector<Move> *moves)
{
  std::vector<Delta> deltas;
  size_t idx = moves->size();
  moves->push_back(Move(gs));

  while (idx < moves->size()) {
    Move this_move = (*moves)[idx];
    const GameState& gs = this_move.get_gamestate();
    const unsigned int steps_left = this_move.get_stepsleft();
    assert(steps_left == gs.get_stepsleft());
    deltas.clear();
    if (steps_left) {
      generate_steps(gs, &deltas);
    }
    if (steps_left >= 2) {
      generate_pushes(gs, &deltas);
      generate_pulls(gs, &deltas);
    }
    for(delta_it it=deltas.begin(); it != deltas.end(); ++it) {
      Move mv = Move(this_move);
      mv.add_delta(*it);
      moves->push_back(mv);
    }
    ++idx;
  }
}

const std::string Move::to_std_string() const
{
  std::string s;
  for (int i=0; i < total_steps_taken_; ++i) {
    s += steps_[i].to_std_string() + " ";
  }
  return s;
}
