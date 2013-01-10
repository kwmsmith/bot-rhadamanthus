#ifndef ARIMAA_ARCHIVE_H_
#define ARIMAA_ARCHIVE_H_

#include <cstdio>
#include <string>
#include <vector>

class ArimaaArchive 
{
    public:

        ArimaaArchive() : archive_fh_(NULL) { }

        ~ArimaaArchive();

        int init(const char *fname);

        std::vector<std::string> canonical_columns();

        int read_header();

        std::vector<std::string> get_record();

    private:

        FILE *archive_fh_;

        // disable copying / copy constructor.
        ArimaaArchive(const ArimaaArchive&);
        ArimaaArchive& operator=(const ArimaaArchive&);
};

bool get_line(FILE *fp, std::string &line);

std::vector<std::string> &split(const std::string &s, const char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, const char delim);

#endif // ARIMAA_ARCHIVE_H_
