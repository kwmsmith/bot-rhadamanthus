#include "arimaa_archive.h"
#include <cstdio>
#include <vector>
#include <sstream>
#include <algorithm>
#include <boost/unordered_map.hpp>

using namespace std;

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
        "events"
    };
    static vector<string> columns(columns_, columns_ + 29);
    return columns;
}

vector<string> ArimaaArchive::get_record()
{
    string line = "";
    vector<string> record;
    if(!get_line(archive_fh_, line))
        return record;
    record = split(line, '\t');
}

int ArimaaArchive::init(const char *fname)
{
    FILE *fh = fopen(fname, "rb");
    if (fh == NULL) {
        return 0;
    }
    archive_fh_ = fh;
    return 1;
}

ArimaaArchive::~ArimaaArchive()
{
    if(archive_fh_ != NULL)
        fclose(archive_fh_);
    archive_fh_ = NULL;
}

int ArimaaArchive::read_header()
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

bool get_line(FILE *fp, string &line)
{
    char buf[256];
    if (fp == NULL)
        return false;
    line.clear();
    size_t olen=0, delta=0;
    while(1) {
        if(fgets(buf, 256, fp) == NULL)
            break;
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
