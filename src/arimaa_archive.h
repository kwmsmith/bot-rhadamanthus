#ifndef ARIMAA_ARCHIVE_H_
#define ARIMAA_ARCHIVE_H_

#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>

#include "util.h"
#include "step.h"
#include "gamestate.h"

typedef std::unordered_map<std::string, std::string> map_ss;

std::vector<std::string> get_record_setup(const map_ss& record, int color);

class ArimaaArchive 
{
  public:

    ArimaaArchive() : archive_fh_(NULL) { }

    ~ArimaaArchive();

    int init(const char *fname);

    static std::vector<std::string> canonical_columns();

    const static unsigned int get_num_columns() {
      return canonical_columns().size();
    }

    map_ss get_record();

  private:

    int read_and_validate_header();

    FILE *archive_fh_;

    // disable copying / copy constructor.
    ArimaaArchive(const ArimaaArchive&);
    ArimaaArchive& operator=(const ArimaaArchive&);
};

class ArchivedGame
{

  public:

    explicit ArchivedGame(const map_ss&);

    unsigned int get_numply() const;

    bool verify() const;

    const std::vector<Step> &get_move(unsigned int idx) const;

  private:

    const std::auto_ptr<std::vector<std::vector<Step> > > movelist_;

    ArchivedGame(const ArchivedGame&);
    ArchivedGame& operator=(const ArchivedGame&);
};

bool setup(const ArchivedGame& ag, GameState *gs);
bool make_moves(const ArchivedGame& ag, GameState *gs);
bool play(const ArchivedGame& ag, GameState *gs);

#endif // ARIMAA_ARCHIVE_H_
