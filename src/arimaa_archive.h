#ifndef ARIMAA_ARCHIVE_H_
#define ARIMAA_ARCHIVE_H_

#include <cstdio>
#include <string>
#include <vector>
#include "boost/unordered_map.hpp"

typedef boost::unordered_map<std::string, std::string> map_ss;

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

std::vector<std::string> &split(const std::string &s, const char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, const char delim);

#endif // ARIMAA_ARCHIVE_H_
