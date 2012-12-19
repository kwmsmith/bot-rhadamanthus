%Module board

struct Board {

%TypeHeaderCode
#include <inttypes.h>
#include "board.h"
%End

    public:

        Board();
        Board(unsigned long);
        void add(unsigned long);
        // unsigned long __contains__(unsigned long);
        bool contains(unsigned long);
        void remove(unsigned long);

        unsigned long _board;
};
