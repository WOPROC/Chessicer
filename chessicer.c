//Defining headers, data types, etc.\\

#include <stdio.h>
#define U64 unsigned long long




//******************************************\\

// macros \\ e

#define setBit(bitNum, sqr) (bitNum |= (1ULL << sqr))
#define get_bit(bitNum, sqr) (bitNum & (1ULL << sqr))
#define popBit(bitNum, sqr) (get_bit(bitNum,sqr) ?bitNum ^= (1ULL << sqr) : 0 )

//^= makes 0 --> 1, and 1 --> 0.

// enumerations \\

enum {
    a8,b8,c8,d8,e8,f8,g8,h8,
    a7,b7,c7,d7,e7,f7,g7,h7,
    a6,b6,c6,d6,e6,f6,g6,h6,
    a5,b5,c5,d5,e5,f5,g5,h5,
    a4,b4,c4,d4,e4,f4,g4,h4,
    a3,b3,c3,d3,e3,f3,g3,h3,
    a2,b2,c2,d2,e2,f2,g2,h2,
    a1,b1,c1,d1,e1,f1,g1,h1
};

enum { white, black };
//******************************************\\


// testing functions \\

void printBoard(U64 bit)
{
    for(int r = 0; r < 8; r++) //r = rank
    {
        for(int f = 0; f < 8; f++) //f = file
        {
            int square = (r * 8) + f;

            if (!f) {
                printf("  %d ", 8 - r);
            }

            //print "bite state" (0=unoccupied, 1 = occupied
            printf(" %d ", get_bit(bit, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n     A  B  C  D  E  F  G  H \n \n");
}

//******************************************\\
//              Constants                   \\
//******************************************\\


//

/* 
* This is what notAFile is:

  8  0  1  1  1  1  1  1  1
  7  0  1  1  1  1  1  1  1
  6  0  1  1  1  1  1  1  1
  5  0  1  1  1  1  1  1  1
  4  0  1  1  1  1  1  1  1
  3  0  1  1  1  1  1  1  1
  2  0  1  1  1  1  1  1  1
  1  0  1  1  1  1  1  1  1

     A  B  C  D  E  F  G  H


*/
const U64 notAfile = 18374403900871474942ULL;
const U64 notHfile = 9187201950435737471ULL;
const U64 notHGfile = 4557430888798830399ULL;
const U64 notABfile = 18229723555195321596ULL;

//******************************************\\
//******************************************\\



//******************************************\\
//              Movement                    \\
//******************************************\\

U64 pawnAtks[2][64];

U64 knightAtks[64];

U64 genPawnAtks(int side, int square)
{
    U64 attks = 0ULL; //"attack map"
    U64 bitboard = 0ULL; //the board
    setBit(bitboard, square);
 

    if(!side)//white pawns
    {
        //this creates pawn captures
        if ((bitboard >> 7) & notAfile) attks |= (bitboard >> 7);
        if((bitboard >> 9)&notHfile) attks |= (bitboard >> 9);
    }
    else //black pawns
    {
        //this creates pawn captures
        if ((bitboard << 7) & notHfile) attks |= (bitboard << 7);
        if ((bitboard << 9) & notAfile) attks |= (bitboard << 9);
    }
    return attks;
}
U64 genKnightAtks(int sqr) {
    U64 attks = 0ULL; //"attack map"
    U64 bitboard = 0ULL; //the board
    setBit(bitboard, sqr);
  

    //knight bit-code attacks: 17, 15, 10, 6
    if ((bitboard >> 17) & notHfile) attks |= (bitboard >> 17);
 if ((bitboard >> 15) & notAfile) attks |= (bitboard >> 15);
    if ((bitboard >> 10) & notHGfile) attks |= (bitboard >> 10);
    if ((bitboard >> 6) & notABfile) attks |= (bitboard >> 6);

    if ((bitboard << 17) & notAfile) attks |= (bitboard << 17);
    if ((bitboard << 15) & notHfile) attks |= (bitboard << 15);
    if ((bitboard << 10) & notABfile) attks |= (bitboard << 10);
    if ((bitboard << 6) & notHGfile) attks |= (bitboard << 6);

    return attks;

}
//leaper pieces
void initLeaperAttacks() {
    for (int sqr = 0; sqr < 64; sqr++) {
        pawnAtks[white][sqr] = genPawnAtks(white, sqr);
        pawnAtks[black][sqr] = genPawnAtks(black, sqr);
    }
}



//******************************************\\
//******************************************\\
//******************************************\\


int main()
{
    initLeaperAttacks();
    printBoard(genKnightAtks(g4));
    return 0;
}