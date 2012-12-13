%Module board

struct Board {

%TypeHeaderCode
#include <inttypes.h>
#include "board.h"
%End

    public:

       Board();
       Board(unsigned long board);

      unsigned long _board;
};
