#include "arimaa_archive.h"
#include <cstdio>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

static bool get_line(FILE *fp, string &line); // proto

vector<string> ArimaaArchive::canonical_columns()
{
  string columns_[] = {
    "id",
    "wplayerid",
    "bplayerid",
    "wusername",
    "busername",
    "wtitle",
    "btitle",
    "wcountry",
    "bcountry",
    "wrating",
    "brating",
    "wratingk",
    "bratingk",
    "wtype",
    "btype",
    "event",
    "site",
    "timecontrol",
    "postal",
    "startts",
    "endts",
    "result",
    "termination",
    "plycount",
    "mode",
    "rated",
    "corrupt",
    "movelist",
    "events",
  };
  // FIXME: remove the `29` in the line below.
  static vector<string> columns(columns_, columns_ + 29);
  return columns;
}

map_ss ArimaaArchive::get_record()
{
  typedef vector<string>::iterator vsit;
  string line = "";
  if(!get_line(archive_fh_, line)) {
    return map_ss();
  }
  vector<string> values;
  split(values, line, "\t", true);
  if(values.size() != get_num_columns()) {
    return map_ss();
  }
  vector<string> canon = canonical_columns();
  map_ss mp;
  for(vsit key_it = canon.begin(), val_it = values.begin() ;
      key_it != canon.end() && val_it != values.end() ;
      ++key_it, ++val_it)
    mp[*key_it] = *val_it;
  return mp;
}

int ArimaaArchive::init(const char *fname)
{
  FILE *fh = fopen(fname, "rb");
  if (fh == NULL) {
    return 0;
  }
  archive_fh_ = fh;
  return read_and_validate_header();
}

ArimaaArchive::~ArimaaArchive()
{
  if(archive_fh_ != NULL)
    fclose(archive_fh_);
  archive_fh_ = NULL;
}

int ArimaaArchive::read_and_validate_header()
{
  string header = "";
  if(!get_line(archive_fh_, header))
    return 0;
  vector<string> columns;
  split(columns, header, "\t", true);
  vector<string> canon = canonical_columns();
  if (!equal(columns.begin(), columns.end(),  canon.begin())) {
    return 0;
  }
  return 1;
}

static bool get_line(FILE *fp, string &line)
{
  char buf[256];
  if (fp == NULL)
    return false;
  line.clear();
  while(1) {
    if(fgets(buf, 256, fp) == NULL || feof(fp) || ferror(fp))
      return false;
    line += buf;
    if(line[line.length()-1] == '\n')
      break;
  }
  // remove the newline from the end.
  line.erase(line.end()-1);
  return true;
}

vector<string> get_record_setup(const map_ss& record, int color)
{
  typedef vector<string>::iterator vsit;
  vector<string> ret;
  if (record.count("movelist") != 1)
    return ret;
  vector<string> all_moves;
  split(all_moves, record.at("movelist"), "\\n", true);
  vector<string> placement_white, placement_black;
  split(placement_white, all_moves[0], " ", true);
  split(placement_black, all_moves[1], " ", true);
  return (color == W ? placement_white : placement_black);
}

bool vecstep_from_vecstr(const vector<string>& vecstr, vector<Step> *vecstep, int skip = 0)
{
  for(vector<string>::const_iterator it=vecstr.begin()+skip;
      it != vecstr.end(); ++it) {
    Step step = make_step(*it);
    if (!step.is_valid()) return false;
    vecstep->push_back(step);
  }
  return true;
}

  ArchivedGame::ArchivedGame(const map_ss& record)
: movelist_(new vector<vector<Step> >)
{
  unsigned long plycount_ = strtoul(record.at("plycount").c_str(), NULL, 0);

  vector<string> all_moves;
  split(all_moves, record.at("movelist"), "\\n", true);

  bool res = false;

  for (vector<string>::iterator it=all_moves.begin();
      it != all_moves.end(); ++it) {
    vector<string> steps_str;
    split(steps_str, *it, " ", true);
    assert(steps_str.size() >= 1);
    vector<Step> steps;
    res = vecstep_from_vecstr(steps_str, &steps, 1);
    assert(res);
    movelist_->push_back(steps);
  }
  assert(    movelist_->size() == (plycount_) * 2 
      || movelist_->size() == (plycount_) * 2 - 1);
}

unsigned int ArchivedGame::get_numply() const
{
  // NOTE: NOT the same as the "plycount" entry in the game archive!
  // This is the total number of plies, where a single ply is one full move
  // by black or white.
  return movelist_->size();
}

bool ArchivedGame::verify() const
{
  assert(0);
  return false;
}

const vector<Step> &ArchivedGame::get_move(unsigned int idx) const
{
  return movelist_->at(idx);
}

bool setup(const ArchivedGame& ag, GameState *gs)
{
  for (unsigned int idx=0; idx < 2; ++idx) {
    const vector<Step> &full_move = ag.get_move(idx);
    for (vector<Step>::const_iterator it=full_move.begin();
        it != full_move.end(); ++it) {
      if (!it->is_placement())
        return false;
      if (!gs->add_piece_at(*it))
        return false;
    }
    gs->flip_color();
  }
  return true;
}

bool make_moves(const ArchivedGame& ag, GameState *gs)
{
  const unsigned int numply = ag.get_numply();

  for (unsigned int idx=2; idx < numply; ++idx) {
    const vector<Step> &full_move = ag.get_move(idx);
    for (vector<Step>::const_iterator it=full_move.begin();
        it != full_move.end(); ++it) {
      if (it->is_capture())
        // TODO: XXX: verify_capture(*gs, *it);
        continue;
      if (!gs->move_piece(*it))
        return false;
    }
    gs->flip_color();
  }
  return true;
}

bool play(const ArchivedGame& ag, GameState *gs)
{
  if (!setup(ag, gs))
    return false;
  return make_moves(ag, gs);
}
