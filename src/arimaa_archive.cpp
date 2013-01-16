#include "arimaa_archive.h"
#include <cstdio>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>

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
    vector<string> values = split(line, '\t');
    if(values.size() != get_num_columns()) {
        return map_ss();
    }
    vector<string> canon = canonical_columns();
    map_ss mp;
    for(vsit key_it = canon.begin(), val_it = values.begin() ;
            key_it != canon.end(), val_it != values.end() ;
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
    vector<string> columns = split(header, '\t');
    vector<string> canon = canonical_columns();
    if (!equal(columns.begin(), columns.end(),  canon.begin())) {
        return 0;
    }
    return 1;
}

vector<string> &split(const string &s, const string &delim, vector<string> &elems)
{
    size_t start = 0, stop = 0;

    stop = s.find(delim, start);
    while(stop != string::npos) {
        elems.push_back(s.substr(start, stop-start));
        start = stop + delim.length();
        stop = s.find(delim, start);
    }
    return elems;
}

vector<string> &split(const string &s, const char delim, vector<string> &elems) 
{
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, const char delim) 
{
    vector<string> elems;
    return split(s, delim, elems);
}

static bool get_line(FILE *fp, string &line)
{
    char buf[256];
    if (fp == NULL)
        return false;
    line.clear();
    size_t olen=0, delta=0;
    while(1) {
        if(fgets(buf, 256, fp) == NULL || feof(fp) || ferror(fp))
            return false;
        olen = line.length();
        line += buf;
        delta = line.length() - olen;
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
    split(record.at("movelist"), "\\n", all_moves);
    vector<string> placement_white, placement_black;
    split(all_moves[0], ' ', placement_white);
    split(all_moves[1], ' ', placement_black);
    return (color == W ? placement_white : placement_black);
}

ArchiveGame::ArchiveGame(const map_ss& record)
    : plycount_(0),
      setup_white_(new vector<string>),
      setup_black_(new vector<string>),
      movelist_(new vector<string>)
{
    plycount_ = strtoul(record.at("plycount").c_str(), NULL, 0);

    vector<string> all_moves;
    split(record.at("movelist"), "\\n", all_moves);

    split(all_moves[0], ' ', *setup_white_);
    split(all_moves[1], ' ', *setup_black_);

    for (vector<string>::iterator it=all_moves.begin()+2; it != all_moves.end(); ++it)
        movelist_->push_back(*it);
    assert(movelist_->size() == (plycount_-1) * 2 || movelist_->size() == (plycount_-1) * 2 - 1);
}

unsigned int ArchiveGame::get_plycount() const
{
    return plycount_;
}
