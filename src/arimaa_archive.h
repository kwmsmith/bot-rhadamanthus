#ifndef ARIMAA_ARCHIVE_H_
#define ARIMAA_ARCHIVE_H_

#include <cstdio>
#include <string>
#include <vector>
#include "boost/unordered_map.hpp"
#include "boost/scoped_ptr.hpp"
#include "gamestate.h"

typedef boost::unordered_map<std::string, std::string> map_ss;

std::vector<std::string> &split(const std::string &, const char, std::vector<std::string> &);
std::vector<std::string> &split(const std::string &, const std::string &, std::vector<std::string> &);
std::vector<std::string> split(const std::string &, const char);

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

        unsigned int get_plycount() const;

        bool verify() const;

    private:

        boost::scoped_ptr<std::vector<std::vector<Step> > > movelist_;

        ArchivedGame(const ArchivedGame&);
        ArchivedGame& operator=(const ArchivedGame&);
};

#endif // ARIMAA_ARCHIVE_H_
