#include <core/entry.h>

int main ( ) {
	blastbot::core::g_entry->entry ( );
	return 0;
}
/*
capture display(depending on how we do this, all of this i suppose, this may end up being really slow)

determine background color

determine interior block color

find grid, should always be an 8x8 grid that will go

A1 B1 C1 D1 E1 F1 G1 H1

A2 B2 C2 D2 E2 F2 G2 H2

A3 B3 C3 D3 E3 F3 G3 H3

A4 B4 C4 D4 E4 F4 G4 H4

A5 B5 C5 D5 E5 F5 G5 H5

A6 B6 C6 D6 E6 F6 G6 H6

A7 B7 C7 D7 E7 F7 G7 H7

A8 B8 C8 D8 E8 F8 G8 H8


determine filled blocks, vs unfilled blocks, output a grid generated in this same text format with (f) for filled blocked, unfilled gets left blank.

determine our pieces that require placement.

determine best position for said pieces based off of the grid,

actually place the piece in the said position.

our end goal is to place pieces fast enough to not lose our combo, and place them in positions with such a calculation that we will not end up running out of space to place our next pieces
to do this we need to determine what piece to place first that will still leave us with space to place our next pieces, and clear spaces for future pieces by filling in a row of the grid.
the solver needs to know not just "what's the best placement for this piece" but "what ordering of all 3 pieces leaves the board in the best state."
*/