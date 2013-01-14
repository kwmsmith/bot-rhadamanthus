#include "arimaa_archive.h"
#include <cstdio>
#include <vector>
#include <sstream>
#include <algorithm>

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
