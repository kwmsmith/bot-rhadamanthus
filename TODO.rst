=======================================================================
TODO
=======================================================================

 o Speedup options -- faster bboard -> index conversion (can remove vector operations...):

    224         ulong occupied_traps = ~pos.bitBoards[Piece.EMPTY] & TRAPS;
    225         while (occupied_traps)
    226         {
    227             ulong tbit = occupied_traps & -occupied_traps;
    228             occupied_traps ^= tbit;
    229             bitix tix = bitindex(tbit);
    230             Piece tpiece = pos.pieces[tix];
    231             Side tside = (tpiece > Piece.WELEPHANT) ? Side.BLACK : Side.WHITE;
    232
    233             if (safe_traps[tside] & tbit)
    234             {
    235                 score = -piece_value[tpiece] * ON_TRAP_MUL / 2.0;
    236             } else {
    237                 score = -piece_value[tpiece] * ON_TRAP_MUL;
    238             }

 o Use this "tofrom" trick using xor to move a piece in one operation:

    1180             ulong tofrom = step.frombit ^ step.tobit;
    1181             bitBoards[Piece.EMPTY] ^= tofrom;
    1182             bitBoards[piece] ^= tofrom;
    1183             placement[piece_side] ^= tofrom;
    1184             pieces[fromix] = Piece.EMPTY;
    1185             pieces[toix] = piece;

 o Consider representing a Step as just "to" and "from" bytes, removing the piece and action.

 o Would allow a "Move" to just have a statically allocated array of 4 Steps internally, rather than a vector.

 o Add a "pieces[64]" array -- the piece at each spot.  What about black / white?

 o Consider keeping a "frozen" bboard in the GameState class...


alpha-beta search:

    Implement basic ab, nothing fancy, get working.

    quiescence search

    null move

    negascout / PVS (see wikipedia)

    history? killer move?

move ordering:

    Simple:

        moves w/ push pulls before otherwise.

        moves w/ captures before otherwise.

        moves w/ strongest pieces before otherwise.

    Full:

        Extract essential features from AI evaluator to make a fast move
        order-er.

Evaluation:

    Material evaluation:

        FAME, DAME, etc...

    Trap evaluation:

        Trap control
