//Defining headers, data types, etc.\\

#include <stdio.h>
#define U64 unsigned long long




//******************************************\\

// bit manipulation \\

#define setBit(bitNum, sqr) (bitNum |= (1ULL << sqr))
#define get_bit(bitNum, sqr) (bitNum & (1ULL << sqr))
#define popBit(bitNum, sqr) (get_bit(bitNum,sqr) ?bitNum ^= (1ULL << sqr) : 0 )

//^= makes 0 --> 1, and 1 --> 0.

//count all bits
static inline int countBits(U64 board) //inline just means function gets called fast basically
{
    int c = 0;
    while(board)
    {
        c++; //increment count

        board &= board - 1; //reset least significant 1st bit
        /*
        The LSB (least significant bit( is also known as the “rightmost” or “past-the-end” bit. 
        If the LSB is on the right, the architecture is called “little-endian.” For example, in a 
        little-endian architecture, the LSB of binary number 00000001 is 1.
        */
    }
    return c;
}

static inline int index_LS1stB(U64 board)
{
    if (board) //make sure board has values
    {
        return countBits((board & -1*board) - 1);
    }
    else
        return -1; //useless / invalid value
}


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

const U64 notAfile = 18374403900871474942ULL;
const U64 notHfile = 9187201950435737471ULL;
const U64 notHGfile = 4557430888798830399ULL;
const U64 notABfile = 18229723555195321596ULL;

const char *sqr2Cord[] = { //* = pointer
    "a8","b8","c8","d8","e8","f8","g8","h8",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a1","b1","c1","d1","e1","f1","g1","h1"
};
//******************************************\\
//******************************************\\



//******************************************\\
//              Movement                    \\
//******************************************\\

U64 pawnAtks[2][64];

U64 knightAtks[64];
U64 kingAtks[64];


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
U64 genKingAtks(int sqr){
    U64 attks = 0ULL; //"attack map"
    U64 bitboard = 0ULL; //the board
    setBit(bitboard, sqr);

    //make king attacks
    if(bitboard >>8) attks |= (bitboard >> 8);
    if((bitboard>>9) & notHfile) attks |= (bitboard >> 9);
    if ((bitboard >> 7) & notAfile) attks |= (bitboard >> 7);
    if ((bitboard >> 1) && notHfile) attks |= (bitboard >> 1);
    if (bitboard << 8) attks |= (bitboard << 8);
    if ((bitboard << 9) & notAfile) attks |= (bitboard << 9);
    if ((bitboard << 7) & notHfile) attks |= (bitboard << 7);
    if ((bitboard << 1) && notAfile) attks |= (bitboard << 1);

 

    return attks;

}
U64 genBishopAtks(int sqr) {
    U64 attks = 0ULL;

    //rank, files
    int r, f;
    
    //target ranks & files
    int tr = sqr / 8;
    int tf = sqr % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
        attks |= (1ULL << (r * 8 + f)); 
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
        attks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
        attks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
        attks |= (1ULL << (r * 8 + f));


    return attks;
}
U64 genRookAtks(int sqr) {
    U64 attks = 0ULL;

    //rank, files
    int r, f;

    //target ranks & files
    int tr = sqr / 8;
    int tf = sqr % 8;
    for (r = tr + 1; r <= 6; r++) attks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) attks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) attks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) attks |= (1ULL << (tr * 8 + f));

    return attks;
}


///// "on the fly" movement
U64 live_genBishopAtks(int sqr, U64 block) {
    U64 attks = 0ULL;

    //rank, files
    int r, f;

    //target ranks & files
    int tr = sqr / 8;
    int tf = sqr % 8;

    //generate the attacks for bishop!
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) && block) break; //stop loop if piece is detected
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) && block) break; //stop loop if piece is detected
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) && block) break; //stop loop if piece is detected
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) && block) break; //stop loop if piece is detected
    }


    return attks;
}

U64 live_genRookAtks(int sqr, U64 blockage)
{
    U64 atks = 0ULL;
    int rank, file;
    int tr = sqr / 8;
    int tf = sqr % 8;

    for(rank = tr + 1; rank <= 7; rank++)
    {
        atks |= (1ULL << (rank * 8 + tf));
        if ((1ULL << (rank * 8 + tf)) & blockage) break;
    }
    for (rank = tr - 1; rank >= 0; rank--)
    {
        atks |= (1ULL << (rank * 8 + tf));
        if ((1ULL << (rank * 8 + tf)) & blockage) break;
    }
    for (file = tf + 1; file <= 7; file++)
    {
        atks |= (1ULL << (tr * 8 + file));
        if ((1ULL << (tr * 8 + file)) & blockage) break;
    }
    for(file = tf - 1; file >= 0; file--)
    {
        atks |= (1ULL << (tr * 8 + file));
        if ((1ULL << (tr * 8 + file)) & blockage) break;
    }

    return atks;
}


//leaper pieces
void initLeaperAttacks() {
    for (int sqr = 0; sqr < 64; sqr++) {
        pawnAtks[white][sqr] = genPawnAtks(white, sqr);
        pawnAtks[black][sqr] = genPawnAtks(black, sqr);
        knightAtks[sqr] = genKnightAtks(sqr);
        kingAtks[sqr] = genKingAtks(sqr);
    }
}



//******************************************\\
//******************************************\\
//******************************************\\


int main()
{
    initLeaperAttacks();

    //initalize taken squares
    U64 usedSpaces = 0ULL;
    setBit(usedSpaces, d7);
    setBit(usedSpaces, d2);
    setBit(usedSpaces, d1);
    setBit(usedSpaces, b4);
    setBit(usedSpaces, g4);
    
    printf("position: %s\n", sqr2Cord[index_LS1stB(usedSpaces)]);

    return 0;
}