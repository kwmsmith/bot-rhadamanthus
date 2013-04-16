Arimaa Match Rules
==================

The following rules apply to official Arimaa games that are played for ranks, tournaments, contests, championships, etc.

Match Game Requirements
-----------------------

Game must be recorded. All moves made in the game must be recorded using the
Notation for Recording Arimaa Games

Time controls must be used. An official Arimaa match must be played with some
form of Arimaa Time Control. Details of time controls are given below.

The time control must be chosen to allow a game to reach at least 80 moves.

If a game is stopped due to reaching the time control limit then the winner is
determined by scoring the game as follows: the player who currently has or
most recently had more pieces left after the completion of a turn wins.
Otherwise the player to move second (silver) wins.

A player may resign at any time to end the match. However, resigning is highly
discouraged. Continuing a game to a natural finish is highly encouraged.

Time Controls
-------------

The Arimaa time controls were chosen to achieve the following:

    1. Keep the game moving, by not allowing a player to take forever to make a
    move and bore the spectators.

    2. Allow a lot of flexibility in specifying the time controls.

    3. Allow for a fixed upper limit on the total game time for practical reasons.

    4. Attempt to prevent a player from losing the game due to time while imposing
    these time limits.

    5. Preserve the quality of the game while imposing these time limits.

    6. Allow for the most common time controls used in Chess.  Thus the Arimaa
    time controls support all the common time controls used in Chess and more.

The time control used for Arimaa is specified as: M/R/P/L/G/T, where:

    M is the number of minutes:seconds per move; required

    R is the number of minutes:seconds in reserve; required

    P is the percent of unused move time that gets added to the reserve;
    optional defaults to 100

    L is the number of minutes:seconds to limit the reserve; 0 means no limit;
    optional; defaults to 0

    G is the number of hours:minutes after which time the game is halted and
    the winner is determined by score. G can also be specified as the maximum
    number of moves by ending with 't'; 0 means no limit; optional; defaults
    to 0

    T is the number of minutes:seconds within which a player must make the
    move; 0 means no limit; optional; defaults to 0

On each turn a player gets a fixed amount of time per move (M) and there may
be some amount of time left in the reserve (R).

If a player does not complete the move within the move time (M) then the time
in reserve (R) is used.  If there is no more time remaining in reserve and the
player has not completed the move then the player automatically loses.  Even
if there is time left in the move or reserve, but the player has not made the
move within the maximum time allowed for moves (T) then the player
automatically loses.

If a player completes the move in less than the time allowed for the move (M),
then a percentage (P) of the remaining time is added to the players reserve.
The result is rounded to the nearest second.  This parameter is optional and
if not specified, it is assumed to be 100%.

An upper limit (L) can be given for the reserve so that the reserve does not
exceed L when more time is added to the reserve.  If the initial reserve
already exceeds this limit then more time is not added to the reserve until it
falls below this limit. The upper limit for the reserve is optional and if not
given or set to 0 then it implies that there is no limit on how much time can
be added to the reserve.

For practical reasons a total game time (G) may be set.  If the game is not
finished within this allotted time then the game is halted and the winner is
determined by scoring the game.  This parameter is optional and if not given
(or set to 0) it means there is no limit on the game time. Also instead of an
upper limit for the total game time, an upper limit for the total number of
turns each player can make may be specified by adding the letter 't' after the
number. After both players have taken this many turns and the game is not
finished the winner is determined by scoring the game.

For games which use a time per move of less than 1 minute, both players are
always given 1 minute of time to setup the initial position in the first move
of the game.  If the setup is not completed in 1 minute then the reserve time
(R) is also used.  The unused time from the setup move is not added to the
reserve time.

Example 1: 0/5 means 0 minutes per move with 5 minutes in reserve (per
player). This is equivalent to G/5 in Chess; it means each player has a total
of 5 minutes of time to play.  If a player runs out of time before the game is
over, the player loses.  This is known as Blitz or "Sudden Death" time control
in Chess.

Example 2: 0:12/5 means 12 seconds per move with 5 minutes in reserve and all
of the unused time from each move is added to the reserve time.  It is similar
to "5 12" in Chess which means "Game in 5 minutes with a 12 second increment".
After each move 12 seconds is added to the remaining time. This is known as
Incremental time control in Chess.

Example 3: 3/0 means 3 minute per move and no reserve time, but 100 percent of
the unused time for each move is added to the reserve.  This guarantees that
each player will make at least 40 moves in 2 hours.  This is similar to the
"40/2" Quota System time control used in Chess.

Example 4: 0:30/5/100/3 means 30 seconds per move with 5 minutes in reserve
and 100% of the unused time from each move is added to the reserve time.  When
the reserve already exceeds the limit, more time is not added to it.  When the
reserve falls below 3 minutes more time can be added to it, but the reserve is
capped at 3 minutes.

Example 5: 4/2/50/10/6 This means 4 minutes per move with a starting reserve
of 2 minutes.  After the move 50% of the time remaining for the move (rounded
to the nearest second) is added to the reserve such that it does not exceed 10
minutes.  There is a limit of 6 hours for the game after which time the game
is halted and the winner is determined by score.  

Example 6: 4/4/100/4/6 This means 4 minutes per move and a starting reserve of
4 minutess. 100% percent of the unused move time gets added to the reserve
such that it does not exceed 4 minutes.  There is a time limit of 6 hour for
the game after which the winner is determined by score.

Example 7: 4/4/100/4/90t This is the same as above, but the game ends after
both players have made 90 moves. Thus it ends after move 90 of silver is
completed.

Example 8: 4/4/100/4/90t/5 This is the same as above, but the players may not
take more than 5 minutes for each turn even if there is still time remaining
in reserve.

Different time units for any of the time control fields can be specifed by
adding one of the following letters after the numbers.  In such cases the
letter serves as the seperator and : should not be used.

      s - seconds
      m - minutes
      h - hours
      d - days

For example:  24h5m10s/0/0/0/60d means 24 hours, 5 minutes and 10 seconds
per move and the game must end after 60 days.  Such a time control may be
used in a postal type match.

The game time parameter (G) can also be specified in terms of maximum number
of turns each player can make by adding the letter t after the number.

Notes
-----

The rules for official matches are subject to change by the game desingers as
more experience is gained with the game. These rules are effective July 1st,
2008.

Copyright © 1999-present www.arimaa.com. All Rights Reserved.  You are free to
copy and distribute these rules under the terms provided by the Arimaa Public
License
